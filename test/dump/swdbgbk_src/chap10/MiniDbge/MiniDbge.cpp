/*---------------------------------------------------------------------
MiniDbge.cpp - Samplest Win32 Application to serve as debuggee.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <stdio.h>

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	int a=0,b=1;
	a+=b;
	printf("%d", a);
	OutputDebugString("This is mini debuggee.\n");
	return 0;
}



