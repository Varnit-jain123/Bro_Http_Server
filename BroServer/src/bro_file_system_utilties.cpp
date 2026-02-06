#include<bro_file_system_utility.h>
FileSystemUtility::FileSystemUtility(){}
bool FileSystemUtility::createDirectory(const char *directoryName)
{
    return filesystem::create_directory(directoryName);
}
unsigned int FileSystemUtility::getLastUpdatedTime(const char *fileName)
{
    struct stat attributes;
    stat(fileName,&attributes);
    return attributes.st_mtime;
}
bool FileSystemUtility::fileExists(const char *path)
{
    struct stat s;
    int x;
    x=stat(path,&s);
    if(x!=0) return false;
    if(s.st_mode & S_IFDIR) return false;
    return true;
}
bool FileSystemUtility::directoryExists(const char *path)
{
    struct stat s;
    int x;
    x=stat(path,&s);
    if(x!=0) return false;
    if(s.st_mode & S_IFDIR) return true;
    return false;
}
string FileSystemUtility::getFileExtension(const char *path)
{
    int x;
    x=strlen(path)-1;
    while(x>=0 && path[x]!='.') x--;
    if(x==-1 || path[x]!='.')  return string("");
    return string(path+(x+1));
}
