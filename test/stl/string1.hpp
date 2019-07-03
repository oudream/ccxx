
#include "global.h"

using namespace std;

void testString11()
{
    string s1 = "0123456789";

    cout << s1 << endl;

    string s2 = s1;
    char * ps2 = (char *)s2.c_str();
    for (int i = 0; i < s2.size(); ++i)
    {
        ps2[i] = 'a' + i;
    }
    cout << s1 << endl;
    cout << s2 << endl;
}