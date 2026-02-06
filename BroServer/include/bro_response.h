#ifndef __BRO_RESPONSE__
#define __BRO_RESPONSE__
    #include<bro_global.h>
    #include<bro_cookies.h>
    class Response
    {
        private :
            set<string> cookies;
            string contentType;
            int cookiesDataLength;
            forward_list<string> content;
            forward_list<string>::iterator contentIterator;
            unsigned long contentLength;
        public:
            Response();
            ~Response();
            void setContentType(string contentType);
            void addCookie(Cookie &cookie);
            Response & operator<<(Cookie cookie);
            Response & operator<<(string content);
            friend class HTTPResponseUtility;
    };
#endif