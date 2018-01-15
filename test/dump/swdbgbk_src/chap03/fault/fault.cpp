/*---------------------------------------------------------------------
Fault.cpp - Fix code fault using SEH and continue execution.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include <stdio.h>
#include <windows.h>

#define VAR_WATCH() printf("nDiviedend=%d, nDivisor=%d, nResult=%d.\n", \
			nDividend,nDivisor,nResult)

int main(int argc, char* argv[])
{
	int nDividend=22,nResult=100;
    int nDivisor=0;

	__try
	{
		printf("Before div in __try block:");
		VAR_WATCH();

        // nResult=nDividend / nDivisor;
        __asm{
            mov eax, nDividend
            cdq
            idiv nDivisor 
            mov nResult, eax
        };

		printf("After div in __try block:");
		VAR_WATCH();
	}
	__except(printf("In __except block:"),VAR_WATCH(),
		GetExceptionCode()==EXCEPTION_INT_DIVIDE_BY_ZERO?
		(nDivisor=1,
			printf("Divide Zero exception detected:"), VAR_WATCH(),
			EXCEPTION_CONTINUE_EXECUTION):
		EXCEPTION_CONTINUE_SEARCH)
	{
		printf("In handler block.\n");
	}

	return getchar();
}

