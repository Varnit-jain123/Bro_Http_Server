#include<bro_global.h>
#ifdef _WIN32
    const char *PATH_SEPARATOR="\\";
    #include<windows.h>
#endif
#ifdef linux
    const char *PATH_SEPARATOR="/";
#endif