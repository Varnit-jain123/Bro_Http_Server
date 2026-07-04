#include<bits/stdc++.h>
#include<iostream>
#include<map>
#include<forward_list>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#ifdef _WIN32
    #include<windows.h>
#endif
#ifdef linux
    #include<sys/socket.h>
    #include<arpa/inet.h>
#endif


using namespace std;

// Amit [The bro programmer]

#define _forward_(request,url) \
request.forwardTo(url); \
return;

enum __container_operation_failure_reason__
{
    __KEY_EXISTS__,
    __KEY_DOES_NOT_EXIST__,
    __OUT_OF_MEMORY__,
    __VALUE_SIZE_MISMATCH__
};


class Container
{
    typedef struct _bag
    {
        void *ptr;
        int size;
    }Bag;
    map<string,Bag> dataSet;
    public:
    template<class Whatever>
    void set(string keyName,Whatever something,bool *success,__container_operation_failure_reason__ *reason)
    {
        auto iterator=dataSet.find(keyName);
        if(iterator!=dataSet.end()) 
        {
            if(reason) *reason={__KEY_EXISTS__};
            if(success) *success=false;
            return;
        }
        // cout<<sizeof(something)<<endl;
        void *ptr;
        ptr=malloc(sizeof(something));
        if(ptr==NULL)
        {
            if(reason) *reason={__OUT_OF_MEMORY__};
            if(success)*success=false;
            return;
        }
        memcpy(ptr,&something,sizeof(something));
        Bag bag;
        bag.ptr=ptr;
        bag.size=sizeof(something); 
        dataSet.insert(pair<string,Bag>(keyName,bag));
        if(success) *success=true;
    }
    template<class Cool>
    void get(string keyName,Cool anything,bool *success,__container_operation_failure_reason__ *reason)
    {   
        // cout<<sizeof(*anything)<<endl;
        auto iterator=dataSet.find(keyName);
        if(iterator==dataSet.end()) 
        {
            if(reason) *reason={__KEY_DOES_NOT_EXIST__};
            if(success) *success=false;
            return;
        }
        Bag bag;
        bag=iterator->second;
        if(bag.size!=sizeof(*anything))
        {
            if(reason) *reason={__VALUE_SIZE_MISMATCH__};
            if(success) *success=false;
            return;
        }
        memcpy(anything,bag.ptr,sizeof(*anything)); 
        if(success) *success=true;
    }

    template<class Cool>
    void remove(string keyName,Cool anything,bool *success,__container_operation_failure_reason__ *reason)
    {   
        // cout<<sizeof(*anything)<<endl;
        auto iterator=dataSet.find(keyName);
        if(iterator==dataSet.end()) 
        {
            if(reason) *reason={__KEY_DOES_NOT_EXIST__};
            if(success) *success=false;
            return;
        }
        Bag bag;
        bag=iterator->second;
        if(bag.size!=sizeof(*anything))
        {
            if(reason) *reason={__VALUE_SIZE_MISMATCH__};
            if(success) *success=false;
            return;
        }
        memcpy(anything,bag.ptr,sizeof(*anything)); 
        free(bag.ptr); // to release the memory by bro server programmer
        if(success) *success=true;
    }

    bool contains(string keyName)
    {
        auto iterator=this->dataSet.find(keyName);
        return iterator!=this->dataSet.end();
    }
};

class ApplicationLevelContainer:public Container
{

};


