#include <ccxx/ccxx.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#pragma comment (lib, "Ws2_32.lib")
int __cdecl main(int argc, char **argv)
{
    int iResult;
    INT iRetval;
    DWORD dwRetval;
    int i = 1;
    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    struct addrinfo hints;
    struct sockaddr_in  *sockaddr_ipv4;
    LPSOCKADDR sockaddr_ip;
    char ipstringbuffer[46];
    DWORD ipbufferlength = 46;
    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed: %d/n", iResult);
        return 1;
    }
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    char host_name[256] = {0};
    iResult = gethostname(host_name, 256);
    if (iResult != 0)
    {
        WSACleanup();
        return false;
    }
    //std::cout << "/nhost_name:/t" << host_name << std::endl;
    dwRetval = ::getaddrinfo(host_name, NULL, &hints, &result);
    if ( dwRetval != 0 )
    {
        printf("getaddrinfo failed with error: %d/n", dwRetval);
        WSACleanup();
        return 1;
    }
    printf("getaddrinfo returned success/n");
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next)
    {
        printf("getaddrinfo response %d/n", i++);
        //printf("/tFlags: 0x%x/n", ptr->ai_flags);
        printf("/tFamily: ");
        switch (ptr->ai_family) {
            case AF_UNSPEC:
                printf("Unspecified/n");
                break;
            case AF_INET:
                printf("AF_INET (IPv4)/n");
                sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
                printf("/tIPv4 address %s/n",
                    inet_ntoa(sockaddr_ipv4->sin_addr) );
                break;
            case AF_INET6:
                printf("AF_INET6 (IPv6)/n");

                sockaddr_ip = (LPSOCKADDR) ptr->ai_addr;
                ipbufferlength = 46;
                iRetval = WSAAddressToString(sockaddr_ip, (DWORD) ptr->ai_addrlen, NULL,
                    ipstringbuffer, &ipbufferlength );
                if (iRetval)
                    printf("WSAAddressToString failed with %u/n", WSAGetLastError() );
                else
                    printf("/tIPv6 address %s/n", ipstringbuffer);
                break;
            case AF_NETBIOS:
                printf("AF_NETBIOS (NetBIOS)/n");
                break;
            default:
                printf("Other %ld/n", ptr->ai_family);
                break;
        }
    }
    freeaddrinfo(result);
    WSACleanup();
    return 0;
}
