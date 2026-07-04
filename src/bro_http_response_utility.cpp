#include<bro_http_response_utility.h>
HTTPResponseUtility::HTTPResponseUtility()
{
}
void HTTPResponseUtility::sendResponse(int clientSocketDescriptor,Response &response)
{
//char header[200];
char *header=new char[200+response.cookiesDataLength];
int contentLength=response.contentLength;
sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type:text/html\nContent-Length:%d\n",contentLength);
for(string cookieString:response.cookies)
{
    strcat(header,"Set-Cookie: ");
    strcat(header,cookieString.c_str());
    strcat(header,"\r\n"); 
}
strcat(header,"Connection: close\r\n\r\n");
send(clientSocketDescriptor,header,strlen(header),0);
auto contentIterator=response.content.begin();
while(contentIterator!=response.content.end())
{
    string str=*contentIterator;
    send(clientSocketDescriptor,str.c_str(),str.length(),0);
    ++contentIterator;        
}
delete []header;
}

