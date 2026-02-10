#include<iostream>
#include<map>
#include<forward_list>
#include<string.h>
#include<unistd.h>
#ifdef _WIN32
    #include<windows.h>
#endif
#ifdef linux
    #include<sys/socket.h>
    #include<arpa/inet.h>
#endif


using namespace std;

// Amit [The bro programmer]

class StringUtility
{
    private:
    StringUtility(){}
    public:
        static void toLowerCase(char *str)
        {
            if(str==NULL) return;
            int i=0;
            while(str[i]!='\0')
            {
                if(str[i]>='A' && str[i]<='Z')    str[i]=str[i]+32;
                i++;
            }
        }
};

class HttpErrorStatusUtility
{
    private:
    HttpErrorStatusUtility(){}
    public:
        static void sendBadRequestError(int clientSocketDiscriptor)
        {
            // will be implemented later
        }
        static void sendHttpVersionNotSupportedError(int clientSocketDiscriptor,char *httpVersion)
        {
            // will be implemented later
        }
        static void sendNotFoundError(int clientSocketDiscriptor,char *requestURI)
        {
            // will be implemented later
            char content[1000];
            char header[200];
            char response[2000];
            sprintf(content,"<!DOCTYPE html> <html lang='en'> <head> <meta charset='UTF-8'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <title>404 Error</title> </head> <body> Requested Resource [%s] Not found </body> ",requestURI);
            int contentLength=strlen(content);
            sprintf(header,"HTTP/1.1 404 Not Found\r\nContent-Type:text/html\nContent-Length:%d\nConnection: close\r\n\r\n",contentLength);
            strcpy(response,header);
            strcat(response,content);
            send(clientSocketDiscriptor,response,strlen(response),0);
        }
        static void sendMethodNotAllowedError(int clientSocketDiscriptor,const char *method,char *requestURI)
        {
            // will be implemented later        
        }
};

class Validator
{
    private:
    Validator(){}
    public:
        static bool isValidPath(string &path)
        {
            // basic validation for path
            return true;
        }
        static bool isValidMIMEType(string &mimeType)
        {
            // basic validation for MIME type
            return true;
        }
        static bool isValidURLFormat(string &url)
        {
            // basic validation for URL pattern
            return true;
        }
};
class Error
{
    private:
    string error;
    public:
        Error(string error)
        {
            this->error=error;
        }
        bool hasError()
        {
            return this->error.length()>0;
        }
        string getError()
        {
            return this->error;
        }
};
class Request
{
    private:
    char *method;
    char *requestURI;
    char *httpVersion;
    Request(char *method,char *requestURI,char *httpVersion)
    {
        this->method=method;
        this->requestURI=requestURI;
        this->httpVersion=httpVersion;
    }
    friend class Bro;
	// request related data and methods
};
class Response
{
    private :
        string contentType;
        forward_list<string> content;
        forward_list<string>::iterator contentIterator;
        unsigned long contentLength;
    public:
        Response()
        {
            this->contentLength=0;
            this->contentIterator=this->content.before_begin();
        }
        ~Response()
        {
            // not yet declared
        }
        void setContentType(string contentType)
        {
            if(Validator::isValidMIMEType(contentType))
            {
                this->contentType=contentType;         
            }
        }
        Response& operator<<(string content)
        {
            this->contentLength+=content.length();
            this->contentIterator=this->content.insert_after(this->contentIterator,content);
            return *this;
        }
        friend class HTTPResponseUtility;
};

class HTTPResponseUtility
{
    private:
    HTTPResponseUtility(){}
    public:
    static void sendResponse(int clientSocketDescriptor,Response &response)
    {
    char header[200];
    int contentLength=response.contentLength;
    sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type:text/html\nContent-Length:%d\nConnection: close\r\n\r\n",contentLength);
    send(clientSocketDescriptor,header,strlen(header),0);
    auto contentIterator=response.content.begin();
    while(contentIterator!=response.content.end())
    {
        string str=*contentIterator;
        send(clientSocketDescriptor,str.c_str(),str.length(),0);
        ++contentIterator;        
    }
    }
};

