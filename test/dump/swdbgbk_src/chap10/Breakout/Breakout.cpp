/*---------------------------------------------------------------------
Breakout.cpp - Utility to call DbgUiRemoteBreakin() locally.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#include <windows.h>

typedef DWORD (WINAPI*NAPI_DbgUiRemoteBreakin)( LPVOID lpParameter);
int main(int argc, char* argv[])
{
    HINSTANCE hLib; 
    NAPI_DbgUiRemoteBreakin DbgUiRemoteBreakin; 
	int nRet=0;

	printf("Utility to call DbgUiRemoteBreakin() locally for experiment puspose only.\n--Raymond\n");
    // Get a handle to the DLL module.
    hLib = LoadLibrary("NTDLL.dll"); 
 
    // If the handle is valid, try to get the function address.
    if (hLib != NULL) 
    { 
        DbgUiRemoteBreakin = (NAPI_DbgUiRemoteBreakin)
			GetProcAddress(hLib, "DbgUiRemoteBreakin"); 
 
        // If the function address is valid, call the function.
         if (NULL != DbgUiRemoteBreakin) 
        {
			printf("DbgUiRemoteBreakin will be called at 0x%x.\n", DbgUiRemoteBreakin);
			nRet = DbgUiRemoteBreakin(0);
			// will never return here.
        }
 
        // Free the DLL module.
        FreeLibrary(hLib); 
    } 
	printf("Exit with %d.\n", nRet);
 	return nRet;
}

