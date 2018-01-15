/*---------------------------------------------------------------------
MemLeak.cpp - Sample to dump memory leak using CRT's debug heap.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
//#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <malloc.h>
#ifdef _DEBUG
  #define MYDEBUG_NEW   new( _NORMAL_BLOCK, __FILE__, __LINE__)
  #define new MYDEBUG_NEW
#endif// _DEBUG

int main(int argc, char* argv[])
{
	char * p1,*p2;
	printf("Sample to demo memory leak checking of RTC by Raymond!\n");
#ifdef _DEBUG
	_crtDbgFlag|=_CRTDBG_LEAK_CHECK_DF;// _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
#endif

	p1=(char *)new char[20];
	p2=(char *)malloc(100);

	return 0;
}

