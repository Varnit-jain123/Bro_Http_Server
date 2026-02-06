#include<bro_http_error_status_utility.h>
HttpErrorStatusUtility::HttpErrorStatusUtility()
{

}
void HttpErrorStatusUtility::sendBadRequestError(int clientSocketDiscriptor)
{
    // will be implemented later
}
void HttpErrorStatusUtility::sendHttpVersionNotSupportedError(int clientSocketDiscriptor,char *httpVersion)
{
    // will be implemented later
}
void HttpErrorStatusUtility::sendNotFoundError(int clientSocketDiscriptor,char *requestURI)
{
    char content[1000];
    char header[200];
    char response[2000];
    sprintf(content,"<!DOCTYPE html> <html lang='en'> <head> <meta charset='UTF-8'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <title>404 Error</title> </head> <body> Requested Resource [%s] Not found </body> ",requestURI);
    int contentLength=strlen(content);
    sprintf(header,"HTTP/1.1 404 Not Found\r\nContent-Type:text/html\nContent-Length:%d\nConnection: close\r\n\r\n",contentLength);
    strcpy(response,header);
    strcat(response,content);
    send(clientSocketDiscriptor,response,strlen(response),0);
}
void HttpErrorStatusUtility::sendMethodNotAllowedError(int clientSocketDiscriptor,const char *method,char *requestURI)
{
    // will be implemented later        
}

