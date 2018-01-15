/*---------------------------------------------------------------------
SehRaw.cpp - Sample to register SEH handler manually.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <windows.h>

EXCEPTION_DISPOSITION
__cdecl _raw_seh_handler(
    struct _EXCEPTION_RECORD *ExceptionRecord,
    void * EstablisherFrame,
    struct _CONTEXT *ContextRecord,
    void * DispatcherContext )
{
    printf("_raw_seh_handler: code-0x%x, flags-0x%x\n",
		ExceptionRecord->ExceptionCode,
		ExceptionRecord->ExceptionFlags);

	if(ExceptionRecord->ExceptionCode==
		STATUS_INTEGER_DIVIDE_BY_ZERO)
	{
		ContextRecord->Ecx = 10;
		return ExceptionContinueExecution;
	}
	return ExceptionContinueSearch;
}

int main()
{
    __asm
    {                          
        push    OFFSET _raw_seh_handler 
        push    FS:[0]          
        mov     FS:[0],ESP      

		xor     edx, edx
		mov     eax, 100
        xor     ecx, ecx   // Zero out ECX
        idiv    ecx        // Divide EDX:EAX by ECX

        mov     eax,[ESP]       
        mov     FS:[0], EAX     
        add     esp, 8          
    }

	printf("SehRaw exits\n");
    return 0;
}