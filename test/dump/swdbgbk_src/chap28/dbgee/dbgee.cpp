// dbgee.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

TCHAR g_szGlobal[]=_T("A global var.");

int _tmain(int argc, _TCHAR* argv[])
{
    int nRet=0;
    TCHAR szBuffer[MAX_PATH];

    printf("dbgee enters main.\n");
    if(argc==1)
    {
        *(int *)0=1;
        printf("test\n");
        nRet=-1;
    }
    for(int i=0;i<argc;i++)
    {
        _stprintf(szBuffer,_T("Arg [%d]: %s"), i, argv[i]);
        _tprintf(szBuffer);
    }
    printf("dbgee exits.\n");
	return nRet;
}

