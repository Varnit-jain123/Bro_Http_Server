#include<bro_validator.h>
#include <bro_file_system_utility.h>
Validator::Validator(){}
bool Validator::isValidPath(string &path)
{
    return FileSystemUtility::directoryExists(path.c_str());
}
bool Validator::isValidMIMEType(string &mimeType)
{
    // basic validation for MIME type
    return true;
}
bool Validator::isValidURLFormat(string &url)
{
    // basic validation for URL pattern
    return true;
}