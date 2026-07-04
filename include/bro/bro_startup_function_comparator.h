#ifndef __BRO_STARTUP_FUNCTION_COMPARATOR__
#define __BRO_STARTUP_FUNCTION_COMPARATOR__
    #include <bro_global.h>
    #include <bro_startup_function.h>
    class StartupFunctionComparator
    {
    public:
        int operator()(StartupFunction *e,StartupFunction *f);
    };
#endif