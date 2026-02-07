#ifndef __BRO__
#define __BRO__
    #include<bro_global.h>
    #include<bro_request.h>
    #include<bro_response.h>
    #include<bro_error.h>
    #include<bro_application_level_container.h>
    #include<bro_startup_function.h>
    #include<bro_startup_function_comparator.h>
    #include<bro_function.h>
    #include<thread>
    #include<bro_thread_wrapper_node.h>
    class Bro
    {
        private:
            string staticResourceFolder;
            map<string,URLMapping> urlMappings;
            map<string,string> mimeTypes;
            ApplicationLevelContainer applicationLevelContainer;
            priority_queue<StartupFunction *,vector<StartupFunction *>,StartupFunctionComparator> startupFunctions;
            bool isCHTML(const char *requestURI);
        public:
            Bro();
            ~Bro();
            void setStaticResourceFolder(string staticResourceFolder);
            void processCHTMLResource(int clientSocketDiscriptor,const char * requestURI,Request &request);
            bool serveStaticResource(int clientSocketDescriptor,const char*requestURI);
            void addStartupService(int priorityNumber,void (*startupFunction)(void));
            void addStartupService(int priorityNumber,void (*startupFunction)(ApplicationLevelContainer &applicationLevelContainer));
            void get(string url,void (*callBack)(Request &,Response &));
            void get(string url,void (*callBack)(Request &,Response &,ApplicationLevelContainer &));
            void post(string url,void (*callBack)(Request &,Response &));
            void listen(int portNumber,void (*callBack)(Error &));    
            friend void request_processor(int clientSocketDiscriptor,Bro *bro,BroThreadWrapperNode *broThreadWrapperNode);
    };

#endif