#include <bro.h>
#include <mysql.h>
#include <sstream>

// ─────────────────────────────────────────────
//  DB Configuration — change these as needed
// ─────────────────────────────────────────────
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "poonamra1"           // your MySQL root password
#define DB_NAME "edubase"
#define DB_PORT 3306

// ─────────────────────────────────────────────
//  Helper: escape a string for safe SQL insertion
// ─────────────────────────────────────────────
string sqlEscape(MYSQL *conn, const string &s) {
    char *buf = new char[s.size() * 2 + 1];
    mysql_real_escape_string(conn, buf, s.c_str(), s.size());
    string result(buf);
    delete[] buf;
    return result;
}

// ─────────────────────────────────────────────
//  Helper: build a simple JSON response
// ─────────────────────────────────────────────
string jsonResp(const string &status, const string &message) {
    return "{\"status\":\"" + status + "\",\"message\":\"" + message + "\"}";
}

// ─────────────────────────────────────────────
//  Helper: get a DB connection (caller must close)
// ─────────────────────────────────────────────
MYSQL* getConn() {
    MYSQL *conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, NULL, 0)) {
        mysql_close(conn);
        return NULL;
    }
    return conn;
}

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────
int main() {
    try {
        Bro bro;
        bro.setStaticResourceFolder("static");

        // ── Startup: verify DB connection ──────────────────
        bro.addStartupService(1, []() {
            MYSQL *conn = getConn();
            if (!conn) {
                cout << "[ERROR] Cannot connect to MySQL! Check DB_HOST/DB_USER/DB_PASS/DB_NAME in varnit.cpp" << endl;
                cout << "        Also make sure you ran: mysql -u root -p < db_setup.sql" << endl;
            } else {
                cout << "[OK]    MySQL connected successfully to database '" << DB_NAME << "'" << endl;
                mysql_close(conn);
            }
        });

        // ── Startup: init app container ──────────────────
        static long global_total_visits = 0;
        bro.addStartupService(2, [](ApplicationLevelContainer &alc) {
            long *ptr = &global_total_visits;
            alc.set("total_visits_ptr", ptr, NULL, NULL);
            cout << "[OK]    Global visit counter initialized" << endl;
        });

        // ══════════════════════════════════════
        //  API: GET /api/stats
        //  Returns { students: N, faculty: N, visits: N }
        // ══════════════════════════════════════
        bro.get("/api/stats", [](Request &request, Response &response, ApplicationLevelContainer &alc) {
            response.setContentType("application/json");

            // Increment global visit counter via pointer
            long *vptr = NULL;
            alc.get("total_visits_ptr", &vptr, NULL, NULL);
            if (vptr) (*vptr)++;
            long visits = vptr ? *vptr : 0;
            MYSQL *conn = getConn();
            if (!conn) { response << jsonResp("error", "DB error"); return; }

            int sc = 0, fc = 0;
            if (mysql_query(conn, "SELECT COUNT(*) FROM students") == 0) {
                MYSQL_RES *r = mysql_store_result(conn);
                MYSQL_ROW row = mysql_fetch_row(r);
                if (row) sc = atoi(row[0]);
                mysql_free_result(r);
            }
            if (mysql_query(conn, "SELECT COUNT(*) FROM faculty") == 0) {
                MYSQL_RES *r = mysql_store_result(conn);
                MYSQL_ROW row = mysql_fetch_row(r);
                if (row) fc = atoi(row[0]);
                mysql_free_result(r);
            }
            mysql_close(conn);

            ostringstream oss;
            oss << "{\"students\":" << sc << ",\"faculty\":" << fc << ",\"visits\":" << visits << "}";
            response << oss.str();
        });

        // ══════════════════════════════════════
        //  API: GET /api/students
        //  Returns JSON array of all students
        // ══════════════════════════════════════
        bro.get("/api/students", [](Request &request, Response &response) {
            response.setContentType("application/json");
            MYSQL *conn = getConn();
            if (!conn) { response << "[]"; return; }

            if (mysql_query(conn, "SELECT id,name,roll,branch,year,email,phone FROM students ORDER BY created_at DESC")) {
                mysql_close(conn); response << "[]"; return;
            }
            MYSQL_RES *res = mysql_store_result(conn);
            MYSQL_ROW row;
            ostringstream oss;
            oss << "[";
            bool first = true;
            while ((row = mysql_fetch_row(res))) {
                if (!first) oss << ",";
                first = false;
                oss << "{"
                    << "\"id\":"     << row[0] << ","
                    << "\"name\":\""   << (row[1] ? row[1] : "") << "\","
                    << "\"roll\":\""   << (row[2] ? row[2] : "") << "\","
                    << "\"branch\":\"" << (row[3] ? row[3] : "") << "\","
                    << "\"year\":\""   << (row[4] ? row[4] : "") << "\","
                    << "\"email\":\""  << (row[5] ? row[5] : "") << "\","
                    << "\"phone\":\""  << (row[6] ? row[6] : "") << "\""
                    << "}";
            }
            oss << "]";
            mysql_free_result(res);
            mysql_close(conn);
            response << oss.str();
        });

        // ══════════════════════════════════════
        //  API: POST /api/add_student
        //  Params: name, roll, branch, year, email, phone
        // ══════════════════════════════════════
        bro.post("/api/add_student", [](Request &request, Response &response) {
            response.setContentType("application/json");
            
            string name   = request["name"];
            string roll   = request["roll"];
            string branch = request["branch"];
            string year   = request["year"];
            string email  = request["email"];
            string phone  = request["phone"];

            if (name.empty() || roll.empty()) {
                response << jsonResp("error", "Name and Roll Number are required");
                return;
            }
            MYSQL *conn = getConn();
            if (!conn) { response << jsonResp("error", "Database connection failed"); return; }

            string q = "INSERT INTO students (name,roll,branch,year,email,phone) VALUES ('"
                + sqlEscape(conn, name) + "','"
                + sqlEscape(conn, roll) + "','"
                + sqlEscape(conn, branch) + "','"
                + sqlEscape(conn, year) + "','"
                + sqlEscape(conn, email) + "','"
                + sqlEscape(conn, phone) + "')";

            if (mysql_query(conn, q.c_str())) {
                string err = mysql_error(conn);
                mysql_close(conn);
                if (err.find("Duplicate") != string::npos)
                    response << jsonResp("error", "Roll number already exists");
                else
                    response << jsonResp("error", "Failed to add student: " + err);
                return;
            }
            mysql_close(conn);
            cout << "[ADD STUDENT] " << name << " | Roll: " << roll << endl;
            response << jsonResp("success", "Student " + name + " added successfully!");
        });

        // ══════════════════════════════════════
        //  API: POST /api/delete_student
        // ══════════════════════════════════════
        bro.post("/api/delete_student", [](Request &request, Response &response) {
            response.setContentType("application/json");
            
            string id = request["id"];
            if (id.empty()) { response << jsonResp("error", "ID required"); return; }

            MYSQL *conn = getConn();
            if (!conn) { response << jsonResp("error", "Database connection failed"); return; }

            string q = "DELETE FROM students WHERE id=" + sqlEscape(conn, id);
            mysql_query(conn, q.c_str());
            mysql_close(conn);
            cout << "[DELETE STUDENT] ID: " << id << endl;
            response << jsonResp("success", "Student deleted successfully");
        });

        // ══════════════════════════════════════
        //  API: GET /api/faculty
        //  Returns JSON array of all faculty
        // ══════════════════════════════════════
        bro.get("/api/faculty", [](Request &request, Response &response) {
            response.setContentType("application/json");
            MYSQL *conn = getConn();
            if (!conn) { response << "[]"; return; }

            if (mysql_query(conn, "SELECT id,name,emp_id,dept,designation,email,phone,specialization,experience FROM faculty ORDER BY created_at DESC")) {
                mysql_close(conn); response << "[]"; return;
            }
            MYSQL_RES *res = mysql_store_result(conn);
            MYSQL_ROW row;
            ostringstream oss;
            oss << "[";
            bool first = true;
            while ((row = mysql_fetch_row(res))) {
                if (!first) oss << ",";
                first = false;
                oss << "{"
                    << "\"id\":"                << row[0] << ","
                    << "\"name\":\""            << (row[1] ? row[1] : "") << "\","
                    << "\"emp_id\":\""          << (row[2] ? row[2] : "") << "\","
                    << "\"dept\":\""            << (row[3] ? row[3] : "") << "\","
                    << "\"designation\":\""     << (row[4] ? row[4] : "") << "\","
                    << "\"email\":\""           << (row[5] ? row[5] : "") << "\","
                    << "\"phone\":\""           << (row[6] ? row[6] : "") << "\","
                    << "\"specialization\":\""  << (row[7] ? row[7] : "") << "\","
                    << "\"experience\":"        << (row[8] ? row[8] : "0")
                    << "}";
            }
            oss << "]";
            mysql_free_result(res);
            mysql_close(conn);
            response << oss.str();
        });

        // ══════════════════════════════════════
        //  API: POST /api/add_faculty
        //  Params: name,emp_id,dept,designation,email,phone,specialization,experience
        // ══════════════════════════════════════
        bro.post("/api/add_faculty", [](Request &request, Response &response) {
            response.setContentType("application/json");
            
            string name   = request["name"];
            string emp_id = request["emp_id"];
            string dept   = request["dept"];
            string desig  = request["designation"];
            string email  = request["email"];
            string phone  = request["phone"];
            string spec   = request["specialization"];
            string exp    = request["experience"];
            if (exp.empty()) exp = "0";

            if (name.empty() || emp_id.empty()) {
                response << jsonResp("error", "Name and Employee ID are required");
                return;
            }
            MYSQL *conn = getConn();
            if (!conn) { response << jsonResp("error", "Database connection failed"); return; }

            string q = "INSERT INTO faculty (name,emp_id,dept,designation,email,phone,specialization,experience) VALUES ('"
                + sqlEscape(conn, name) + "','"
                + sqlEscape(conn, emp_id) + "','"
                + sqlEscape(conn, dept) + "','"
                + sqlEscape(conn, desig) + "','"
                + sqlEscape(conn, email) + "','"
                + sqlEscape(conn, phone) + "','"
                + sqlEscape(conn, spec) + "',"
                + sqlEscape(conn, exp) + ")";

            if (mysql_query(conn, q.c_str())) {
                string err = mysql_error(conn);
                mysql_close(conn);
                if (err.find("Duplicate") != string::npos)
                    response << jsonResp("error", "Employee ID already exists");
                else
                    response << jsonResp("error", "Failed to add faculty: " + err);
                return;
            }
            mysql_close(conn);
            cout << "[ADD FACULTY] " << name << " | EmpID: " << emp_id << endl;
            response << jsonResp("success", "Faculty " + name + " added successfully!");
        });

        // ══════════════════════════════════════
        //  API: POST /api/delete_faculty
        // ══════════════════════════════════════
        bro.post("/api/delete_faculty", [](Request &request, Response &response) {
            response.setContentType("application/json");

            string id = request["id"];
            if (id.empty()) { response << jsonResp("error", "ID required"); return; }

            MYSQL *conn = getConn();
            if (!conn) { response << jsonResp("error", "Database connection failed"); return; }

            string q = "DELETE FROM faculty WHERE id=" + sqlEscape(conn, id);
            mysql_query(conn, q.c_str());
            mysql_close(conn);
            cout << "[DELETE FACULTY] ID: " << id << endl;
            response << jsonResp("success", "Faculty deleted successfully");
        });

        // ══════════════════════════════════════
        //  API: GET /report (Renders CHTML template)
        // ══════════════════════════════════════
        bro.get("/report", [](Request &request, Response &response, ApplicationLevelContainer &alc) {
            MYSQL *conn = getConn();
            int sc = 0, fc = 0;
            string db_status = "Connected / Active";

            if (!conn) { 
                db_status = "Disconnected / Error"; 
            } else {
                if (mysql_query(conn, "SELECT COUNT(*) FROM students") == 0) {
                    MYSQL_RES *r = mysql_store_result(conn);
                    MYSQL_ROW row = mysql_fetch_row(r);
                    if (row) sc = atoi(row[0]);
                    mysql_free_result(r);
                }
                if (mysql_query(conn, "SELECT COUNT(*) FROM faculty") == 0) {
                    MYSQL_RES *r = mysql_store_result(conn);
                    MYSQL_ROW row = mysql_fetch_row(r);
                    if (row) fc = atoi(row[0]);
                    mysql_free_result(r);
                }
                mysql_close(conn);
            }

            long *vptr = NULL;
            alc.get("total_visits_ptr", &vptr, NULL, NULL);
            if (vptr) (*vptr)++;
            long visits = vptr ? *vptr : 0;

            request.setCHTMLVariable("total_students", sc);
            request.setCHTMLVariable("total_faculty", fc);
            request.setCHTMLVariable("visit_count", (int)visits);
            request.setCHTMLVariable("db_status", db_status);

            request.forwardTo("/report.chtml");
        });

        // ── Start server ───────────────────────────────────
        bro.listen(6060, [](Error &error) {
            if (!error.hasError())
                cout << "\n[OK] EduBase server is running at http://localhost:6060\n"
                     << "     Open your browser and visit http://localhost:6060/index.html\n" << endl;
            else
                cout << "[ERROR] " << error.getError() << endl;
        });

    } catch (string exception) {
        cout << "[EXCEPTION] " << exception << endl;
    }
    return 0;
}
