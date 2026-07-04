#ifndef __BRO_VALIDATOR__
#define __BRO_VALIDATOR__
    #include<bro_global.h>
    class Validator
    {
    private:
    Validator();
    public:
        static bool isValidPath(string &path);
        static bool isValidMIMEType(string &mimeType);
        static bool isValidURLFormat(string &url);
    };
#endif