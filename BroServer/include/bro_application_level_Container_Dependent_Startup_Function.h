#ifndef __BRO_APPLICATION_LEVEL_CONTAINER_DEPENDENT_STARTUP_FUNCTION__
#define __BRO_APPLICATION_LEVEL_CONTAINER_DEPENDENT_STARTUP_FUNCTION__
    #include <bro_global.h>
    #include <bro_startup_function.h>
    #include <bro_application_level_container.h>
    class ApplicationLevelContainerDependentStartupFunction:public StartupFunction
    {
        private:
            int priorityNumber;
            void (*startupFunction)(ApplicationLevelContainer &);
            ApplicationLevelContainer *p2ApplicationLevelContainer;
        public:
            ApplicationLevelContainerDependentStartupFunction(int priorityNumber,void (*startupFunction)(ApplicationLevelContainer &),ApplicationLevelContainer *p2ApplicationLevelContainer);
            int getPriorityNumber();
            void run();
    };
#endif