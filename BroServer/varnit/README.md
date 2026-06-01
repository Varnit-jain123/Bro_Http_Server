# EduBase — Student & Faculty Management System

A full-stack web application built on **Bro HTTP Server + MySQL**, featuring a modern dark UI.

## Project Structure

```
varnit/
├── varnit.cpp          ← Main server (edit DB credentials here)
├── db_setup.sql        ← Run once to create database + tables
├── bro-data/
│   └── mime.types      ← Required for static file serving
└── static/
    ├── index.html      ← Home page with live stats
    ├── students.html   ← Student records (add/view/delete)
    ├── faculty.html    ← Faculty records (add/view/delete)
    └── style.css       ← Global dark theme stylesheet
```

## Prerequisites

- MySQL server installed and running
- MySQL C connector (`libmysqlclient-dev` on Linux, or MySQL Connector/C on Windows)
- Bro HTTP Server library already built (`../lib/libbro.a`)

## Setup Steps

### 1. Configure your MySQL credentials

Open `varnit.cpp` and edit the top of the file:
```cpp
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS ""          // ← put your MySQL password here
#define DB_NAME "edubase"
```

### 2. Create the database

```bash
mysql -u root -p < db_setup.sql
```

### 3. Compile

**Linux:**
```bash
g++ varnit.cpp -o varnit.out -I ../include/ -L ../lib/ -lbro -lmysqlclient -lpthread
```

**Windows (MinGW):**
```bash
g++ varnit.cpp -o varnit.exe -I ../include/ -L ../lib/ -lbro -lmysql -lws2_32 -static-libgcc -static-libstdc++
```
> Note: You need the MySQL Connector/C for Windows. Download from https://dev.mysql.com/downloads/connector/c/

### 4. Run

```bash
# Linux
./varnit.out

# Windows
.\varnit.exe
```

Open your browser: **http://localhost:6060/index.html**

## API Endpoints

| Endpoint | Description |
|----------|-------------|
| `GET /api/stats` | Returns count of students and faculty |
| `GET /api/students` | List all students as JSON |
| `GET /api/add_student?name=&roll=&...` | Add a student |
| `GET /api/delete_student?id=N` | Delete student by ID |
| `GET /api/faculty` | List all faculty as JSON |
| `GET /api/add_faculty?name=&emp_id=&...` | Add a faculty member |
| `GET /api/delete_faculty?id=N` | Delete faculty by ID |
