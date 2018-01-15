// Assert.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <crtdbg.h>
#include <memory.h>
#include <assert.h>

void Foo(char * lpsz)
{
	_ASSERTE(lpsz!=NULL);
	//...
}
#define BIG_ARRAY 2048
int BadAssert()
{
	char * lpsz=new char[BIG_ARRAY];
	_ASSERT(lpsz!=NULL);

	memset(lpsz,0,BIG_ARRAY);
	//...
	delete [] lpsz;
	return 1;
}
void ansi_assert(char * p)
{
	assert(p!=NULL);

	if(p!=NULL)
		*p='a';
}

int main(int argc, char* argv[])
{
	printf("Hello Assert!\n");
	//_CrtSetReportHook( AdvReportFunction );

	_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_DEBUG);


	Foo(argv[1]);

	ansi_assert(argv[0]);

	_RPT1(_CRT_WARN, "The application will exit with %d\n",1);
	getchar();
	return 0;
}

