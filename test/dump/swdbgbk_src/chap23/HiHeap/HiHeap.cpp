/*---------------------------------------------------------------------
HiHeap.cpp - Debuggee to explore fundamental heap facilities.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <crtdbg.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
//#include <winheap.h>
//#include <dbgint.h>
 
//HeapAlloc
void EnumHeaps()
{
	DWORD dwTotal,dwHeapComp;
	HANDLE *phHeaps;
	
	dwTotal=GetProcessHeaps(0,NULL);
	if(dwTotal==0)
	{
TAG_ERROR:
		printf("GetProcessHeaps failed for %d.\n",
			GetLastError());
		return;
	}
	phHeaps=(PHANDLE)new HANDLE[dwTotal];
	dwTotal=GetProcessHeaps(dwTotal,phHeaps);
	if(dwTotal==0)
		goto TAG_ERROR;
	for(unsigned int i=0;i<dwTotal;i++)
	{
#if WINVER>=0x0501
		if(HeapQueryInformation(phHeaps[i],
			HeapCompatibilityInformation,
			&dwHeapComp,
			sizeof(DWORD),
			NULL))
		{
			printf("HeapCompatibilityInformation of Heap %8X is %d\n",
				phHeaps[i],dwHeapComp); 
		}
#endif
  }
	delete phHeaps;
}
void TestAlloc(BOOL bLeak)
{
	void * pStruct = HeapAlloc(GetProcessHeap(), 0, MAX_PATH);
	if(!bLeak)
		HeapFree(GetProcessHeap(),0, pStruct);
}
void TestNew(BOOL bLeak)
{
	char * lpsz=new char[2048];

	if(!bLeak)
		delete lpsz;
}
void TestMalloc(BOOL bLeak)
{
	void * p=malloc(5);
	
	if(!bLeak)
		free(p);
}
void TestAllocA(int n)
{
   char * buf = (char*)_alloca( n );
   // do something with buf
   //_freea( buf );
}
void TestMallocDbg(int n)
{
	char * buf=(char*)_malloc_dbg(10, 111, NULL, 0);
	strcpy(buf, "test");
}
void CheckMem()
{
#ifdef _DEBUG
	_CrtMemState s;
#endif
	_CrtMemCheckpoint(&s);
	_CrtMemDumpStatistics(&s);
}
void TestGlobal()
{
	HGLOBAL hMemGlobal=GlobalAlloc(0, 111);
	GlobalFree(hMemGlobal);
	HLOCAL hMemLocal=LocalAlloc(0,111);
	LocalFree(hMemLocal);
}
void TestVirtualAlloc(DWORD dwGranularity)
{
	ULONG ulSize=1<<16<<dwGranularity;
	PVOID pMem=HeapAlloc(GetProcessHeap(),0,ulSize);
	if(IsDebuggerPresent())
		DebugBreak();
	HeapFree(GetProcessHeap(),0, pMem);
}
// do allocations so that grow heap with more segments
void TrigerMulSegment()
{
	char c=0;
	PVOID pMem;
	ULONG ulSize=0xf000*8; // about 480KB
	// should be less than 0xfe00*8 to avoid virtal alloc directly
	while(c!='b')
	{
		pMem=HeapAlloc(GetProcessHeap(),0,ulSize);
		printf("Allocated %d at 0x%x. Enter 'b' to abort\n",
			ulSize, pMem);
		// there will be memory leak here, anyway...
		c=getchar();
	}
}

void TestDecommit(ULONG ulSize)
{
	printf("Any key to alloc %d bytes on heap.\n", ulSize);
	getchar();
	PVOID pMem=HeapAlloc(GetProcessHeap(),0,ulSize);
	printf("Allocate memroy at 0x%x, any key to free it.\n",pMem);
	getchar();
	HeapFree(GetProcessHeap(),0,pMem);
	printf("Memroy is freed, any key to continue.\n");
	getchar();
}
int help()
{
	printf("Debuggee to explore heap by Raymond\n" \
		"hiheap <cmd letter> [para]\n" \
		"cmd letters:\n" \
		"v - Virtual Allocation\n" \
		"g - GlobalAlloc and LocalAlloc\n" \
		"d - Decommit\n" \
		"s - Grow to multiple segments\n" \
		"a - alloca and HeapAlloc\n" \
		"n - new\n" \
		"m - malloc\n" \
		"b - bad block type\n" \
		"c - check memory\n");
	return -1;
}
int main(int argc, char* argv[])
{
	SYSTEM_INFO sSysInfo;           // useful system information

	GetSystemInfo(&sSysInfo);  // fill the system information structure

	printf("Page Size=%d, Granularity=%d\n",
		sSysInfo.dwPageSize,
		sSysInfo.dwAllocationGranularity);

	EnumHeaps();
	if(argc<2)
	{
		return help();
	}
	switch(argv[1][0])
	{
	case 'v':
		TestVirtualAlloc(8);
		break;
	case 'g':
		TestGlobal();
		break;
	case 'd':
		TestDecommit(argc>2?atoi(argv[2]):0x1008);
		break;
	case 's':
		TrigerMulSegment();
		break;
	case 'a':
		TestAlloc(FALSE);
		TestAllocA(FALSE);
		break;
	case 'n':
		TestNew(FALSE);
		break;
	case 'm':
		TestMalloc(TRUE);
		TestMallocDbg(FALSE);
		break;
	case 'c':
		CheckMem();
		break;
	default:
		printf("bad command %s\n",argv[1]);
	}
	_CrtDumpMemoryLeaks();
	return 0;
}

