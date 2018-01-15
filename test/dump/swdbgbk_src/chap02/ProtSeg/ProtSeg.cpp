/*---------------------------------------------------------------------
ProtSeg.cpp - Utility to watch protection of segment register.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	__asm
	{
		mov bx,25
		mov ds,bx
	}
	return 0;
}



