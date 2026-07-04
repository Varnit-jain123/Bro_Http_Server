#ifndef __BRO_TEMPLATE_ENGINE__
#define __BRO_TEMPLATE_ENGINE__
    #include <bro_global.h>
    #include <bro_request.h>
    class TemplateEngine
    {
        private:
        struct vmd
        {
            long start_position;
            long end_position;
            char var_name[256];
        };
        static void createVMDFileName(const char *chtmlFileName,char *vmdFileName);
        static void createVMDFile(const char *chtmlFileName,const char *pathToVMDFile);
        public:
        static void processCHTMLFile(const char *chtmlFileName,Request &request,int clientSocketDescriptor);
    };

#endif
