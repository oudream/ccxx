
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
    int ia;
    char ca;
public:
    A()
            :ia(0)
            , ca('A')
    {}

    virtual void f()
    {
        cout << "A::f()" << endl;
    }

    virtual void Bf()
    {
        cout << "A::Af()" << endl;
    }
};

class B1:virtual public A
{
public:
    int ib1;
    char cb1;
public:
    B1()
            :ib1(1)
            , cb1('1')
    {}
    virtual void f()
    {
        cout << "B1::f()" << endl;
    }

    virtual void f1()
    {
        cout << "B1::f1()" << endl;
    }

    virtual void B1f()
    {
        cout << "B1::B1f()" << endl;
    }

};

class B2:virtual public A
{
public:
    int ib2;
    char cb2;
public:
    B2()
            :ib2(2)
            , cb2('2')
    {}

    virtual void f()
    {
        cout << "B2::f()" << endl;
    }

    virtual void f2()
    {
        cout << "B2::f2()" << endl;
    }

    virtual void B2f()
    {
        cout << "B2::B2f()" << endl;
    }

};

class D :public B1,public B2
{
public:
    int id;
    char cd;
public:
    D()
            :id(3)
            , cd('D')
    {}

    virtual void f()
    {
        cout << "D::f()" << endl;
    }

    virtual void f1()
    {
        cout << "D::f1()" << endl;
    }

    virtual void f2()
    {
        cout << "D::f2()" << endl;
    }

    virtual void Df()
    {
        cout << "D::Df()" << endl;
    }

};

typedef void(*Fun)();
void PrintVTable(int* VTable)
{
    cout << " 虚表地址>" << VTable << endl;

    if ((int)VTable < 0xff) return;

    for (int i = 0; VTable[i] > 0xff; ++i)
    {
        printf(" 第%d个虚函数地址 :0X%x,->", i, VTable[i]);
        Fun f = (Fun)VTable[i];
        f();
    }
}


void test()
{
    A a;
    B1 b1;
    B2 b2;
    D d1;

    cout << "sizeof(A)::" << sizeof(a) << endl;
    cout << "sizeof(B1)::" << sizeof(b1) << endl;
    cout << "sizeof(B2)::" << sizeof(b2) << endl;
    cout << "sizeof(D)::" << sizeof(d1) << endl;

    int* VTable = (int*)(*(int*)&d1);
    PrintVTable(VTable);
    cout << "        虚基表指针->: " << (int*)((int*)&d1 + 1) << endl;
    cout << "         B1::ib1 = " << *(int*)((int*)&d1 + 2) << endl;
    cout << "         B1::cb1 =" << (char)*((int*)&d1 + 3) << endl;

    VTable = (int*)*((int*)&d1 + 4);
    PrintVTable(VTable);
    cout << "        虚基表指针->:" << (int*)((int*)&d1 + 5) << endl;
    cout << "         B2::ib2 =" << *(int*)((int*)&d1 + 6) << endl;
    cout << "         B2::cb2 =" << (char)*((int*)&d1 + 7) << endl;

    cout << "         D::ID =" << *((int*)&d1 + 8) << endl;
    cout << "         D::cd =" << (char)*((int*)&d1 + 9) << endl;
    cout << " 虚基表的偏移地址->:"<<(int*)((int*)&d1 + 10) << endl;
    VTable = (int*)*((int*)&d1 + 11);
    PrintVTable(VTable);
    cout << "         A::ia =" << *(int*)((int*)&d1 + 12) << endl;
    cout << "         A::ca =" << (char)*((int*)&d1 + 13) << endl;

}

int main()
{
//    system("chcp 65001");
    system("chcp 936");
    test();
    system("pause");
    return 0;
}
