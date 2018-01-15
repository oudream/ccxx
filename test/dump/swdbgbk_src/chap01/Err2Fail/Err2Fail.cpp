/*---------------------------------------------------------------------
Err2Fail.cpp - Sample to demo failures happen conditionally.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
int bad_div(int n,unsigned int m)
{
	return n/m;
}

int main(int argc, char* argv[])
{
	printf("%d/%d=%d!\n",6,3,bad_div(6,3));
	printf("%d/%d=%d!\n",-6,3,bad_div(-6,3));
	printf("%d/%d=%d!\n",-10,2,bad_div(-10,2));
	return 0;
}

void f()
{
	printf("never reach here.\n");
}