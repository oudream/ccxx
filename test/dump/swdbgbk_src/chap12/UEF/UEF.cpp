/*---------------------------------------------------------------------
UEF.cpp : A brave console application to triger unhandled exceptions.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include <stdio.h>

int main(int argc, char* argv[])
{
	printf("Going to assign value to null pointer!\n");
	*(int*)0=1;
	return 0;
}

