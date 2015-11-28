#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#if defined(__WIN32) || defined(__WIN32__)
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

char buffer[10*1024*1024];

int main(void)
{
#if defined(__WIN32) || defined(__WIN32__)
    int iResult;
    WSADATA wsaData;

    //----------------------
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
#endif

    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == SocketFD)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

//    stSockAddr.sin_family = AF_INET;
//    stSockAddr.sin_port = htons(1100);
//    Res = inet_pton(AF_INET, "192.168.1.3", &stSockAddr.sin_addr);

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    stSockAddr.sin_port = htons( 1100 );

//    if (0 > Res)
//    {
//        perror("error: first parameter is not a valid address family");
//        close(SocketFD);
//        exit(EXIT_FAILURE);
//    }
//    else if (0 == Res)
//    {
//        perror("char string (second parameter does not contain valid ipaddress");
//        close(SocketFD);
//        exit(EXIT_FAILURE);
//    }

    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        send(SocketFD, buffer, sizeof(buffer), 0);
        ::Sleep(10);
    }

    /* perform read write operations ... */

    shutdown(SocketFD, SD_SEND);

    close(SocketFD);

#if defined(__WIN32) || defined(__WIN32__)
    WSACleanup();
#endif
    return 0;
}

