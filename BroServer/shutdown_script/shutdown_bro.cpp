#include<iostream>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
using namespace std;
int main()
{
    int success;
    int x;
    int portNumber=6060;
    int socketDescriptor;
    struct sockaddr_in serverAddress;
    char messageToSend[]="GET /shutdown HTTP/1.1\r\nHost: localhost:6060\r\nAccept: text/html\r\n";
    char responseBuffer[1024];
    x=1;
    while(x<=2)
    {
        socketDescriptor=socket(AF_INET,SOCK_STREAM,0);
        if(socketDescriptor<0)
        {
            cout<<"Socket creation failed"<<endl;
            return 1;
        }
        serverAddress.sin_family=AF_INET;
        serverAddress.sin_port=htons(portNumber);
        success=inet_pton(AF_INET,"127.0.0.1",&(serverAddress.sin_addr));
        if(success<=0)
        {
            cout<<"Unable to resolve 127.0.0.1"<<endl;
            close(socketDescriptor);
            return 1;
        }
        success=connect(socketDescriptor,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
        if(success<0)
        {
            cout<<"Connection failed"<<endl;
            close(socketDescriptor);
            return 1;
        }
        send(socketDescriptor,messageToSend,strlen(messageToSend),0);
        read(socketDescriptor,responseBuffer,1024);
        close(socketDescriptor);
        x++;
    }
    cout<<"Shutdown initiated"<<endl;
    return 0;
}
