/*---------------------------------------------------------------------
BufOvr.cpp - A buggy application with buffer overflow.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#include <string.h>
 
int main(int argc, char* argv[])
{
	char szInput[5];
	printf("Enter string:\n");
	gets(szInput);
	printf("You Entered:\n%s\n",szInput);
	return 0;
}

