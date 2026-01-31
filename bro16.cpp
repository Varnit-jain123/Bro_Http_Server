#include<bits/stdc++.h>
#include<iostream>
#include<map>
#include<forward_list>
#include<queue>
#include<vector>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include <filesystem>

#ifdef _WIN32
    const char *PATH_SEPARATOR="\\";
    #include<windows.h>
#endif
#ifdef linux
    const char *PATH_SEPARATOR="/";
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

class Stringifyable
{
    public:
        virtual string stringify()=0;
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
    static bool createDirectory(const char *directoryName)
    {
        return filesystem::create_directory(directoryName);
    }
    static unsigned int getLastUpdatedTime(const char *fileName)
    {
        struct stat attributes;
        stat(fileName,&attributes);
        return attributes.st_mtime;
    }
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
    const char *requestURI;
    char *httpVersion;
    Request(char *method,const char *requestURI,char *httpVersion,char *dataInRequest)
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
    map<string,string> varMap; // this will be changed later on
    public:
    void setCHTMLVariable(string name,string value) // changed later on
    {
        varMap.insert({name,value}); // map.insert(pair<string,string>(name,value)); 
    }
    void setCHTMLVariable(string name,Stringifyable *stringifyable)
    {
        varMap.insert({name,stringifyable->stringify()}); 
    }
    void setCHTMLVariable(string name,const char *ptr)
    {
        varMap.insert({name,string(ptr)}); 
    }
    void setCHTMLVariable(string name,short int value)
    {
        varMap.insert({name,to_string(value)}); 
    }
    void setCHTMLVariable(string name,unsigned short int value)
    {
        varMap.insert({name,to_string(value)}); 
    }
    void setCHTMLVariable(string name,int value)
    {
        varMap.insert({name,to_string(value)}); 
    }
    void setCHTMLVariable(string name,unsigned int value)
    {
        varMap.insert({name,to_string(value)}); 
    }
    void setCHTMLVariable(string name,long value)
    {
        varMap.insert({name,to_string(value)}); 
    }
    void setCHTMLVariable(string name,unsigned long value)
    {
        varMap.insert({name,to_string(value)}); 
    }
    void setCHTMLVariable(string name,long long int value)
    {
        varMap.insert({name,to_string(value)}); 
    }
    void setCHTMLVariable(string name,unsigned long long int value)
    {
        varMap.insert({name,to_string(value)}); 
    }
    void setCHTMLVariable(string name,float value)
    {
        varMap.insert({name,to_string(value)}); 
    }
    void setCHTMLVariable(string name,double value)
    {
        varMap.insert({name,to_string(value)}); 
    }
    void setCHTMlVariable(string name,long double value)
    {
        varMap.insert({name,to_string(value)}); 
    }
    void setCHTMLVariable(string name,char value)
    {
        string str;
        str+=value;
        varMap.insert({name,str}); 
    }
    void setCHTMLVariable(string name,unsigned char value)
    {
        string str;
        str+=value;
        varMap.insert({name,str}); 
    }
    void setCHTMLVariable(string name,bool value)
    {
        varMap.insert({name,(value==true)?"true":"false"}); 
    }
    bool containsCHTMLVariable(string name) // changed later on
    {
        return varMap.find(name)!=varMap.end();
    }
    string getCHTMLVariable(string name) // changed later on
    {
        string data="";
        auto a=varMap.find(name);
        if(a==varMap.end()) return string("");
        return a->second; 
    }
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

class StartupFunction
{
    public:
    virtual void run()=0;
    virtual int getPriorityNumber()=0;
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

class SimpleStartupFunction:public StartupFunction 
{
    private:
        void (*startupFunction)(void);
        int priorityNumber;
    public:
        SimpleStartupFunction(int priorityNumber,void (*startupFunction)(void))
        {
            this->priorityNumber=priorityNumber;
            this->startupFunction=startupFunction;
        }
        int getPriorityNumber()
        {
            return this->priorityNumber;
        }
        void run()
        {
            startupFunction();
        }
};

class ApplicationLevelContainerDependentStartupFunction:public StartupFunction
{
    private:
        int priorityNumber;
        void (*startupFunction)(ApplicationLevelContainer &);
        ApplicationLevelContainer *p2ApplicationLevelContainer;
    public:
        ApplicationLevelContainerDependentStartupFunction(int priorityNumber,void (*startupFunction)(ApplicationLevelContainer &),ApplicationLevelContainer *p2ApplicationLevelContainer)
        {
            this->priorityNumber=priorityNumber;
            this->startupFunction=startupFunction;
            this->p2ApplicationLevelContainer=p2ApplicationLevelContainer;
        }
        int getPriorityNumber()
        {
            return this->priorityNumber;
        }
        void run()
        {
            startupFunction(*p2ApplicationLevelContainer);
        }
};

class StartupFunctionComparator
{
    public:
        int operator()(StartupFunction *e,StartupFunction *f)
        {
            return !(e->getPriorityNumber()<f->getPriorityNumber());
        }
};

class TemplateEngine
{
    private:
    struct vmd
    {
        long start_position;
        long end_position;
        char var_name[256];
    };
    static void createVMDFileName(const char *chtmlFileName,char *vmdFileName)
    {
        char *dotPtr;
        const char *ptr;
        ptr=chtmlFileName;
        while(*ptr!='\0') ptr++;
        while(ptr>=chtmlFileName)
        {
            if(*ptr=='/')
            {
                chtmlFileName=ptr+1;
                break;
            }
            ptr--;
        }
        for(;*chtmlFileName!='\0';chtmlFileName++,vmdFileName++)
        {
            if(*chtmlFileName=='.') dotPtr=vmdFileName;
            *vmdFileName=*chtmlFileName;
        }
        *vmdFileName='\0';
        strcpy(dotPtr+1,"vmd");
    }
    static void createVMDFile(const char *chtmlFileName,const char *pathToVMDFile)
    {
        char *start,*end,*ptr;
        unsigned int x;
        FILE *f;
        int toRead;
        char buffer[513];
        long fileLength;
        f=fopen(chtmlFileName,"rb");
        if(f==NULL)
        {
           // code to send back 404 error
           return;
        }
        FILE *vmdFile;
        vmdFile=fopen(pathToVMDFile,"wb");
        if(vmdFile==NULL)
        {

            // code to send back 500 internal server error page
            fclose(f);
            return ;
        }
        fseek(f,0,SEEK_END);
        fileLength=ftell(f);
        // cout<<fileLength<<endl;
        rewind(f);
        unsigned int sp,ep;
        int ii;
        x=0;
        struct vmd vmd_record;
        while(1)
        {
            if((fileLength-x)>256) toRead=256;
            else toRead=fileLength-x;
            // cout<<"Reading : "<<toRead<<" number of bytes"<<endl;
            fread(buffer,toRead,1,f);
            buffer[toRead]='\0';
            x+=toRead;
            // printf("%s",buffer);
            // processing the contents of the buffer starts here
            start=buffer;
            while(1)
            {
                start=strstr(start,"${");
                if(start==NULL)
                {
                    if(x==fileLength) break;
                    if(buffer[toRead-1]=='$')
                    {
                        buffer[0]='$';
                        if((fileLength-x)>255) toRead=255;
                        else toRead=fileLength-x;
                        fread(buffer+1,toRead,1,f);
                        x+=toRead;
                        buffer[1+toRead]='\0';
                        start=buffer;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                end=strstr(start+2,"}");
                if(end!=NULL)
                {
                    // printf("-------------------------\n");
                    for(ptr=start+1;ptr<end;ptr++) if(*ptr=='$' && *(ptr+1)=='{') break;
                    if(*ptr=='$') start=ptr;
                    // write vmd record to the file starts here
                    for(ptr=start+2,ii=0;ptr<end;ptr++,ii++)
                    {
                        vmd_record.var_name[ii]=*ptr;
                    }
                    vmd_record.var_name[ii]='\0';
                    sp=(x-toRead)+(start-buffer);
                    ep=(x-toRead)+(end-buffer);
                    cout<<"1--->"<<sp<<","<<ep<<endl;
                    vmd_record.start_position=sp;
                    vmd_record.end_position=ep;
                    fwrite(&vmd_record,sizeof(struct vmd),1,vmdFile);
                    // write vmd record to the file ends here
                    // for(ptr=start;ptr<=end;ptr++) printf("%c",*ptr);
                    // printf("\n");
                    // printf("-------------------------\n");
                    start=end+1;
                    continue;
                }
                // if } not found , then load the next chunk of 256 bytes
                // and store it in buffer from index 256
                if(x==fileLength) break;
                if((fileLength-x)>256) toRead=256;
                else toRead=fileLength-x;
                fread(buffer+256,toRead,1,f);
                buffer[256+toRead]='\0';
                x=x+toRead;
                end=strstr(buffer+256,"}");
                if(end==NULL) break;
                for(ptr=start+1;ptr<end;ptr++) if(*ptr=='$' && *(ptr+1)=='{') break;
                if(*ptr=='$') start=ptr;
                // write vmd record to the file starts here
                for(ptr=start+2,ii=0;ptr<end;ptr++,ii++)
                {
                    vmd_record.var_name[ii]=*ptr;
                }
                vmd_record.var_name[ii]='\0';
                sp=(x-toRead)+(start-buffer)-256;
                ep=(x-toRead)+(end-buffer)-256;
                cout<<"2--->"<<sp<<","<<ep<<endl;
                vmd_record.start_position=sp;
                vmd_record.end_position=ep;
                fwrite(&vmd_record,sizeof(struct vmd),1,vmdFile);
                // write vmd record to the file ends here
                // for(ptr=start;ptr<=end;ptr++) printf("%c",*ptr);
                // printf("\n");
                strcpy(buffer,buffer+256);
                end=end-256;
                start=end+1;
            }
            // processing the contents of the buffer ends here
            if(x==fileLength) break;
        }
        fclose(f);
        fclose(vmdFile);
    }
    public:
    static void processCHTMLFile(const char *chtmlFileName,Request &request,int clientSocketDescriptor)
    {
        if(!FileSystemUtility::directoryExists("vmd_files"))
        {
            if(!FileSystemUtility::createDirectory("vmd_files"))
            {
                // later on
            }
        }
        char vmdFileName[257];
        createVMDFileName(chtmlFileName,vmdFileName);
        string folderName=string("vmd_files");
        string pathToVMDFile=folderName+string(PATH_SEPARATOR)+string(vmdFileName);
        bool generateVMDFile=false;
        if(FileSystemUtility::fileExists(pathToVMDFile.c_str()))
        {
            if(FileSystemUtility::getLastUpdatedTime(chtmlFileName)>FileSystemUtility::getLastUpdatedTime(pathToVMDFile.c_str()))
            {
                generateVMDFile=true;
            }
        }
        else
        {
            generateVMDFile=true;
        }
        if(generateVMDFile)
        {
            createVMDFile(chtmlFileName,pathToVMDFile.c_str());
        }
        // process the chtml file (pick up info from VMD file)
        FILE *chtmlFile=fopen(chtmlFileName,"rb");
        FILE *vmdFile=fopen(pathToVMDFile.c_str(),"rb");
        fseek(chtmlFile,0,SEEK_END);
        long fileLength=ftell(chtmlFile);
        rewind(chtmlFile);  
        long responseSize=fileLength;
        string data;
        struct vmd vmdRecord;
        while(1)
        {
            fread(&vmdRecord,sizeof(struct vmd),1,vmdFile);
            if(feof(vmdFile)) break;
            responseSize=responseSize-((vmdRecord.end_position-vmdRecord.start_position)+1);
            data=request.getCHTMLVariable(vmdRecord.var_name);
            responseSize=responseSize+data.length();
        }
        cout<<"Response size is : "<<responseSize<<endl;
        string mimeType;
        mimeType=string("text/html");
        char header[200];
        sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n",mimeType.c_str(),responseSize);
        send(clientSocketDescriptor,header,strlen(header),0);
        long bytesLeftToRead;
        int bytesToRead;
        char buffer[4096];
        bytesLeftToRead=fileLength;
        rewind(vmdFile);
        long tmpBytesLeftToRead;
        long bytesProcessedFromFile=0;
        while(1)
        {
            fread(&vmdRecord,sizeof(struct vmd),1,vmdFile);
            if(feof(vmdFile)) break;
            tmpBytesLeftToRead=vmdRecord.start_position-bytesProcessedFromFile;
            bytesToRead=4096;
            while(tmpBytesLeftToRead>0)
            {
                if(tmpBytesLeftToRead<bytesToRead) bytesToRead=tmpBytesLeftToRead;
                fread(buffer,bytesToRead,1,chtmlFile);
                if(feof(chtmlFile)) break; // this won't happen on our case
                bytesProcessedFromFile+=bytesToRead;
                send(clientSocketDescriptor,buffer,bytesToRead,0);
                tmpBytesLeftToRead=tmpBytesLeftToRead-bytesToRead;
            }
            fread(buffer,(vmdRecord.end_position-vmdRecord.start_position)+1,1,chtmlFile);
            bytesProcessedFromFile+=(vmdRecord.end_position-vmdRecord.start_position)+1;
            string data;
            if(request.containsCHTMLVariable(vmdRecord.var_name))
            {
                data=request.getCHTMLVariable(vmdRecord.var_name);
                send(clientSocketDescriptor,data.c_str(),data.length(),0);
            }
        }
        bytesLeftToRead-=bytesProcessedFromFile;
        bytesToRead=4096;
        while(bytesLeftToRead>0)
        {
            if(bytesLeftToRead<bytesToRead) bytesToRead=bytesLeftToRead;
            fread(buffer,bytesToRead,1,chtmlFile);
            if(feof(chtmlFile)) break; // this won't happen on our case 
            send(clientSocketDescriptor,buffer,bytesToRead,0);
            bytesLeftToRead=bytesLeftToRead-bytesToRead;
        }
        fclose(chtmlFile);
        fclose(vmdFile);
    }
};

class Bro
{
    private:
        string staticResourceFolder;
        map<string,URLMapping> urlMappings;
        map<string,string> mimeTypes;
        ApplicationLevelContainer applicationLevelContainer;
        priority_queue<StartupFunction *,vector<StartupFunction *>,StartupFunctionComparator> startupFunctions;
        bool isCHTML(const char *requestURI)
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

        void processCHTMLResource(int clientSocketDiscriptor,const char * requestURI,Request &request)
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
        bool serveStaticResource(int clientSocketDescriptor,const char*requestURI)
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

        void addStartupService(int priorityNumber,void (*startupFunction)(void))
        {
            StartupFunction *sf;
            sf=new SimpleStartupFunction(priorityNumber,startupFunction);
            this->startupFunctions.push(sf);
        }
        void addStartupService(int priorityNumber,void (*startupFunction)(ApplicationLevelContainer &applicationLevelContainer))
        {
            StartupFunction *sf;
            sf=new ApplicationLevelContainerDependentStartupFunction(priorityNumber,startupFunction,&(this->applicationLevelContainer));
            this->startupFunctions.push(sf);
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
                auto urlMappingIterator=urlMappings.find(string(requestURI));
                if(urlMappingIterator==urlMappings.end())
                {
                    if(isCHTML(requestURI))
                    {
                        Request request(method,requestURI,httpVersion,dataInRequest);
                        processCHTMLResource(clientSocketDiscriptor,requestURI,request);
                    }
                    else if(!serveStaticResource(clientSocketDiscriptor,requestURI))
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
                    urlMappingIterator=urlMappings.find(forwardTo);
                    if(urlMappingIterator==urlMappings.end())
                    {
                        if(isCHTML(forwardTo.c_str()))
                        {
                            request.forwardTo(string(""));
                            processCHTMLResource(clientSocketDiscriptor,forwardTo.c_str(),request);
                        }
                        else if(!serveStaticResource(clientSocketDiscriptor,forwardTo.c_str()))
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

// Bobby [The Web application developer - User o<f bro web server]
class Bulb:public Stringifyable
{
    private:
        int wattage;
        int price;
    public:
        void setWattage(int wattage)
        {
            this->wattage=wattage;
        }
        int getWattage()
        {
            return this->wattage;
        }
        void setPrice(int price)
        {
            this->price=price;
        }
        int getPrice()
        {
            return this->price;
        }
        string stringify()
        {
            return "Bulb wattage is = "+to_string(this->wattage)+", price = "+to_string(this->price);
        }
};

int main()
{
    try
    {
        Bro bro;
        bro.setStaticResourceFolder("Whatever");
        bro.addStartupService(2,[](){
            cout<<"---------------------------------------------"<<endl;
            cout<<"some cool function that get call on startup"<<endl;
            cout<<"The priority number set for this function is 2"<<endl;
            cout<<"---------------------------------------------"<<endl;
        });
        bro.addStartupService(1,[](){
            cout<<"---------------------------------------------"<<endl;
            cout<<"some great function that get call on startup"<<endl;
            cout<<"The priority number set for this function is 1"<<endl;
            cout<<"---------------------------------------------"<<endl;
        });
        bro.addStartupService(1,[](){
            cout<<"---------------------------------------------"<<endl;
            cout<<"some great too function that get call on startup"<<endl;
            cout<<"The priority number set for this function is 1"<<endl;
            cout<<"---------------------------------------------"<<endl;
        });
        bro.addStartupService(3,[](ApplicationLevelContainer &applicationLevelContainer){
            cout<<"---------------------------------------------"<<endl;
            cout<<"some too cool function that get call on startup"<<endl;
            cout<<"The priority number set for this function is 3"<<endl;
            cout<<"---------------------------------------------"<<endl;
        });

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

        /*
            Bobby wants to specify the function(s) that shoud be called ,
            when server starts
            the function may require (ApplicationLevelContainer)
            
            Bobby should also be able to specify the priority level of the startup function
            priority will be decided the sequence of execution of the function specified by bobby
            weighted priority level 
            1 a
            2 b
            3 c
        */

        bro.get("/Slogan", [](Request &request, Response &response) {
            string slogan;
            ifstream iFile("data/sofd.data");
            string line;
            while(true)
            {
                if(!getline(iFile,line)) break;
                if(slogan.length()>0) slogan+=string("<br>");
                slogan+=line;
            }
            iFile.close();
            response.setContentType("text/html");
            const char *html = R""""(
            <!DOCTYPE html>
            <html>
            <head>
            <meta charset='utf-8'>
            <title>Bro test cases</title>
            </head>
            <body>
                <h1>Words of Wisdom</h1>
            )"""";
            response<<html;
            response<<slogan.c_str();
            const char*html2=R""""(
            </body>
            </html>
            )"""";
            response<<html2;
            
        });

        bro.get("/SloganOfTheDay", [](Request &request, Response &response) {
            string slogan;
            ifstream iFile("data/sofd.data");
            string line;
            while(true)
            {
                if(!getline(iFile,line)) break;
                if(slogan.length()>0) slogan+=string("<br>");
                slogan+=line;
            }
            iFile.close();
            cout<<"size of slogan of the day is : "<<slogan.length()<<endl;
            request.setCHTMLVariable("sloganOfTheDay",slogan);
            request.setCHTMLVariable("city1","indore");
            request.setCHTMLVariable("city2","goa");
            Bulb bulb;
            bulb.setWattage(100);
            bulb.setPrice(250);
            request.setCHTMLVariable("bulb",&bulb);
            short int a=10;
            unsigned short int b=65520;
            int c=-200000;
            unsigned int d=4000000000;  
            long int e=-9000000000;
            unsigned long int f=18000000000;
            float g=3.14159f;
            double h=2.718281828459045;
            bool i=true;
            request.setCHTMLVariable("a",a);
            request.setCHTMLVariable("b",b);
            request.setCHTMLVariable("c",c);
            request.setCHTMLVariable("d",d);
            request.setCHTMLVariable("e",e);
            request.setCHTMLVariable("f",f); 
            request.setCHTMLVariable("g",g);
            request.setCHTMLVariable("h",h);
            request.setCHTMLVariable("i",i);
            _forward_(request,string("/WordsOfWisdom.chtml"));   
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