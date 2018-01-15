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

//clib
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

#include <windows.h>

using namespace std;

string toString(double d)
{
    ostringstream out;
    out.precision(15);//覆盖默认精度
    out<<d;
    string str= out.str(); //从流中取出字符串 数值现在存储在str中，等待格式化。
    return str;
}

std::string ginput()
{
    std::string msg;
    std::cin >> msg;
    return msg;
}

int * fn_oConsoleInputThreadInit()
{
    static int m = 0;
    return &m;
}

int main()
{
    TerminateProcess(1, SIGINT);

    cout << "Hello World!" << endl;

    double d = 2002005706;

    stringstream ss;
    ss << d;
    string s(ss.str());

    cout << s << endl;

    double d2 = strtod(s.c_str(), NULL);
    cout << d2 << endl;

    std::ostringstream strs;
    strs << d;
    std::string str = strs.str();

    cout << str << endl;

    cout << toString(double(2002005706)) << endl;
    cout << toString(double(20020057061)) << endl;
    cout << toString(double(200200570612)) << endl;
    cout << toString(double(2002005706123)) << endl;
    cout << toString(double(20020057061234)) << endl;
    cout << toString(double(2002005706.1234)) << endl;
    cout << toString(double(1.234567897879)) << endl;

    int * pNum = fn_oConsoleInputThreadInit();
    cout << * pNum << endl;
    * pNum |= 10;
    cout << * fn_oConsoleInputThreadInit() << endl;

    cout << ginput() << endl;

    return 0;
}

