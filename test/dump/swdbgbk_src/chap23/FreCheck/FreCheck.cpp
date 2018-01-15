/*---------------------------------------------------------------------
FreCheck.cpp - Target to watch heap checking trigered by free operation.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	char * p;
	HANDLE hHeap;
	hHeap = HeapCreate(0, 1024, 0);
	p=(char*)HeapAlloc(hHeap, 0, 9);
	for(int i=0;i<20; i++)
		*(p+i)=i;
	
	if(!HeapFree(hHeap, 0, p))
		printf("Free %x from %x failed.", p, hHeap);

	if(!HeapDestroy(hHeap))
		printf("Destroy heap %x failed.", hHeap);

	printf("Exit with 0");
	return 0;
}


