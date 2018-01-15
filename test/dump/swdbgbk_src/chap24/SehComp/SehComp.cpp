/*---------------------------------------------------------------------
SehComp.cpp - Sample to explore SEH compiling  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <windows.h>
int TrySeh(int n)
{
	__try
	{
		n=1/n;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		n=0x122;
	}
	return n;
}
int TryLevel(int n)
{
	__try{ // Try0
		n=n-1;
		__try{ // Try1
			n=1/n;
		}
		__except(EXCEPTION_CONTINUE_SEARCH){
			n=0x221;
		}
		n++;
		__try{ // Try2
			n=1/n;
		}
		__except(EXCEPTION_CONTINUE_SEARCH){
			n=0x222;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		n=0x220;
	}
	__try{ // Try3
		n=1/n;
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		n=0x223;
	}
	return n;
}

int main(int argc, char* argv[])
{
	printf("SEH Compile!\n");
	
	printf("TrySeh got %d\n", TrySeh(argc-1));
	printf("TrySeh got %d\n", TryLevel(argc-1));

	__try
	{
		argc=0;
		argc=1/argc;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		printf("A\n");
	}
	__try
	{
		argc=0;
		argc=1/argc;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		printf("B\n");
	}

	return 0;
}

