#ifndef __BRO_HTTP_RESPONSE_UTILITY__
#define __BRO_HTTP_RESPONSE_UTILITY__
    #include<bro_global.h>
    #include<bro_response.h>
    class HTTPResponseUtility
    {
        private:
        HTTPResponseUtility();
        public:
        static void sendResponse(int clientSocketDescriptor,Response &response);
    };
#endif