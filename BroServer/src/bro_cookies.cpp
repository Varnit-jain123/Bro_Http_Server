#include<bro_cookies.h>
string name;
string value;
string expiresOn;
long maxAge;
string domain;
string path;
bool isSecure;
bool isHttpOnly;
COOKIE_SAME_SITE_FLAG sameSiteFlag;
Cookie::Cookie(string name,string value)
{
    this->name=name;
    this->value=value;
    this->expiresOn="";
    this->maxAge=-1; 
    this->domain="";
    this->path="";
    this->isSecure=false;
    this->isHttpOnly=false;
}
string Cookie::getName()
{
    return this->name;
}
string Cookie::getValue()
{
    return this->value;
}
void Cookie::setExpiresOn(GMTDateTime &gmtDateTime)
{
    this->expiresOn=gmtDateTime.stringfy();
}
string Cookie::getExpiresOn()
{
    return this->expiresOn;
}
void Cookie::setMaxAgeInSeconds(int maxAgeInSeconds)
{
    this->maxAge=maxAgeInSeconds;
}
void Cookie::setMaxAgeInMinutes(int maxAgeInMinutes)
{
    this->maxAge=maxAgeInMinutes*60;
}
void Cookie::setMaxAgeInHours(int maxAgeInHours)
{
    this->maxAge=maxAgeInHours*60*60;
}
void Cookie::setDomain(string domain)
{
    this->domain=domain;
}
string Cookie::getDomain()
{
    return this->domain;
}
void Cookie::setPath(string path)
{
    this->path=path;
}
string Cookie::getPath()
{            
    return this->path;
}
void Cookie::setSecure(bool secure)
{
    this->isSecure=secure;
}
bool Cookie::getSecure()
{
    return this->isSecure;
}
void Cookie::setHttpOnly(bool httpOnly)
{
    this->isHttpOnly=httpOnly;
}
bool Cookie::getHttpOnly()
{            
    return this->isHttpOnly;
}
string Cookie::stringfy()
{
    string cookieString=this->name+string("=")+this->value;
    if(this->expiresOn.length()>0) cookieString+=string("; Expires=")+this->expiresOn;
    if(this->maxAge!=-1) cookieString+=string("; Max-Age=")+to_string(this->maxAge);
    if(this->domain.length()>0) cookieString+=string("; Domain=")+this->domain;
    if(this->path.length()>0) cookieString+=string("; Path=")+this->path;
    if(this->isSecure) cookieString+=string("; Secure");
    if(this->isHttpOnly) cookieString+=string("; HttpOnly");
    switch (sameSiteFlag)
    {
        case COOKIE_SAME_SITE_STRICT:
            cookieString+=string("; SameSite=Strict");
            break;
        case COOKIE_SAME_SITE_LAX:
            cookieString+=string("; SameSite=Lax");
            break;
        case COOKIE_SAME_SITE_NONE:
            cookieString+=string("; SameSite=None");
            break;
        default:
            break;
    }
    cout<<"+++++++++++++++++++++"<<endl;
    cout<<"Cookie String:["<<cookieString<<"]"<<endl;
    cout<<"+++++++++++++++++++++"<<endl;
    return cookieString;
}


