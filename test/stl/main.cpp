
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

int main(int argc, char *agrv[])
{
    gprint("stl begin:");

//    testClass11();

    testBool1();
    
    gprint("stl end!");
    return 1;
}
