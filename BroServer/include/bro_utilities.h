#ifndef __BRO_UTILITIES__
#define __BRO_UTILITIES__
    #include<bro_global.h>
    class BroUtilities
    {
        private:
        BroUtilities();
        public:
        static bool isHexChar(int w);
        static void decode(char *encodedString,char *decodedString);
        static void loadMIMETypes(map<string,string> &mimeTypesMap);
    };
#endif