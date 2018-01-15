/*---------------------------------------------------------------------
DbgString.cpp - Sample to test existence of DBWinMutex.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	printf("Program to test existence of DBWinMutex object.\n");
	BOOL bDebuggerPresent=IsDebuggerPresent();

	if(!bDebuggerPresent)
	{
		printf("Inspect this process now to verify no DBWinMutex.\n");
		getchar();
		OutputDebugString(argv[0]);
		printf("Inspect this process now to verify DBWinMutex exists.\n");
		getchar();
	}
	else
	{
		OutputDebugString(argv[0]);
		printf("Inspect this process now to verify no DBWinMutex.\n");
		getchar();
	}
	return 0;
}

