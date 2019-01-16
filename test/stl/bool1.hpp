
#include "global.h"

void testBool1()
{
    bool bs[15];
    for (int i = 0; i < 15; ++i)
    {
        bs[i] = i % 2 == 0 ? true : false;
    }
    unsigned char buf[64];
    unsigned char * p = (unsigned char * )bs;
    for (int j = 0; j < 15; ++j)
    {
        buf[j] = * p;
        p++;
    }
    cout << buf[8] << endl;
}