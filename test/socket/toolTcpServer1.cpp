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
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(-1 == SocketFD)
    {
        perror("can not create socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("error bind failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if(-1 == listen(SocketFD, 10))
    {
        perror("error listen failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    for(;;)
    {
        int ConnectFD = accept(SocketFD, NULL, NULL);

        if(0 > ConnectFD)
        {
            perror("error accept failed");
            close(SocketFD);
            exit(EXIT_FAILURE);
        }

        int iCount = 0;
        int iRec = 0;
        while (1)
        {
            iRec += recv(ConnectFD, buffer, sizeof(buffer), 0);
            printf("recv : %d - %dMB\n", ++iCount, iRec / ( 1024 * 1024) );
        }

        /* perform read write operations ... */

        shutdown(ConnectFD, SD_SEND);

        close(ConnectFD);
    }

    close(SocketFD);

#if defined(__WIN32) || defined(__WIN32__)
    WSACleanup();
#endif
    return 0;
}

