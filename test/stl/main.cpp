
#include "global.h"

#include "class1.hpp"

#include "bool1.hpp"

#include "random1.hpp"

#include "string1.hpp"

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

//    testBool1();

//    testRandom1(argc, agrv);
//    testRandom2(argc, agrv);
//    testRandom3(argc, agrv);

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

    printf("begin test signal-lost1:\n");
    fflush(stdout);

    while (1) {

    }
    testString11();

    gprint("stl end!");
    return 1;
}

