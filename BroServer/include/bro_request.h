#ifndef __BRO_REQUEST__
#define __BRO_REQUEST__
    #include<bro_global.h>
    #include <bro_cookies.h>
    #include<bro_container.h>
    #include<bro_stringifyable.h>
    #include<bro_utilities.h>
    class Bro;
    class Request:public Container
    {
    private:
    map<string,string> dataMap;
    map<string,string> &headerFieldsMap;
    string _forwardTo;
    char *method;
    const char *requestURI;
    char *httpVersion;
    map<string,string> varMap; // this will be changed later on
    Request(char *method,const char *requestURI,char *httpVersion,char *dataInRequest,map<string,string> &headerFieldsMap);
    string forwardToWhichResource();
    bool isToBeForwarded();
    void createDataMap(char *str,map<string,string> &dataMap);
    public:
    void setCHTMLVariable(string name,string value); // changed later on
    void setCHTMLVariable(string name,Stringifyable *stringifyable);
    void setCHTMLVariable(string name,const char *ptr);
    void setCHTMLVariable(string name,short int value);
    void setCHTMLVariable(string name,unsigned short int value);
    void setCHTMLVariable(string name,int value);
    void setCHTMLVariable(string name,unsigned int value);
    void setCHTMLVariable(string name,long value);
    void setCHTMLVariable(string name,unsigned long value);
    void setCHTMLVariable(string name,long long int value);
    void setCHTMLVariable(string name,unsigned long long int value);
    void setCHTMLVariable(string name,float value);
    void setCHTMLVariable(string name,double value);
    void setCHTMlVariable(string name,long double value);
    void setCHTMLVariable(string name,char value);
    void setCHTMLVariable(string name,unsigned char value);
    void setCHTMLVariable(string name,bool value);
    bool containsCHTMLVariable(string name); // changed later on
    string getCHTMLVariable(string name); // changed later on
    void forwardTo(string _forwardTo);
    string operator[](string key);
    string getCookieValue(string name);
    void getCookies(list<Cookie> &cookies);
    void getCookieNames(list<string> &cookieName);
    friend class Bro;
    friend void request_processor(int clientSocketDiscriptor,Bro *bro);
    };
#endif