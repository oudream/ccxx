/*---------------------------------------------------------------------
Interop.cpp  - Target to explore using both SEH and CppEH.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <windows.h>

class CBug
{
public:
	CBug(int n) : m_nIndex(n)
	{
		printf("Bug %d constructed.\n", m_nIndex);
	}
	~CBug()
	{
		printf("Bug %d deconstructed.\n", m_nIndex);
	};
protected:
	int m_nIndex;
};
int FuncObjUnwind(int n)
{
	CBug bug0(0);
	try
	{
		printf("Going to n=1/%d.\n",n);
		n=1/n;
	}
	catch(...)
	{
		printf("Caught all.\n");
	}
	if(n>=0)
	{
		CBug bug1(1);
	}
	return n;
}

int main(int argc, char* argv[])
{
	__try
	{
		printf("FuncObjUnwind got %x!\n", FuncObjUnwind(argc-1));
	}
	__except(printf("Filter in main\n"),
		EXCEPTION_EXECUTE_HANDLER)
	{
		printf("Handling block in main.\n");
	}
	return 0;
}
