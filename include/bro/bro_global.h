#ifndef __BRO_GLOBAL__
#define __BRO_GLOBAL__
    #include<bits/stdc++.h>
    #include<iostream>
    #include<map>
    #include<forward_list>
    #include<queue>
    #include<vector>
    #include<string.h>
    #include<unistd.h>
    #include<sys/stat.h>
    #include <filesystem>
    #include<set> 
    
    #ifdef _WIN32
        #include<windows.h>
    #endif
    #ifdef linux
        #include<sys/socket.h>
        #include<arpa/inet.h>
    #endif

    extern const char *PATH_SEPARATOR;
    #ifdef _WIN32
        #include<windows.h>
    #endif
    
    using namespace std;

    // Amit [The bro programmer]

    #define _forward_(request,url) \
    request.forwardTo(url); \
    return;

    enum __container_operation_failure_reason__
    {
        __KEY_EXISTS__,
        __KEY_DOES_NOT_EXIST__,
        __OUT_OF_MEMORY__,
        __VALUE_SIZE_MISMATCH__
    };

    enum __request_method__
    {   
    __GET__,
    __POST__,
    __PUT__,
    __DELETE__,
    __CONNECT__,
    __OPTIONS__,
    __TRACE__,
    __HEAD__
    };


#endif

