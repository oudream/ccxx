// callcv64.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define BUFF_SIZE 1024*4
void func()
{
	char sz[BUFF_SIZE];
	for(int i=0;i<BUFF_SIZE;i++)
	{
		sz[i]=i%127;
	}
	printf("%s",sz);
}

int _tmain(int argc, _TCHAR* argv[])
{
	func();
	return 0;
}

