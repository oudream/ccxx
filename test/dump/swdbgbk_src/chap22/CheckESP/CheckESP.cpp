/*---------------------------------------------------------------------
CheckESP.cpp - A buggy application which does not obey stack balance.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"

void BadEsp()
{
	_asm push eax;
}

int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	BadEsp();
	return 0;
}

