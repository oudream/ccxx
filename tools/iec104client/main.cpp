/* Client code in C */

#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#include "iec104protocol.h"

using namespace std;

bool f_read_finish;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
vector<char> f_chaches;


void recSocket(void* v)
{
    int * pSocketFD = v;
    int SocketFD = * pSocketFD;
    char recvBuffer[4096];
    while (1)
    {
        int recvBytes = recv(SocketFD, recvBuffer, 4096, 0);
        if ( recvBytes < 0 )
        {
            printf("Receive buffer error!\n");
            close(SocketFD);
            return -1;
        }
        else
        {
            printf("Received buffer(%d):", recvBytes);
            for (int i = 0; i < recvBytes; i++)
            {
                printf("%02x ", recvBuffer[i]);
            }
            printf("\n");

            pthread_mutex_lock(&count_mutex);
            f_chaches.insert(f_chaches.end(), recvBuffer, recvBytes);
            pthread_mutex_unlock(&count_mutex);
        }
    }
    f_read_finish = true;
}


class SendSocketImpl : public SendSocket
{
public:
    SendSocketImpl(int SocketFD)
        : m_SocketFD(SocketFD)
    {

    }

    int sendData(const char *pData, int iLength)
    {
        int sendBytes = 0;

        if( (sendBytes = send(socket_fd, pData, iLength, 0)) < 0 )
        {
            printf("send message error\n");
            close(socket_fd);
            return -1;
        }
        else
        {
            printf("Send message(%d):\n", sendBytes);
            for (int i = 0; i < sendBytes; i++)
            {
                printf("%x ", pData[i]);
            }
            printf("\n");
        }

        return sendBytes;
    }

private:
    int m_SocketFD;

};

int main(void)
{
    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == SocketFD)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    Res = inet_pton(AF_INET, "192.168.1.3", &stSockAddr.sin_addr);

    if (0 > Res)
    {
        perror("error: first parameter is not a valid address family");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    else if (0 == Res)
    {
        perror("char string (second parameter does not contain valid ipaddress");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    f_read_finish = false;
    pthread_mutex_init(&count_mutex, NULL);
    SendSocketImpl sendSocket(SocketFD);
    IEC104Protocol iec104(&sendSocket);
    pthread_t p;
    pthread_create(&p, NULL, recSocket, (void *) &SocketFD);

    while (1)
    {
        if (f_read_finish)
        {
            break;
        }

        if ( f_chaches.size() )
        {
            vector<char> data;
            pthread_mutex_lock(&count_mutex);
            data.insert(data.end(), f_chaches.begin(), f_chaches.size());
            f_chaches.clear();
            pthread_mutex_unlock(&count_mutex);

            const char * pData = data.data();
            iec104.ProtocolReceivedData(pData, data.size());
        }

        iec104.ProtocolTimerTimeOut(CxTime::currentMsepoch());
        sleep(5);
    }

    /* perform read write operations ... */

    shutdown(SocketFD, SHUT_RDWR);

    close(SocketFD);
    return 0;
}
