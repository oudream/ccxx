
//stl
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

using namespace std;

class A
{
public:
    void showa()
    {
        cout << "a->showa" << endl;
    }

};

class B : virtual public A
{
public:
    void showb()
    {
        cout << "b->showb" << endl;
    }

};

class C : virtual public A
{
public:
    void showc()
    {
        cout << "c->showc" << endl;
    }

};

class D : public B, public C
{
public:
    void showc()
    {
        cout << "c->showc" << endl;
    }

};

void test()
{
    D d1;
    d1.showa();
}

int main()
{
//    system("chcp 65001");
    system("chcp 936");

    cout << "begin test1" << endl;

    test();

    cout << "end test1" << endl;

    system("pause");
    return 0;
}
