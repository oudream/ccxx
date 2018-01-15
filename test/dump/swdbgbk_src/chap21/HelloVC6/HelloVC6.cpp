// HelloVC6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
class Cat
{
public:
	Cat()
	{
		MessageBox(NULL,"Cat()", "", MB_OK);
	}
};
Cat c;
int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	return 0;
}
int PreMain1(void)
{
   MessageBox(NULL, TEXT("First Pre-Main Function"),
                    TEXT("PreMain1"), 0);
   return 0;
}

int PreMain2(void)
{
   MessageBox(NULL, TEXT("Second Pre-Main Function"),
                    TEXT("PreMain2"), 0);
   return 0;
}

int PostMain1(void)
{
   MessageBox(NULL, TEXT("First Post-Main Function"),
                    TEXT("PostMain1"), 0);
   return 0;
}

int PostMain2(void)
{
   MessageBox(NULL, TEXT("Second Post-Main function"),
                    TEXT("PostMain2"), 0);
   return 0;
}
int myinit()
{
   MessageBox(NULL, "my init function test", "_initterm ", 0);
   return 0;
}

typedef int cb(void);

#pragma data_seg(".CRT$XIU")
//static cb *autostart[] = { PreMain1, PreMain2 };
static cb *autostart[] = { myinit };

#pragma data_seg(".CRT$XPU")
static cb *autoexit[] = { PostMain1, PostMain2 };

#pragma data_seg()    /* reset data-segment */