class BroUtilities
{
    private:
    BroUtilities(){}
    public:
    static bool isHexChar(int w)
    {
        if(w>=48 && w<=57) return true;
        if(w<='a' && w<='f') return true;
        if(w>='A' && w<='F') return true;
        return false;
    }
    static void decode(char *encodedString,char *decodedString)
    {
        char *ptr=encodedString;
        char *d=decodedString;
        int i,m;
        i=0;
        while(*ptr!='\0')
        {
            if(*ptr=='+')
            {
                d[i]=' ';
                i++;
                ptr++;
                continue;
            }
            if(*ptr!='%')
            {
                d[i]=*ptr;
                i++;
                ptr++;
                continue;
            }
            ptr++;
            if(isHexChar(*ptr) && isHexChar(*(ptr+1)))
            {
                sscanf(ptr,"%2x",&m);
                d[i]=m;
                i++;
                ptr+=2;
            }
            else
            {
                i=0;
                break;
            }
        }
        d[i]='\0';
    }
    static void loadMIMETypes(map<string,string> &mimeTypesMap)
    {
        FILE *file;
        file=fopen("bro-data/mime.types","r");
        if(file==NULL) return;
        char *mimeType;
        char *extension;
        char line[200];
        int x;
        while(1)
        {
            fgets(line,200,file);
            if(feof(file)) break;
            if(line[0]=='#') continue;
            x=strlen(line)-1;
            while(1) // logic to remove '\r' and '\n' from end of string
            {
                if(line[x]=='\r' || line[x]=='\n') 
                {
                    line[x]='\0';
                    x--;
                }
                else 
                {
                    break;
                }
            }
            mimeType=&line[0];
            for(x=0;line[x]!='\t';x++);
            line[x]='\0';
            x++;
            while(line[x]=='\t') x++;
            while(1)
            {
                extension=&line[x];
                while(line[x]!='\0' && line[x]!=' ') x++;
                if(line[x]=='\0')
                {
                    // add entry to map and break the loop
                    mimeTypesMap.insert(pair<string,string>(string(extension),string(mimeType)));
                    //cout<<extension<<"   ,   "<<mimeType<<endl;
                    break;
                }
                else
                {
                    // place \0 on xth index, add entry to the map and increment the value of x
                    line[x]='\0';
                    mimeTypesMap.insert(pair<string,string>(string(extension),string(mimeType)));
                    //cout<<extension<<"   ,   "<<mimeType<<endl;
                    x++;
                }
            }
        }
        fclose(file);
    }
};

