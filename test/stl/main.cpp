
#include "global.h"

#include "class1.hpp"

#include "bool1.hpp"


void gprint(std::string msg)
{
    std::cout << msg << std::endl;
}

std::string ginput()
{
    std::string msg;
    std::cin >> msg;
    return msg;
}

void TestVector21()
{
    std::string str;
    do
    {
        str = ginput();

        if (str[0] == 'C')
        {
            if (str[1] == 'B')
            {
                if (str[2] == '0')
                {

                }
            }
            else if (str[1] == 'E')
            {
            }
            else if (str[1] == 'T')
            {
            }
            else if (str[1] == 'R')
            {
            }
            else if (str[1] == 'C')
            {
                break;
            }
        }
        else
        {
        }
    } while (str != "CC");
}

bool isValid()

int main(int argc, char *agrv[])
{
    gprint("stl begin:");

//    testClass11();

//    testBool1();

    int ns[4] = {7,8,9,12};
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int k = 0; k < 4; ++k)
            {

            }
        }
    }
    for (int i = 0; i < 4; ++i)
    {

    }

http_proxy="http://10.31.58.125:1080"
HTTP_PROXY="http://10.31.58.125:1080"
https_proxy="https://10.31.58.125:1080"
HTTPS_PROXY="https://10.31.58.125:1080"
ftp_proxy="socks5://10.31.58.125:1080"
FTP_PROXY="socks5://10.31.58.125:1080"
socks_proxy="socks5://10.31.58.125:1080"
SOCKS_PROXY="socks5://10.31.58.125:1080"
all_proxy="socks5://10.31.58.125:1080"
ALL_PROXY="socks5://10.31.58.125:1080"
no_proxy="127.0.0.1,127.0.1.1,localhost,192.168.0.*,192.168.1.*,192.168.99.*,10.31.58.*,10.32.50.*,10.31.16.*,10.30.0.*,hadoop-master,hadoop-slave1,hadoop-slave2,vbox-ubuntu1,vbox-ubuntu2,vbox-ubuntu3,::1"
NO_PROXY="127.0.0.1,127.0.1.1,localhost,192.168.0.*,192.168.1.*,192.168.99.*,10.31.58.*,10.32.50.*,10.31.16.*,10.30.0.*,hadoop-master,hadoop-slave1,hadoop-slave2,vbox-ubuntu1,vbox-ubuntu2,vbox-ubuntu3,::1"

10.31.58.39  vbox-ubuntu1


Acquire::http::proxy "http://10.31.58.125:1080/";
Acquire::ftp::proxy "ftp://10.31.58.125:1080/";
Acquire::https::proxy "https://10.31.58.125:1080/";

curl -x 10.31.58.125:1080 -fsSL "https://get.docker.com/" | sh
curl -x socks5://socks5://127.0.0.1:1080 -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo curl -x socks5://10.31.58.125:1080 -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository \
"deb [arch=amd64] https://download.docker.com/linux/ubuntu \
$(lsb_release -cs) \
stable"



    gprint("stl end!");
    return 1;
}
