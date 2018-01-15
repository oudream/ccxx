/*---------------------------------------------------------------------
ChkHFC.cpp - Sample to see demo free heap checking of Win32 Heap.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	void * p;
	BOOL bRet;
	HANDLE hHeap;

	printf("Heap Free Check (HFC)!\n");
	hHeap=HeapCreate(0, 4096, 0);
	p = HeapAlloc(hHeap, 0, 20);
	bRet=HeapFree(hHeap, 0, p);
	printf("Free pointer p first time, %d\n", bRet);
	bRet=HeapFree(hHeap, 0, p);
	printf("Free pointer p second time, %d\n", bRet);
	bRet=HeapValidate(hHeap, 0, NULL); // it may hang
	printf("HeapValidate returned %d\n", bRet);
	bRet=HeapDestroy(hHeap);
	printf("HeapDestroy returned %d\n", bRet);
	return getchar();
}

