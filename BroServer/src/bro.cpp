#include<bro.h>
#include<bro_validator.h>
#include<bro_file_system_utility.h>
#include<bro_template_engine.h>
#include<bro_function.h>
#include<bro_simple_function.h>
#include<bro_simple_startup_function.h>
#include<bro_application_level_Container_Dependent_Function.h>
#include<bro_application_level_Container_Dependent_Startup_Function.h>
#include<bro_http_error_status_utility.h>
#include<bro_header_utility.h>
#include<bro_string_utility.h>
#include<bro_http_response_utility.h>

void request_processor(int clientSocketDiscriptor,Bro *bro,BroThreadWrapperNode *broThreadWrapperNode)
{
    char requestBuffer[4097]; // one extra byte for null termination
    int requestLength;
    int x;
    requestLength=recv(clientSocketDiscriptor,requestBuffer,sizeof(requestBuffer)-1,0); 
    if(requestLength==0 || requestLength==-1)
    {
        close(clientSocketDiscriptor);
        broThreadWrapperNode->setCompletedStatus(true);
        return;
    }
    char *method,*requestURI,*httpVersion,*dataInRequest;
    requestBuffer[requestLength]='\0';
    cout<<requestBuffer<<endl;
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
        broThreadWrapperNode->setCompletedStatus(true);
        return;
    }
    requestBuffer[i]='\0';
    i++;
    if(requestBuffer[i]=='\0' || requestBuffer[i]==' ')
    {
        HttpErrorStatusUtility::sendBadRequestError(clientSocketDiscriptor);
        close(clientSocketDiscriptor);
        broThreadWrapperNode->setCompletedStatus(true);
        return;
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
        broThreadWrapperNode->setCompletedStatus(true);
        return;
    }
    requestURI=requestBuffer+i;
    while(requestBuffer[i]!=' ' && requestBuffer[i]) i++;
    if(requestBuffer[i]=='\0')
    {
        HttpErrorStatusUtility::sendBadRequestError(clientSocketDiscriptor);
        close(clientSocketDiscriptor);
        broThreadWrapperNode->setCompletedStatus(true);
        return;
    }
    requestBuffer[i]='\0';
    i++;
    if(requestBuffer[i]=='\0' || requestBuffer[i]==' ')
    {
        HttpErrorStatusUtility::sendBadRequestError(clientSocketDiscriptor);
        close(clientSocketDiscriptor);
        broThreadWrapperNode->setCompletedStatus(true);
        return;
    }
    httpVersion=requestBuffer+i;
    while(requestBuffer[i]!='\r' && requestBuffer[i]!='\n' && requestBuffer[i]) i++;
    if(requestBuffer[i]=='\0')
    {
        HttpErrorStatusUtility::sendBadRequestError(clientSocketDiscriptor); 
        close(clientSocketDiscriptor);
        broThreadWrapperNode->setCompletedStatus(true);
        return;
    }
    if(requestBuffer[i]=='\r' && requestBuffer[i+1]!='\n') 
    {
        HttpErrorStatusUtility::sendBadRequestError(clientSocketDiscriptor); 
        close(clientSocketDiscriptor);
        broThreadWrapperNode->setCompletedStatus(true);
        return;
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
        broThreadWrapperNode->setCompletedStatus(true);
        return;
    }
    // code to parse the first line of the http request ends here
    int headerStartIndex=i;
    i=0;
    dataInRequest=NULL;
    while(requestURI[i]!='\0' && requestURI[i]!='?') i++;
    if(requestURI[i]=='?') 
    {
        requestURI[i]='\0';
        dataInRequest=requestURI+i+1;
    }
    cout<<"Request Arrived, URI is : "<<requestURI<<endl;
    auto urlMappingIterator=bro->urlMappings.find(string(requestURI));
    if(urlMappingIterator==bro->urlMappings.end())
    {
        if(bro->isCHTML(requestURI))
        {
            map<string,string> headerFieldsMap;
            HeaderUtility::parseHeader(requestBuffer+headerStartIndex,headerFieldsMap);
            Request request(method,requestURI,httpVersion,dataInRequest,headerFieldsMap);
            bro->processCHTMLResource(clientSocketDiscriptor,requestURI,request);
        }
        else if(!bro->serveStaticResource(clientSocketDiscriptor,requestURI))
        {
            HttpErrorStatusUtility::sendNotFoundError(clientSocketDiscriptor,requestURI);
        }
        close(clientSocketDiscriptor);
        broThreadWrapperNode->setCompletedStatus(true);
        return;
    }
    URLMapping urlMapping=urlMappingIterator->second;
    if(urlMapping.requestMethod==__GET__ && strcmp(method,"get")!=0)
    {
        HttpErrorStatusUtility::sendMethodNotAllowedError(clientSocketDiscriptor,method,requestURI);
        close(clientSocketDiscriptor);
        broThreadWrapperNode->setCompletedStatus(true);
        return;
    }
    // code to parse the header and then the payload if exists start here
    map<string,string> headerFieldsMap;
    HeaderUtility::parseHeader(requestBuffer+headerStartIndex,headerFieldsMap);
    // code to parse the header and then the payload if exists ends here
    Request request(method,requestURI,httpVersion,dataInRequest,headerFieldsMap);
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
        urlMappingIterator=bro->urlMappings.find(forwardTo);
        if(urlMappingIterator==bro->urlMappings.end())
        {
            if(bro->isCHTML(forwardTo.c_str()))
            {
                request.forwardTo(string(""));
                bro->processCHTMLResource(clientSocketDiscriptor,forwardTo.c_str(),request);
            }
            else if(!bro->serveStaticResource(clientSocketDiscriptor,forwardTo.c_str()))
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
    broThreadWrapperNode->setCompletedStatus(true);
    // lots of code to be added here for complete functionality
}
    
