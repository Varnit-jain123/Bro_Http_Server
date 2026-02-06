#ifndef __BRO_APPLICATION_LEVEL_CONTAINER_DEPENDENT_FUNCTION__
#define __BRO_APPLICATION_LEVEL_CONTAINER_DEPENDENT_FUNCTION__
    #include<bro_global.h>
    #include<bro_application_level_container.h>
    #include<bro_request.h>
    #include<bro_response.h>
    #include<bro_function.h>
    class ApplicationLevelContainerDependentFunction:public Function
    {
    private:
        void (*mappedFunction)(Request &,Response &,ApplicationLevelContainer &);
        ApplicationLevelContainer *p2ApplicationLevelContainer;
    public:
        ApplicationLevelContainerDependentFunction(void(*mappedFunction)(Request &,Response &,ApplicationLevelContainer &),ApplicationLevelContainer *p2ApplicationLevelContainer);
        void doService(Request &request,Response &response);
    };

#endif