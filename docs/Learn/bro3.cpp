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
};
class Bro
{
    private:
        string staticResourceFolder;
        map<string,void(*)(Request &,Response &)> urlMappings;
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
                urlMappings.insert(pair<string,void(*)(Request &,Response &)>(url,callBack));
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
                int requestNumber=0;
                int clientSocketDiscriptor;
                while(1)
                {
                    clientSocketDiscriptor=accept(serverSocketDiscriptor,(struct sockaddr *)&clientSocketInformation,&len);
                    requestNumber++;
                    cout<<"Request Number: " << requestNumber << "arrived"<<endl;
                    if(clientSocketDiscriptor<0)
                    {
                        // not yet decided
                    }
                    forward_list<string> requestBufferDS;
                    forward_list<string>::iterator requestBufferDSIterator;
                    requestBufferDSIterator=requestBufferDS.before_begin();
                    int requestBufferDSSize=0;
                    int requestDataCount=0;
                    while(1)
                    {
                        requestLength=recv(clientSocketDiscriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
                        if(requestLength==0) break;
                        requestBuffer[requestLength]='\0';
                        requestBufferDSIterator=requestBufferDS.insert_after(requestBufferDSIterator,string(requestBuffer));
                        requestBufferDSSize++;
                        requestDataCount+=requestLength;
                    }
                    if(requestBufferDSSize>0)
                    {
                        char *requestData=new char[requestDataCount+1];
                        char *p;
                        const char *q;
                        p=requestData;
                        requestBufferDSIterator=requestBufferDS.begin();
                        while(requestBufferDSIterator!=requestBufferDS.end())
                        {
                            q=(*requestBufferDSIterator).c_str();
                            while(*q!='\0')
                            {
                                *p=*q;
                                p++;
                                q++;
                            }
                            ++requestBufferDSIterator;
                        }
                        *p='\0';
                        requestBufferDS.clear();
                        printf("-------------Request Data Begin------------- \n");
                        printf("%s\n",requestData);
                        printf("-------------Request Data Ends--------------\n");
                        // the code to parse the request goes here
                        delete []requestData;   
                    }
                    else
                    {
                        //something if no data was receive
                    }
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