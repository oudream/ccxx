
#ifndef PROJECT_GLOBAL_H
#define PROJECT_GLOBAL_H

#include <typeinfo>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <stddef.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

using namespace std;

static void gprint(std::string msg)
{
    std::cout << msg << std::endl;
}

std::string ginput()
{
    std::string msg;
    std::cin >> msg;
    return msg;
}

inline void TestVector21()
{
    string str;
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

#endif //PROJECT_GLOBAL_H
