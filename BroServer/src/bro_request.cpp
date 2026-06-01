#include<bro_request.h>
Request::Request(char *method,const char *requestURI,char *httpVersion,char *dataInRequest,map<string,string> &headerFieldsMap) : headerFieldsMap(headerFieldsMap)
{
    this->method=method;
    this->requestURI=requestURI;
    this->httpVersion=httpVersion;
    if(dataInRequest!=NULL && (strcmp(this->method,"get")==0 || strcmp(this->method,"post")==0))
    {
        createDataMap(dataInRequest,dataMap);
    }
}
string Request::forwardToWhichResource()
{
    return this->_forwardTo;
}
bool Request::isToBeForwarded()
{
    return this->_forwardTo.length()>0;
}

void Request::createDataMap(char *str,map<string,string> &dataMap)
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
void Request::setCHTMLVariable(string name,string value) // changed later on
{
    varMap.insert({name,value}); // map.insert(pair<string,string>(name,value)); 
}
void Request::setCHTMLVariable(string name,Stringifyable *stringifyable)
{
    varMap.insert({name,stringifyable->stringify()}); 
}
void Request::setCHTMLVariable(string name,const char *ptr)
{
    varMap.insert({name,string(ptr)}); 
}
void Request::setCHTMLVariable(string name,short int value)
{
    varMap.insert({name,to_string(value)}); 
}
void Request::setCHTMLVariable(string name,unsigned short int value)
{
    varMap.insert({name,to_string(value)}); 
}
void Request::setCHTMLVariable(string name,int value)
{
    varMap.insert({name,to_string(value)}); 
}
void Request::setCHTMLVariable(string name,unsigned int value)
{
    varMap.insert({name,to_string(value)}); 
}
void Request::setCHTMLVariable(string name,long value)
{
    varMap.insert({name,to_string(value)}); 
}
void Request::setCHTMLVariable(string name,unsigned long value)
{
    varMap.insert({name,to_string(value)}); 
}
void Request::setCHTMLVariable(string name,long long int value)
{
    varMap.insert({name,to_string(value)}); 
}
void Request::setCHTMLVariable(string name,unsigned long long int value)
{
    varMap.insert({name,to_string(value)}); 
}
void Request::setCHTMLVariable(string name,float value)
{
    varMap.insert({name,to_string(value)}); 
}
void Request::setCHTMLVariable(string name,double value)
{
    varMap.insert({name,to_string(value)}); 
}
void Request::setCHTMlVariable(string name,long double value)
{
    varMap.insert({name,to_string(value)}); 
}
void Request::setCHTMLVariable(string name,char value)
{
    string str;
    str+=value;
    varMap.insert({name,str}); 
}
void Request::setCHTMLVariable(string name,unsigned char value)
{
    string str;
    str+=value;
    varMap.insert({name,str}); 
}
void Request::setCHTMLVariable(string name,bool value)
{
    varMap.insert({name,(value==true)?"true":"false"}); 
}
bool Request::containsCHTMLVariable(string name) // changed later on
{
    return varMap.find(name)!=varMap.end();
}
string Request::getCHTMLVariable(string name) // changed later on
{
    string data="";
    auto a=varMap.find(name);
    if(a==varMap.end()) return string("");
    return a->second; 
}
void Request::forwardTo(string _forwardTo)
{
    this->_forwardTo=_forwardTo;
}
string Request::operator[](string key)
{
    auto iterator=dataMap.find(key);
    if(iterator==dataMap.end())
    {
        return string("");
    }
    return iterator->second;
}
string Request::getCookieValue(string name)
{
    auto f=headerFieldsMap.find("cookie");
    if(f==headerFieldsMap.end()) return string("");
    string cookiesString=f->second;
    const char *ptr=cookiesString.c_str()   ;
    const char *nsp,*nep,*vsp,*vep;
    while(*ptr)
    {
        if(*ptr==' ') ptr++;
        if(*ptr=='\0') break;
        nsp=ptr;
        nep=ptr;
        while(*nep && *nep!='=') nep++;
        if(*nep=='\0') break; // this will not happen
        vsp=nep+1;
        vep=vsp;
        while(*vep && *vep!=';') vep++;
        nep--; // nep is pointing to the block that contains =
        vep--; // vep is pointing to the block that contains \0 or ;
        string cookieName(nsp,nep-nsp+1);
        string cookieValue(vsp,vep-vsp+1);
        // cout<<"Cookie Name:["<<cookieName<<"]"<<endl;       
        // cout<<"Cookie Value:["<<cookieValue<<"]"<<endl;       
        if(cookieName==name) return cookieValue;
        ptr=vep+2;
    }
    return string("");
}
void Request::getCookies(list<Cookie> &cookies)
{
    // code to be implemented later
}
void Request::getCookieNames(list<string> &cookieName)
{
    // code to be implemented later
}
// request related data and methods