class FileSystemUtility
{
    private:
    FileSystemUtility(){}
    public:
    static bool fileExists(const char *path)
    {
        struct stat s;
        int x;
        x=stat(path,&s);
        if(x!=0) return false;
        if(s.st_mode & S_IFDIR) return false;
        return true;
    }
    static bool directoryExists(const char *path)
    {
        struct stat s;
        int x;
        x=stat(path,&s);
        if(x!=0) return false;
        if(s.st_mode & S_IFDIR) return true;
        return false;
    }
    static string getFileExtension(const char *path)
    {
        int x;
        x=strlen(path)-1;
        while(x>=0 && path[x]!='.') x--;
        if(x==-1 || path[x]!='.')  return string("");
        return string(path+(x+1));
    }
};

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
            return FileSystemUtility::directoryExists(path.c_str());
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
    map<string,string> dataMap;
    string _forwardTo;
    char *method;
    char *requestURI;
    char *httpVersion;
    Request(char *method,char *requestURI,char *httpVersion,char *dataInRequest)
    {
        this->method=method;
        this->requestURI=requestURI;
        this->httpVersion=httpVersion;
        if(dataInRequest!=NULL && strcmp(this->method,"get")==0)
        {
            createDataMap(dataInRequest,dataMap);
        }
    }
    string forwardToWhichResource()
    {
        return this->_forwardTo;
    }
    bool isToBeForwarded()
    {
        return this->_forwardTo.length()>0;
    }

    void createDataMap(char *str,map<string,string> &dataMap)
    {
        char *ptr1,*ptr2;
        char *decoded;
        int keyLength,valueLength;
        ptr1=str;
        ptr2=str;
        while (1)
        {
            while(*ptr2!='\0' && *ptr2!='=') ptr2++;
            if(*ptr2=='\0') return;
            *ptr2='\0';
            keyLength=ptr2-ptr1;
            decoded=new char[keyLength+1];
            BroUtilities::decode(ptr1,decoded);
            string key=string(decoded);
            delete [] decoded;
            ptr1=ptr2+1;
            ptr2=ptr1;
            while(*ptr2!='\0' && *ptr2!='&') ptr2++;
            if(*ptr2=='\0')
            {
                valueLength=ptr2-ptr1;
                decoded=new char[valueLength+1];
                BroUtilities::decode(ptr1,decoded);
                dataMap.insert(pair<string,string>(key,string(decoded)));
                delete []decoded;
                break;
            }
            else
            {
                *ptr2='\0';
                valueLength=ptr2-ptr1;
                decoded=new char[valueLength+1];
                BroUtilities::decode(ptr1,decoded);
                dataMap.insert(pair<string,string>(key,string(decoded)));
                delete []decoded;
                ptr1=ptr2+1;
                ptr2=ptr1;
            }
        }
        
    }

    public:
    void forwardTo(string _forwardTo)
    {
        this->_forwardTo=_forwardTo;
    }
    string operator[](string key)
    {
        auto iterator=dataMap.find(key);
        if(iterator==dataMap.end())
        {
            return string("");
        }
        return iterator->second;

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

class Function
{
    public:
    virtual void doService(Request &,Response &)=0;
};

typedef struct __url__mapping
{
    __request_method__ requestMethod;
    Function *function;
    // void ( *mappedFunction)(Request &,Response &);
}URLMapping;

class SimpleFunction:public Function
{
    private:
        void (*mappedFunction)(Request &,Response &);
    public:
        SimpleFunction(void (*mappedFunction)(Request &,Response &))
        {
            this->mappedFunction=mappedFunction;
        }
        void doService(Request &request,Response &response)
        {
            this->mappedFunction(request,response);
        }
};

class ApplicationLevelContainerDependentFunction:public Function
{
    private:
        void (*mappedFunction)(Request &,Response &,ApplicationLevelContainer &);
        ApplicationLevelContainer *p2ApplicationLevelContainer;
    public:
        ApplicationLevelContainerDependentFunction(void(*mappedFunction)(Request &,Response &,ApplicationLevelContainer &),ApplicationLevelContainer *p2ApplicationLevelContainer)
        {
            this->mappedFunction=mappedFunction;
            this->p2ApplicationLevelContainer=p2ApplicationLevelContainer;
        }
        void doService(Request &request,Response &response)
        {
            this->mappedFunction(request,response,*p2ApplicationLevelContainer);
        }
};

class Bro
{
    private:
        string staticResourceFolder;
        map<string,URLMapping> urlMappings;
        map<string,string> mimeTypes;
        ApplicationLevelContainer applicationLevelContainer;
    public:
        Bro()
        {
            BroUtilities::loadMIMETypes(mimeTypes);
            if(mimeTypes.size()==0) throw string("bro-data folder has been tampered with");
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
                string exception=string("Invalid static resource folder path : ")+staticResourceFolder;
                throw exception;
            }
        }

        bool serverStaticResource(int clientSocketDescriptor,const char*requestURI)
        {
            if(this->staticResourceFolder.length()==0) return false;
            if(!FileSystemUtility::directoryExists(this->staticResourceFolder.c_str())) return false;
            string resourcePath=this->staticResourceFolder+string(requestURI);
            cout<<"Static resource path is : "<<resourcePath<<endl;
            if(!FileSystemUtility::fileExists(resourcePath.c_str())) return false;
            FILE *file=fopen(resourcePath.c_str(),"rb");
            if(file==NULL) return false;
            long fileSize;
            fseek(file,0,SEEK_END);
            fileSize=ftell(file);
            if(fileSize==0)
            {
                fclose(file);
                return false;
            }
            rewind(file); // to move the internal file pointer to start of the file
            string extension,mimeType;
            extension=FileSystemUtility::getFileExtension(resourcePath.c_str());
            if(extension.length()>0)
            {
                transform(extension.begin(),extension.end(),extension.begin(),::tolower);
                auto mimeTypeIterator=mimeTypes.find(extension);
                if(mimeTypeIterator!=mimeTypes.end()) 
                {
                    mimeType=mimeTypeIterator->second;
                }
                else
                {
                    mimeType=string("text/html");
                }
            }
            else
            {
                mimeType=string("text/html");
            }
            cout<<"Resource : "<<resourcePath<<" , extension : "<<extension<<" , Mime Type : "<<mimeType<<endl;
            char header[200];
            sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n",mimeType.c_str(),fileSize);
            send(clientSocketDescriptor,header,strlen(header),0);
            long bytesLeftToRead;
            int bytesToRead=4096;
            char buffer[4096];
            bytesLeftToRead=fileSize;
            while(bytesLeftToRead>0)
            {
                if(bytesLeftToRead<bytesToRead) bytesToRead=bytesLeftToRead;
                fread(buffer,bytesToRead,1,file);
                if(feof(file)) break; // this won't happen on our case
                send(clientSocketDescriptor,buffer,bytesToRead,0);
                bytesLeftToRead=bytesLeftToRead-bytesToRead;
            }
            fclose(file);
            return true;
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
                Function *function;
                function=new SimpleFunction(callBack);
                urlMappings.insert(pair<string,URLMapping>(url,{__GET__,function}));               
            }
        }
        void get(string url,void (*callBack)(Request &,Response &,ApplicationLevelContainer &))
        {
            if(Validator::isValidURLFormat(url))
            {
                // URLMapping u;
                // u.requestMethod=__GET__;
                // u.mappedFunction=callBack;
                // urlMappings.insert(pair<string,URLMapping>(url,u)); 
                // OLD WAY
                Function *function;
                function=new ApplicationLevelContainerDependentFunction(callBack,&(this->applicationLevelContainer));
                urlMappings.insert(pair<string,URLMapping>(url,{__GET__,function}));               
            }
        }
        
        void post(string url,void (*callBack)(Request &,Response &))
        {
            if(Validator::isValidURLFormat(url))
            {
                // URLMapping u;
                // u.requestMethod=__POST__;
                // u.mappedFunction=callBack;
                // urlMappings.insert(pair<string,URLMapping>(url,u)); 
                // OLD WAY
                Function *function;
                function=new SimpleFunction(callBack);
                urlMappings.insert(pair<string,URLMapping>(url,{__POST__,function}));               
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
                char *method,*requestURI,*httpVersion,*dataInRequest;
                requestBuffer[requestLength]='\0';
                // printf("****************\n\n");
                // printf("%s\n",requestBuffer);
                // printf("\n\n****************\n\n");
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
                i=0;
                while(requestURI[i]!='\0' && requestURI[i]!='?') i++;
                if(requestURI[i]=='?') 
                {
                    requestURI[i]='\0';
                    dataInRequest=requestURI+i+1;
                }
                cout<<"Request Arrived, URI is : "<<requestURI<<endl;
                auto urlMappingIterator=this->urlMappings.find(string(requestURI));
                if(urlMappingIterator==this->urlMappings.end())
                {
                    if(!serverStaticResource(clientSocketDiscriptor,requestURI))
                    {
                        HttpErrorStatusUtility::sendNotFoundError(clientSocketDiscriptor,requestURI);
                    }
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
                Request request(method,requestURI,httpVersion,dataInRequest);
                while(1)
                {
                    Response response;
                    urlMapping.function->doService(request,response);
                    if(!request.isToBeForwarded())
                    {
                        HTTPResponseUtility::sendResponse(clientSocketDiscriptor,response);
                        break;
                    }
                    string forwardTo=request.forwardToWhichResource();
                    urlMappingIterator=this->urlMappings.find(forwardTo);
                    if(urlMappingIterator==this->urlMappings.end())
                    {
                        if(!serverStaticResource(clientSocketDiscriptor,forwardTo.c_str()))
                        {
                            HttpErrorStatusUtility::sendNotFoundError(clientSocketDiscriptor,requestURI);
                        } 
                        break;
                    }
                    urlMapping=urlMappingIterator->second;
                    if(urlMapping.requestMethod==__GET__ && strcmp(method,"get")!=0)
                    {
                        HttpErrorStatusUtility::sendMethodNotAllowedError(clientSocketDiscriptor,method,requestURI);
                        break;
                    }
                    // some more if conditions for other request methods
                    request.forwardTo(string(""));
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
    try
    {
        Bro bro;
        bro.setStaticResourceFolder("Static");

        // bro.get("/getCustomers", [](Request &request, Response &response) {
        //     const char *html = R""""(
        //     <!DOCTYPE html>
        //     <html>
        //     <body>
        //         <h1>Welcome Administrator</h1>
        //         <a href="getCustomers">Customers list</a>
        //     </body>
        //     </html>
        //     )"""";
        //     response.setContentType("text/html");
        //     response << html;
        // });

        // bro.get("/getCustomers", [](Request &request, Response &response) {
        //     const char *html = R""""(
        //     <!DOCTYPE html>
        //     <html>
        //     <body>
        //         <h1>List of Customers</h1>
        //         <ul>
        //             <li>Amit</li>
        //             <li>Bobby</li>
        //         </ul>
        //         <br>
        //         <a href="/">Home</a>
        //     </body>
        //     </html>
        //     )"""";
        //     response.setContentType("text/html");
        //     response << html;
        // });


        bro.get("/save_test1_data", [](Request &request, Response &response) {
            string nnn=request["nm"];
            string ccc=request["ct"];
            cout<<"Data that arrived in request"<<endl;
            cout<<nnn<<endl;
            cout<<ccc<<endl;
            cout<<"------------------------"<<endl;
            const char *html = R""""(
            <!DOCTYPE html>
            <html>
            <head>
            <meta charset='utf-8'>
            <title>Bro test cases</title>
            </head>
            <body>
                <h1>Test case 1 - GET with query string</h1>
                <h3>Response from server side</h3>
                <b>Data Saved</b>
                <br/><br/>
                <a href="/index.html">Home</a>
            </body>
            </html>
            )"""";
            response.setContentType("text/html");
            response << html;
        });

        bro.post("/save_test2_data", [](Request &request, Response &response) {
            const char *html = R""""(
            <!DOCTYPE html>
            <html>
            <head>
            <meta charset='utf-8'>
            <title>Bro test cases</title>
            </head>
            <body>
                <h1>Test case 2 - POST with form data</h1>
                <h3>Response from server side</h3>
                <b>Data Saved</b>
                <br/><br/>
                <a href="/index.html>Home</a>
            </body>
            </html>
            )"""";
            response.setContentType("text/html");
            response << html;
        });
            
        bro.get("/firstCartoonFilm", [](Request &request, Response &response,ApplicationLevelContainer &cc) {
            string *str;
            str=new string("The jungle book");
            cc.set("firstFilm",str,NULL,NULL);
            const char *html = R""""(
            <!DOCTYPE html>
            <html>
            <head>
            <meta charset='utf-8'>
            <title>Bro test cases</title>
            </head>
            <body>
                <h1>First cartoon film</h1>
                <h3>The jungle book</h3>
                <br><br>
                <a href="/secondCartoonFilm">Watch the next film</a>
            </body>
            </html>
            )"""";
            response.setContentType("text/html");
            response << html;
        });

        bro.get("/secondCartoonFilm", [](Request &request, Response &response,ApplicationLevelContainer &cc) {
            string *s;
            cc.get("firstFilm",&s,NULL,NULL);
            response.setContentType("text/html");
            const char *html1 = R""""(
                <!DOCTYPE html>
                <html>
                <head>
                <meta charset='utf-8'>
                <title>Bro test cases</title>
                </head>
                <body>
                <h1>First film was 
                )"""";
            response<<html1;
            response<<*s;
            const char *html2=R""""(
                    </h1>
                    <br><br>
                    <h1>Second cartoon film</h1>
                    <h3>The Toy Story</h3>
                    <br><br>
                    <br/><br/>
                </body>
                </html>
                )"""";
            response<<html2;
        });
        
        /*
            Bobby wants to specify the function(s) that shoud be called ,
            when server starts
            the function may require (ApplicationLevelContainer)
            
            Bobby should also be able to specify the priority level of the startup function
            priority will be decided the sequence of execution of the function specified by bobby
            weighted priority level 
            100 b
            1000 a
            5 c
        */

        bro.get("/coolOne", [](Request &request, Response &response) {
            cout<<"some processing is done over here by function mapped to /coolOne"<<endl;
            _forward_(request,string("/coolTwo"));
        });

        bro.get("/coolTwo", [](Request &request, Response &response) {
            cout<<"some processing is done over here by function mapped to /coolTwo"<<endl;
            _forward_(request,string("/coolThree"));
            cout<<"Some lines that should not executed as request been forwarded"<<endl;
        });

        bro.get("/coolThree", [](Request &request, Response &response) {
            cout<<"some processing is done over here by function mapped to /coolThree"<<endl;
            _forward_(request,string("/somethingCool.html"));
        }); 

        



        bro.listen(6060, [](Error &error) {
            if (!error.hasError())
                cout << "Bro HTTP Server is listening on port 6060\n";
            else
                cout << "Error occurred: " << error.getError() << endl;
        });
    }catch(string exception)
    {
        cout<<exception<<endl;
    }
    return 0;
}