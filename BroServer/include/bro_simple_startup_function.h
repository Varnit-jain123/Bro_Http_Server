#ifndef __BRO_SIMPLE_STARTUP_FUNCTION__
#define __BRO_SIMPLE_STARTUP_FUNCTION__
    #include <bro_global.h>
    #include <bro_startup_function.h>
    class SimpleStartupFunction:public StartupFunction 
    {
        private:
            void (*startupFunction)(void);
            int priorityNumber;
        public:
            SimpleStartupFunction(int priorityNumber,void (*startupFunction)(void));
            int getPriorityNumber();
            void run();
    };
#endif