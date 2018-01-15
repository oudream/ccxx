/*---------------------------------------------------------------------
AllcStk.cpp : This application has two usage.
 1) Debugging targets to trace how the BaseCreateStack functions works.
 2) Triger a stack overflow to verify where the overflow occurs.
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#include <stdio.h>
int deadloop(int n)
{
	int m=100;
	while(n==m)
		deadloop(n);
	return -1;
}
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

 	if(!CreateProcess(NULL,lpCmdLine,
		NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
	{
		TCHAR szMsg[MAX_PATH];
		sprintf(szMsg,"Failed create process [%s] with %d!",lpCmdLine,GetLastError());
		MessageBox(NULL,szMsg,"",MB_OK);
	}

	deadloop(100);
	return 0;
}



