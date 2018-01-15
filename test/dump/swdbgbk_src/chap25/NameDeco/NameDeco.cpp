/*---------------------------------------------------------------------
NameDeco.cpp - Target to explore name decoration of VC compiler. 
This application could not pass link by design to see the decorated name. 
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
class CTest
{
public:
	CTest();
	~CTest();
	void * operator new(unsigned int n);
	void operator delete(void*p);
	CTest& operator ++(int n);
	CTest& operator =(int n);
	int SetName(char * sz, ...)const;
};
int __cdecl TestFunc(int n,int m);
int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	CTest* pTest=new CTest;

	pTest->SetName("");

	TestFunc(0,1);
	return 0;
}

