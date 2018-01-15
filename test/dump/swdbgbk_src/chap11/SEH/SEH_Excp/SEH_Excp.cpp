/*---------------------------------------------------------------------
SEH_Excp.cpp - Sample to explore exception handling of SEH.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#include <excpt.h>
#include <windows.h>
#include <stdlib.h>

char g_szDefPara[]="0123456789";
int ExcptionFilter(LPEXCEPTION_POINTERS pException,char**ppPara)
{
	PEXCEPTION_RECORD pER=pException->ExceptionRecord;
	PCONTEXT pContext=pException->ContextRecord;

	printf("Exception Info: code=%08X, addr=%08X, flags=%X.\n",
		pER->ExceptionCode, pER->ExceptionAddress,
		pER->ExceptionFlags);
	
	printf("Context Info: EIP=%08X, EAX=%08X.\n",
		pContext->Eip, pContext->Eax);

	if(*ppPara==NULL && pER->ExceptionCode==STATUS_ACCESS_VIOLATION)
	{
		*ppPara=g_szDefPara;
		pContext->Eip-=3;
		printf("New EIP=%08X and *ppPara=%s.\n",
			pContext->Eip,*ppPara);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
		
	return EXCEPTION_EXECUTE_HANDLER;
}
void FuncA(char* lpsz)
{
	printf("Entering FuncA with lpsz=%s.\n",lpsz);
	__try
	{
		*lpsz='2';
	}
	__except(ExcptionFilter(GetExceptionInformation(),&lpsz))
	{
		printf("Exexcuting handling block in FuncB.\n");
	}
	printf("Exiting from FuncA with lpsz=%s.\n",lpsz);
}
void SehPrint(char * szName,LPEXCEPTION_POINTERS pException)
{
	PEXCEPTION_RECORD pER=pException->ExceptionRecord;
	printf("%s:%x\n",szName,pER->ExceptionFlags);
}
int FuncB(int nPara)
{
	printf("Entering FuncB with Para=%d.\n",nPara);
	__try
	{
		nPara=1/nPara;

		*(int*)0=1;
	}
	__except(SehPrint("FuncB",GetExceptionInformation()),
		GetExceptionCode()==EXCEPTION_ACCESS_VIOLATION?
		EXCEPTION_EXECUTE_HANDLER:EXCEPTION_CONTINUE_SEARCH)
	{
		printf("Executing handling block in FuncB [%X].\n",
			GetExceptionCode());
	}
	printf("Exiting from FuncB with Para=%d.\n",nPara);
	return nPara;
}
int main(int argc, char* argv[])
{
	int nRet=0;

	FuncA(argv[1]);

	__try
	{
		nRet=FuncB(argc-1);
	}
	__except(SehPrint("main",GetExceptionInformation()),EXCEPTION_EXECUTE_HANDLER)
	{
		printf("Executing exception handling block in main [%X].\n",
			GetExceptionCode());
	}

	printf("Exit from main with nRet=%d.\n",nRet);
	return nRet;
}

