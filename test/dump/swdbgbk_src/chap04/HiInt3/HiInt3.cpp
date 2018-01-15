// HiInt3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	// manual breakpoint 
	_asm INT 3;
	printf("Hello INT 3!\n");

	_asm
	{
		mov eax,eax
		__asm _emit 0xcd __asm _emit 0x03 
		nop 
		nop
	}
	//or use Windows API 
	DebugBreak();
	//
	return 0;
}

