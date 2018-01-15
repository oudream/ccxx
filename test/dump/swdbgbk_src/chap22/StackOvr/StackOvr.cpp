/*---------------------------------------------------------------------
StackOvr.cpp - A buggy application which will consume all stack space.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#include <windows.h>
int g_nCount=0;
int g_ESP;
#pragma optimize("",off)
void deadloop()
{
	g_nCount++;
	char szMsg[512]="";
	_asm mov g_ESP,esp;
	deadloop();
}
#pragma optimize("",on)
int main(int argc, char* argv[])
{
	printf("Stack overflow example!\n");
	deadloop();
	return g_nCount;
}

