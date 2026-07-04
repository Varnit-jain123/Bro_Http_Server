#ifndef __BRO_FILE_SYSTEM_UTILITY__
#define __BRO_FILE_SYSTEM_UTILITY__
    #include<bro_global.h>
    class FileSystemUtility
    {
        private:
        FileSystemUtility();
        public:
        static bool createDirectory(const char *directoryName);
        static unsigned int getLastUpdatedTime(const char *fileName);
        static bool fileExists(const char *path);
        static bool directoryExists(const char *path);
        static string getFileExtension(const char *path);
    };
#endif