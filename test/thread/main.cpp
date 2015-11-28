
#include <cxglobal.h>
#include <cxapplication.h>
#include <cxtime.h>

#include "cxTestThread1.cpp"

using namespace std;

#include <windows.h>

#include<stdio.h>

int main(int argc, char * argv[])
{
    CxApplication::init(argc, argv);

    int i = 0;
    while (1)
    {
        std::cout << CxTime::currentMsepochString();
        ::Sleep(1);
        i++;
        if (i==5000)
        {
            break;
        }
    }

    std::

    return 0;
}

