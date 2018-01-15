/*---------------------------------------------------------------------
HiStack.cpp - A simple application for inspecting stack.
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include <stdio.h>

int __stdcall Proc(int n)
{
	int a=n;
	printf("A test to inspect stack, n=%d,a=%d.",n,a);
	return n*a;
}
int main()
{
	return Proc(122);
}
