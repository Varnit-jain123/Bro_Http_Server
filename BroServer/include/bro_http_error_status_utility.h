#ifndef __BRO_HTTP_ERROR_STATUS_UTILITY__
#define __BRO_HTTP_ERROR_STATUS_UTILITY__
    #include<bro_global.h>
    class HttpErrorStatusUtility
    {
    private:
    HttpErrorStatusUtility();
    public:
        static void sendBadRequestError(int clientSocketDiscriptor);
        static void sendHttpVersionNotSupportedError(int clientSocketDiscriptor,char *httpVersion);
        static void sendNotFoundError(int clientSocketDiscriptor,char *requestURI);
        static void sendMethodNotAllowedError(int clientSocketDiscriptor,const char *method,char *requestURI);
    };
#endif