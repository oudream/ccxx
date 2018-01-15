/*---------------------------------------------------------------------
StackChk.cpp - Sample to inspect stack check facilities of compiler.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"

#define BUFF_SIZE 1024*4
//#define BUFF_SIZE 1024*2
void Func()
{
	char sz[BUFF_SIZE];
	for(int i=0;i<BUFF_SIZE;i++)
		sz[i]=(i+1)%127;

	printf("%s\n",sz);
}
#pragma check_stack -
// Set /Gs102400 to C++ compiling options
// for the test of access violation caused
// by access cross over guard page.
void FuncVia()
{
	char sz[1024*98];
	sz[0]=66,sz[1]=0;

	printf("%s\n",sz);
}
#pragma check_stack()
void main()
{
	Func();
	FuncVia();
}
