/*---------------------------------------------------------------------
TryInt1.cpp - Sample to see effect of executing INT 1 in user land.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"

int main(int argc, char* argv[])
{
	printf("Try INT 1!\n");
	_asm int 1;
	return 0;
}

