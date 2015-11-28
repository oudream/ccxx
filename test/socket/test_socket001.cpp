#include <iostream>
#include <Winsock2.h>
#include <vector>

using namespace std;

vector<string> getLocalIpAddresses()
{
    vector<string> r;

    WORD wVersionRequested = MAKEWORD(2, 2);

    WSADATA wsaData;
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
        return r;

    char local[255] = {0};
    gethostname(local, sizeof(local));
    hostent* ph = gethostbyname(local);
    if (ph == NULL)
        return r;



    in_addr addr;
    for (size_t i = 0; i < 255; ++i)
    {
        if (ph->h_addr_list[i])
        {
            memcpy(&addr, ph->h_addr_list[i], sizeof(in_addr)); // 这里仅获取第一个ip
            string localIP;
            localIP.assign(inet_ntoa(addr));
            r.push_back(localIP);
        }
        else
        {
            break;
        }
    }

    WSACleanup();
    return r;
}


int main()
{
    vector<string> sLocalIps = getLocalIpAddresses();
    for (size_t i = 0; i < sLocalIps.size(); ++i)
    {
        cout << sLocalIps.at(i) << endl;
    }

//    char date[100];
//    strcpy (date, const_cast<char*>(GetLocalIpAddress().c_str()));
//    cout<<date<<endl;
    system("PAUSE");
}
