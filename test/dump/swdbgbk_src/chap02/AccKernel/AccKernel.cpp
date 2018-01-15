/*---------------------------------------------------------------------
AccKernel.cpp - Sample to see effect of access kernel from user land.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"

int main(int argc, char* argv[])
{
	printf("Hi, I want to access kernel space!\n");
	*(int *)0xA0808080=0x22;

	printf("I would never reach so far!\n");
	return 0;
}

