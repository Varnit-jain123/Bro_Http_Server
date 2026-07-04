#ifndef __BRO_COOKIES__
#define __BRO_COOKIES__
    #include<bro_global.h>
    #include<bro_gmt_date_time.h>
    typedef enum _cookie_same_site_flag
    {
    COOKIE_SAME_SITE_STRICT,
    COOKIE_SAME_SITE_LAX,
    COOKIE_SAME_SITE_NONE   
    }COOKIE_SAME_SITE_FLAG;

    class Cookie
    {
    private:
        string name;
        string value;
        string expiresOn;
        long maxAge;
        string domain;
        string path;
        bool isSecure;
        bool isHttpOnly;
        COOKIE_SAME_SITE_FLAG sameSiteFlag;
    public:
        Cookie(string name,string value);
        string getName();
        string getValue();
        void setExpiresOn(GMTDateTime &gmtDateTime);
        string getExpiresOn();
        void setMaxAgeInSeconds(int maxAgeInSeconds);
        void setMaxAgeInMinutes(int maxAgeInMinutes);
        void setMaxAgeInHours(int maxAgeInHours);
        void setDomain(string domain);
        string getDomain();
        void setPath(string path);
        string getPath();
        void setSecure(bool secure);
        bool getSecure();
        void setHttpOnly(bool httpOnly);
        bool getHttpOnly();
        string stringfy();
    };
#endif