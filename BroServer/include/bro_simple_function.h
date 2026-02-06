#ifndef __BRO_SIMPLE_FUNCTION__
#define __BRO_SIMPLE_FUNCTION__
    #include<bro_global.h>
    #include<bro_request.h>
    #include<bro_response.h>
    #include<bro_function.h>
    class SimpleFunction:public Function
    {
    private:
        void (*mappedFunction)(Request &,Response &);
    public:
        SimpleFunction(void (*mappedFunction)(Request &,Response &));
        void doService(Request &request,Response &response);
    };
#endif