#include<iostream>
#include<string.h>

#ifdef _WIN32
    #include<winsock2.h>
    #include<ws2tcpip.h>
    #define CLOSE_SOCKET(s) closesocket(s)
    typedef int socklen_t;
#else
    #include<sys/socket.h>
    #include<unistd.h>
    #include<arpa/inet.h>
    #define CLOSE_SOCKET(s) close(s)
#endif

using namespace std;

int main()
{
    #ifdef _WIN32
        WSADATA wsaData;
        if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        {
            cout<<"WSAStartup failed"<<endl;
            return 1;
        }
    #endif

    int success;
    int x;
    int portNumber=6060;
    int socketDescriptor;
    struct sockaddr_in serverAddress;
    char messageToSend[]="GET /shutdown HTTP/1.1\r\nHost: localhost:6060\r\nAccept: text/html\r\n\r\n";
    char responseBuffer[1024];
    x=1;
    while(x<=2)
    {
        socketDescriptor=socket(AF_INET,SOCK_STREAM,0);
        if(socketDescriptor<0)
        {
            cout<<"Socket creation failed"<<endl;
            #ifdef _WIN32
                WSACleanup();
            #endif
            return 1;
        }
        serverAddress.sin_family=AF_INET;
        serverAddress.sin_port=htons(portNumber);
        success=inet_pton(AF_INET,"127.0.0.1",&(serverAddress.sin_addr));
        if(success<=0)
        {
            cout<<"Unable to resolve 127.0.0.1"<<endl;
            CLOSE_SOCKET(socketDescriptor);
            #ifdef _WIN32
                WSACleanup();
            #endif
            return 1;
        }
        success=connect(socketDescriptor,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
        if(success<0)
        {
            cout<<"Connection failed - is the server running on port "<<portNumber<<"?"<<endl;
            CLOSE_SOCKET(socketDescriptor);
            #ifdef _WIN32
                WSACleanup();
            #endif
            return 1;
        }
        send(socketDescriptor,messageToSend,strlen(messageToSend),0);
        recv(socketDescriptor,responseBuffer,1024,0);
        CLOSE_SOCKET(socketDescriptor);
        x++;
    }
    cout<<"Shutdown initiated successfully"<<endl;
    #ifdef _WIN32
        WSACleanup();
    #endif
    return 0;
}
