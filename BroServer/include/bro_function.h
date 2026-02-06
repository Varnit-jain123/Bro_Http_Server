#ifndef __BRO_FUNCTION__
#define __BRO_FUNCTION__
    #include<bro_global.h>
    #include<bro_request.h>
    #include<bro_response.h>
    class Function
    {
    public:
    virtual void doService(Request &,Response &)=0;
    };
    typedef struct __url__mapping
{
    __request_method__ requestMethod;
    Function *function;
    // void ( *mappedFunction)(Request &,Response &);
    }URLMapping;

#endif