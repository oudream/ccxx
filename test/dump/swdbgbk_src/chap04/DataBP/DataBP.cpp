/*---------------------------------------------------------------------
// DataBP.cpp : Demonstrate setting data access breakpoint manually.
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	CONTEXT cxt;
	HANDLE hThread=GetCurrentThread();
	DWORD dwTestVar=0;

	if(!IsDebuggerPresent())
	{
		printf("This sample can only run within a debugger.\n");
		return E_FAIL;
	}
	cxt.ContextFlags=CONTEXT_DEBUG_REGISTERS|CONTEXT_FULL;
	if(!GetThreadContext(hThread,&cxt))
	{
		printf("Failed to get thread context.\n");
		return E_FAIL;
	}
	cxt.Dr0=(DWORD) &dwTestVar;
	cxt.Dr7=0xF0001;//4 bytes length read& write breakponits
	if(!SetThreadContext(hThread,&cxt))
	{
		printf("Failed to set thread context.\n");
		return E_FAIL;
	}

	dwTestVar=1;
	GetThreadContext(hThread,&cxt);
	printf("Break into debuger with DR6=%X.\n",cxt.Dr6);

	return S_OK;
}

