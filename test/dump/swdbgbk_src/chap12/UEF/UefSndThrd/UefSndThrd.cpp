// UefSndThrd.cpp : Demonstrate Unhandled Exceptions in non-startup thread.
// Raymond May 1st 2006 
//

#include <windows.h>
#include <stdio.h>

DWORD WINAPI UefThreadProc(
  LPVOID lpParameter
)
{
	int n=0;
	printf("divide by zero %d...\n",n);
	n=1/n;
	printf("should never reach here %d...\n",n);
	// GetExceptionCode STATUS_ACCESS_VIOLATION
	return S_OK;
}

int main(int argc, char* argv[])
{
	//printf("Any key to continue!\n");
	//_asm int 0x2e STATUS_ACCESS_VIOLATION
	CreateThread(NULL,0,UefThreadProc,
		NULL,0,NULL);
	getchar();
	return 0;
}