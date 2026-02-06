#ifndef __BRO_HEADER_UTILITY__
#define __BRO_HEADER_UTILITY__
    #include<bro_global.h>
    class HeaderUtility
    {
    private:
        HeaderUtility();
    public:
        static void parseHeader(const char *header,map<string,string> &headerFieldsMap);
    };
#endif