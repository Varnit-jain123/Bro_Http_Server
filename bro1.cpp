#include<iostream>
using namespace std;

// Amit [The bro programmer]
class Error
{
public:
	bool hasError()
	{
		return false;
	}
	string getError()
	{
		return "";
	}
};
class Request
{
	// request related data and methods
};
class Response
{
public:
	void setContentType(string contentType)
	{
		// set content type
	}
	Response& operator<<(string content)
	{
		// append data to response body
		return *this;
	}
};
class Bro
{
public:
	void setStaticResourceFolder(string staticResourceFolder)
	{

	}
	void get(string urlPattern,void (*callBack)(Request &,Response &))
	{
		// register GET route and its handler
	}
	void listen(int portNumber,void (*callBack)(Error &))
	{
		// start listening on the specified port
	}
};



// Bobby [The Web application developer - User of bro web server]
int main()
{
    Bro bro;
    bro.setStaticResourceFolder("Whatever");

    bro.get("/", [](Request &request, Response &response) {
        const char *html = R"(
        <!DOCTYPE html>
        <html>
        <body>
            <h1>Welcome Administrator</h1>
            <a href="getCustomers">Customers list</a>
        </body>
        </html>
        )";
        response.setContentType("text/html");
        response << html;
    });

    bro.get("/getCustomers", [](Request &request, Response &response) {
        const char *html = R"(
        <!DOCTYPE html>
        <html>
        <body>
            <h1>List of Customers</h1>
            <ul>
                <li>Amit</li>
                <li>Bobby</li>
            </ul>
        </body>
        </html>
        )";
        response.setContentType("text/html");
        response << html;
    });

    bro.listen(6060, [](Error &error) {
        if (!error.hasError())
            cout << "Bro HTTP Server is listening on port 6060\n";
        else
            cout << "Error occurred: " << error.getError() << endl;
    });

    return 0;
}