enum __request_method__
{
    __GET__,
    __POST__,
    __PUT__,
    __DELETE__,
    __CONNECT__,
    __OPTIONS__,
    __TRACE__,
    __HEAD__
};

typedef struct __url__mapping
{
    __request_method__ requestMethod;
    void ( *mappedFunction)(Request &,Response &);
}URLMapping;


class Bro
{
    private:
        string staticResourceFolder;
        map<string,URLMapping> urlMappings;
    public:
        Bro()
        {

        }
        ~Bro()
        {

        }
        void setStaticResourceFolder(string staticResourceFolder)
        {
            if(Validator::isValidPath(staticResourceFolder))
            {
                this->staticResourceFolder=staticResourceFolder;
            }
            else
            {
                // not yet declared
            }
        }
        void get(string url,void (*callBack)(Request &,Response &))
        {
            if(Validator::isValidURLFormat(url))
            {
                // URLMapping u;
                // u.requestMethod=__GET__;
                // u.mappedFunction=callBack;
                // urlMappings.insert(pair<string,URLMapping>(url,u)); 
                // OLD WAY
                urlMappings.insert(pair<string,URLMapping>(url,{__GET__,callBack}));               
            }
        }
            void listen(int portNumber,void (*callBack)(Error &))
            {
                #ifdef _WIN32
                    WSADATA wsaData;
                    WORD ver;
                    ver=MAKEWORD(1,1);
                    WSAStartup(ver,&wsaData);
                #endif
                int serverSocketDiscriptor;
                char requestBuffer[4097]; // one extra byte for null termination
                int requestLength;
                int x;
                serverSocketDiscriptor=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
                if(serverSocketDiscriptor<0)
                {
                    #ifdef _WIN32
                        WSACleanup();
                    #endif
                    Error error("Socket creation failed");
                    callBack(error);
                    return;
                }
                struct sockaddr_in serversocketInformation;   
                serversocketInformation.sin_family=AF_INET;
                serversocketInformation.sin_port=htons(portNumber);
                serversocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
                int successCode=bind(serverSocketDiscriptor,(struct sockaddr *)&serversocketInformation,sizeof(serversocketInformation));
                if(successCode<0)
                {
                    close(serverSocketDiscriptor);
                    #ifdef _WIN32
                        WSACleanup();
                    #endif
                    char a[101];
                    sprintf(a,"unable to Bind to port : %d failed",portNumber);
                    Error error(a);
                    callBack(error);
                    return;
                }
                successCode=::listen(serverSocketDiscriptor,10);
                if(successCode<0)
                {
                    close(serverSocketDiscriptor);
                    #ifdef _WIN32
                        WSACleanup();
                    #endif
                    Error error("Unable to accepts client connections");
                    callBack(error);
                    return;
                }
                Error error("");
                callBack(error);
                struct sockaddr_in clientSocketInformation;
                #ifdef linux                
                    socklen_t len=sizeof(clientSocketInformation);
                #endif
                #ifdef _WIN32
                    int len=sizeof(clientSocketInformation);
                #endif
                int clientSocketDiscriptor;
                while(1)
                {
                    clientSocketDiscriptor=accept(serverSocketDiscriptor,(struct sockaddr *)&clientSocketInformation,&len);
                    requestLength=recv(clientSocketDiscriptor,requestBuffer,sizeof(requestBuffer)-1,0); 
                    if(requestLength==0 || requestLength==-1)
                    {
                        close(clientSocketDiscriptor);
                        continue;
                    }
                    char *method,*requestURI,*httpVersion;
                    requestBuffer[requestLength]='\0';
                    // code to parse the first line of the http request starts here
                    // first line format : REQUEST_METHOD SPACE URI SPACE HTTP_VERSION CRLF
                    method=requestBuffer;
                    int i=0;
                    while(requestBuffer[i]!=' ' && requestBuffer[i]) i++;  
                    if(requestBuffer[i]=='\0')
                    {
                        HttpErrorStatusUtility::sendBadRequestError(clientSocketDiscriptor);
                        close(clientSocketDiscriptor);
                        continue;
                    }
                    requestBuffer[i]='\0';
                    i++;
                    if(requestBuffer[i]=='\0' || requestBuffer[i]==' ')
                    {
                        HttpErrorStatusUtility::sendBadRequestError(clientSocketDiscriptor);
                        close(clientSocketDiscriptor);
                        continue;
                    }
                    StringUtility::toLowerCase(method);
                    if(!(strcmp(method,"get")==0 ||
                    strcmp(method,"post")==0 ||
                    strcmp(method,"put")==0 ||
                    strcmp(method,"delete")==0 ||
                    strcmp(method,"connect")==0 ||
                    strcmp(method,"options")==0 ||
                    strcmp(method,"trace")==0 ||
                    strcmp(method,"head")==0))
                    {
                        HttpErrorStatusUtility::sendBadRequestError(clientSocketDiscriptor);
                        close(clientSocketDiscriptor);
                        continue;
                    }
                    requestURI=requestBuffer+i;
                    while(requestBuffer[i]!=' ' && requestBuffer[i]) i++;
                    if(requestBuffer[i]=='\0')
                    {
                        HttpErrorStatusUtility::sendBadRequestError(clientSocketDiscriptor);
                        close(clientSocketDiscriptor);
                        continue;
                    }
                    requestBuffer[i]='\0';
                    i++;
                    if(requestBuffer[i]=='\0' || requestBuffer[i]==' ')
                    {
                        HttpErrorStatusUtility::sendBadRequestError(clientSocketDiscriptor);
                        close(clientSocketDiscriptor);
                        continue;
                    }
                    httpVersion=requestBuffer+i;
                    while(requestBuffer[i]!='\r' && requestBuffer[i]!='\n' && requestBuffer[i]) i++;
                    if(requestBuffer[i]=='\0')
                    {
                        HttpErrorStatusUtility::sendBadRequestError(clientSocketDiscriptor); 
                        close(clientSocketDiscriptor);
                        continue;
                    }
                    if(requestBuffer[i]=='\r' && requestBuffer[i+1]!='\n') 
                    {
                        HttpErrorStatusUtility::sendBadRequestError(clientSocketDiscriptor); 
                        close(clientSocketDiscriptor);
                        continue;
                    }
                    if(requestBuffer[i]=='\r') 
                    {
                        requestBuffer[i]='\0';
                        i=i+2;
                    }
                    else
                    {
                        requestBuffer[i]='\0';
                        i=i+1;
                    }
                    StringUtility::toLowerCase(httpVersion);
                    if(strcmp(httpVersion,"http/1.1")!=0)
                    {
                        HttpErrorStatusUtility::sendHttpVersionNotSupportedError(clientSocketDiscriptor,httpVersion);
                        close(clientSocketDiscriptor);
                        continue;
                    }
                    // code to parse the first line of the http request ends here
                    auto urlMappingIterator=this->urlMappings.find(string(requestURI));
                    if(urlMappingIterator==this->urlMappings.end())
                    {
                        HttpErrorStatusUtility::sendNotFoundError(clientSocketDiscriptor,requestURI);
                        close(clientSocketDiscriptor);
                        continue;
                    }
                    URLMapping urlMapping=urlMappingIterator->second;
                    if(urlMapping.requestMethod==__GET__ && strcmp(method,"get")!=0)
                    {
                        HttpErrorStatusUtility::sendMethodNotAllowedError(clientSocketDiscriptor,method,requestURI);
                        close(clientSocketDiscriptor);
                        continue;
                    }
                    // code to parse the header and then the payload if exists start here

                    // code to parse the header and then the payload if exists ends here
                    Request request(method,requestURI,httpVersion);
                    Response response;
                    urlMapping.mappedFunction(request,response);
                    HTTPResponseUtility::sendResponse(clientSocketDiscriptor,response);
                    close(clientSocketDiscriptor);
                    // lots of code to be added here for complete functionality
                } // infinite loops ends
                #ifdef _WIN32
                    WSACleanup();
                #endif
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
            <br>
            <a href="/">Home</a>
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