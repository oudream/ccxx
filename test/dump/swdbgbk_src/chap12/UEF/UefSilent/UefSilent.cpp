/*---------------------------------------------------------------------
UefSilent.cpp - Disable GPF dialog by setting error mode.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	int nOldMode;
	printf("Hello World!\n");
	if(argc<=1)
		nOldMode=SetErrorMode(SEM_NOGPFAULTERRORBOX);//0x0002

	*(int*)0=1;

	printf("Never reach here!\n");
	return 0;
}

