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


#define BOOST_STRINGIZE(X) BOOST_DO_STRINGIZE(X)
#define BOOST_DO_STRINGIZE(X) #X

void print_macro(const char* name, const char* value)
{
    const int indent = 4;
    // if name == value+1 then then macro is not defined,
    // in which case we don't print anything:
    if(0 != strcmp(name, value+1))
    {
        for(unsigned i = 0; i < indent; ++i) std::cout.put(' ');
//        std::cout << std::setw(width);
        cout.setf(istream::left, istream::adjustfield);
        std::cout << name;
        if(value[1])
        {
            // macro has a value:
            std::cout << value << "\n";
        }
        else
        {
            // macro is defined but has no value:
            std::cout << " [no value]\n";
        }
    }
}

#define PRINT_MACRO(X) print_macro(#X, BOOST_STRINGIZE(=X))

#define GM_I1 7
#define GM_S1 "aa"

int main(int argc,const char * argv[])
{
    f_argc = argc;
    f_argv = argv;

    cout << "begin test xxx : " << endl;

    std::string sLine;
    std::getline (std::cin, sLine);

    cout << "this size :" << sLine.size() << endl;

    PRINT_MACRO(GM_I1);
    PRINT_MACRO(GM_S1);

    cout << "end test xxx !!!" << endl;
    return 0;
}
