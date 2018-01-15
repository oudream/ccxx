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

extern std::string ginput ();

int test_memery1(int argc, char *argv[])
{
    cout << "test memery begin" << endl;

    string str;
    do
    {
        str = ginput();

        if (str[0] == 't')
        {
            if (str[1] == '1')
            {
                char data1[1000];
                cout << data1[0] << ' ' << data1[1] << ' ' << data1[2] << ' ' << data1[3] << endl;
                int * pi1 = (int *)data1;
                int & i1 = * pi1;
                int i2 = 0x31323334;
                i1 = i2;
                cout << data1[0] << ' ' << data1[1] << ' ' << data1[2] << ' ' << data1[3] << endl;
                i1 = 0x61626364;
                cout << data1[0] << ' ' << data1[1] << ' ' << data1[2] << ' ' << data1[3] << endl;
            }
            else if (str[1] == '2')
            {
                int i1 = 0x31323334;
                char * pData = (char *)(&i1);
                cout << pData[0] << ' ' << pData[1] << ' ' << pData[2] << ' ' << pData[3] << ' ' << endl;
            }
            else if (str[1] == '3')
            {
            }
        }
        else
        {
        }
    }
    while (str != "cc");

    cout << "test wd1 end." << endl;

    return 0;
}
