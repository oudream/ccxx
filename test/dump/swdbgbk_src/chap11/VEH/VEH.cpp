// VEH.cpp : Code to demonstrate Vectored Exception Handling.
// Raymond April 22th, 2006
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct _VEH_REGISTRATION
{
	_VEH_REGISTRATION* next;
	_VEH_REGISTRATION* prev;
	PVECTORED_EXCEPTION_HANDLER pfnVeh;
} VEH_REGISTRATION, * PVEH_REGISTRATION;

#define TRACE(szWhere,ExceptionInfo) \
	printf("Exceuting %s: code=%X, flags=%X\n",szWhere,\
	ExceptionInfo->ExceptionRecord->ExceptionCode,\
	ExceptionInfo->ExceptionRecord->ExceptionFlags)

#define SHOW_VEH(v) printf("Node [%X]: next=%8X, prev=%8X, PFN=%X.\n",\
	v, v->next, v->prev, v->pfnVeh)

LONG WINAPI VEH1(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	TRACE("VEH1",ExceptionInfo);
    return EXCEPTION_CONTINUE_SEARCH;
}

LONG WINAPI VEH2(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	TRACE("VEH2",ExceptionInfo);
    PCONTEXT Context = ExceptionInfo->ContextRecord;

    Context->Eip++;
    return EXCEPTION_CONTINUE_EXECUTION;
}
LONG WINAPI VEH3(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	TRACE("VEH3",ExceptionInfo);
    return EXCEPTION_CONTINUE_SEARCH;
}

void VehTest1()
{
    PVOID h1,h2,h3;
     
    h2 = AddVectoredExceptionHandler(1,VEH2);
    h3 = AddVectoredExceptionHandler(0,VEH3);
	h1 = AddVectoredExceptionHandler(1,VEH1);
	SHOW_VEH(((PVEH_REGISTRATION)h1));
	SHOW_VEH(((PVEH_REGISTRATION)h2));
	SHOW_VEH(((PVEH_REGISTRATION)h3));
    _asm {cli};
    RemoveVectoredExceptionHandler(h1);
    RemoveVectoredExceptionHandler(h2);
    RemoveVectoredExceptionHandler(h3);
}

void VehTest2()
{
    PVOID h1;
     
    h1 = AddVectoredExceptionHandler(1,VEH1);

    __try 
    {
	    _asm {int 1};
    }
    __except(TRACE("SEH Filter in VehTest2",
		((PEXCEPTION_POINTERS)GetExceptionInformation())),
		EXCEPTION_EXECUTE_HANDLER)
    {
        printf("Eexcuting SEH handling block in VehTest2.\n");
    }

    RemoveVectoredExceptionHandler(h1);
}

void main( )
{
	printf("VehTest1:\n");
    VehTest1();
	printf("VehTest2:\n");
    VehTest2();
}
