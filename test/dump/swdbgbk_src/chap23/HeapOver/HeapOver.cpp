/*---------------------------------------------------------------------
HeapOver.cpp - Sample to demo heap buffer overflow.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	char * p1,*p2;
	HANDLE hHeap;
	hHeap = HeapCreate(0, 1024, 0);
	p1=(char*)HeapAlloc(hHeap, 0, 9);
	for(int i=0;i<50; i++)
		*p1++=i;
	
	p2=(char*)HeapAlloc(hHeap, 0, 1);
	printf("Allocation after overflow got 0x%x\n",p2);
	HeapDestroy(hHeap);
	return 0;
}

