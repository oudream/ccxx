#include <typeinfo>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <iso646.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>

using namespace std;

int f_argc = 0;
const char * * f_argv = NULL;

int main(int argc,const char * argv[])
{
    f_argc = argc;
    f_argv = argv;

    cout << "begin test xxx : " << endl;

    std::string sLine;
    std::getline (std::cin, sLine);

    cout << "this size :" << sLine.size() << endl;

    cout << "end test xxx !!!" << endl;

    return 0;
}