//__________________________________________________________________//

bool Bro::isCHTML(const char *requestURI)
{
    int len=strlen(requestURI);
    if(len<7) return false;
    const char *ptr1=requestURI+len-6;
    char lastPart[7]={".chtml"};
    const char *ptr2=lastPart;
    char a,b;
    while(*ptr1 && *ptr2)
    {
        a=*ptr1;
        b=*ptr2;
        if(a>=97 && a<=122) a-=32;
        if(b>=97 && b<=122) b-=32;
        if(a!=b) return false;
        ptr1++;
        ptr2++;
    }
    return true;
}

Bro::Bro()
{
    BroUtilities::loadMIMETypes(mimeTypes);
    if(mimeTypes.size()==0) throw string("bro-data folder has been tampered with");
}
Bro::~Bro()
{

}
void Bro::setStaticResourceFolder(string staticResourceFolder)
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

void Bro::processCHTMLResource(int clientSocketDiscriptor,const char * requestURI,Request &request)
{
    if(this->staticResourceFolder.length()==0) 
    {
        // send back 404;
        return;
    }
    if(!FileSystemUtility::directoryExists(this->staticResourceFolder.c_str())) 
    {
        // send back 404
        return;
    }
    string resourcePath=this->staticResourceFolder+string(requestURI);
    if(!FileSystemUtility::fileExists(resourcePath.c_str())) 
    {
        // send back 404
        return;
    }
    TemplateEngine::processCHTMLFile(resourcePath.c_str(),request,clientSocketDiscriptor);

}
bool Bro::serveStaticResource(int clientSocketDescriptor,const char*requestURI)
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

void Bro::addStartupService(int priorityNumber,void (*startupFunction)(void))
{
    StartupFunction *sf;
    sf=new SimpleStartupFunction(priorityNumber,startupFunction);
    this->startupFunctions.push(sf);
}
void Bro::addStartupService(int priorityNumber,void (*startupFunction)(ApplicationLevelContainer &applicationLevelContainer))
{
    StartupFunction *sf;
    sf=new ApplicationLevelContainerDependentStartupFunction(priorityNumber,startupFunction,&(this->applicationLevelContainer));
    this->startupFunctions.push(sf);
}

void Bro::get(string url,void (*callBack)(Request &,Response &))
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
void Bro::get(string url,void (*callBack)(Request &,Response &,ApplicationLevelContainer &))
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

void Bro::post(string url,void (*callBack)(Request &,Response &))
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
void Bro::listen(int portNumber,void (*callBack)(Error &))
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
    BroThreadWrapperNode *top,*p1,*p2;
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
    top=NULL;
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
    // code to call all startup services starts here
    StartupFunction *startupFunction;
    while(!startupFunctions.empty())
    {
        startupFunction=startupFunctions.top();
        startupFunctions.pop(); 
        startupFunction->run();
    }
    // code to call all startup services ends here
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
    thread *t;
    while(1)
    {
        cout<<"Scanning of data structure starts here"<<endl;
        p1=top;
        while(p1!=NULL)
        {
            if(p1->isComplete())
            {
                p1->th->join();
                if(p1==top)
                {
                    top=top->next;
                    delete p1->th;
                    delete p1;
                    p1=top;
                    continue;
                }
                else
                {
                    p2->next=p1->next;
                    delete p1->th;
                    delete p1;
                    p1=p2->next;
                    continue;
                }
            }
            p2=p1;
            p1=p1->next;
        }
        cout<<"Scanning of data structure ends here"<<endl;
        clientSocketDiscriptor=accept(serverSocketDiscriptor,(struct sockaddr *)&clientSocketInformation,&len);
        p1=new BroThreadWrapperNode;
        t=new thread(request_processor,clientSocketDiscriptor,this,p1);
        p1->th=t;
        p1->next=top;
        top=p1;
        // lots of code to be added here for complete functionality
    } // infinite loops ends

    // when the server shuts down ,the control reaches this line
    while(top!=NULL)
    {
        p1=top;
        while(p1!=NULL)
        {
            if(p1->isComplete())
            {
                p1->th->join();
                if(p1==top)
                {
                    top=top->next;
                    delete p1->th;
                    delete p1;
                    p1=top;
                    continue;
                }
                else
                {
                    p2->next=p1->next;
                    delete p1->th;
                    delete p1;
                    p1=p2->next;
                    continue;
                }
            }
            p2=p1;
            p1=p1->next;
        }
    }
    cout<<"The waits ends and the story ends here"<<endl;
    #ifdef _WIN32
        WSACleanup();
    #endif
}
