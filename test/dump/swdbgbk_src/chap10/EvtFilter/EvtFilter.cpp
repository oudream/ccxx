/*---------------------------------------------------------------------
EvtFilter.cpp - Sample to test for Exception Options of VC6.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	int i,m=1,n;
	printf("Test for Exception Options of VC6!\n");
	getchar();
	
	__try
	{
		n=0;
		i=m/n;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		OutputDebugString("I got the exception.\n");
		return -1;
	}
	return 0;
}

