/*---------------------------------------------------------------------
SEH_Trmt.cpp - Sample to explore termincation handling of SEH.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#include <stdlib.h>
#include <excpt.h>

void PrintHelp()
{
	printf("seh_trmt <number>\n");
}
void FuncWithTry()
{
	__try
	{
	}
	__except(GetExceptionInformation())
	{
		;
	}
}
int main(int argc, char* argv[])
{
	int nNum=0,nRet=0;
	const char* Week_Days[]={"Sunday","Monday","Tuesday",
		"Wednesday","Thursday","Friday","Saturday"};

	printf("Termination Handling of SEH Demonstration!\n");
	if(argc<2)
	{
		PrintHelp(); return -1;
	}
	__try
	{
		printf("You entered: %s. ",argv[1]);
		nNum=atoi(argv[1]);

		printf("It's %d in number.\n",nNum);
		if(nNum<=0)
		{	nRet=-3; __leave; }

		//a test for goto in guarded block
		if(nNum==6666)
			goto EXIT_BYE;

		printf("It's %s in a week.\n",Week_Days[nNum-1]);

		//a test for return in guarded block
		if(nNum==6)
			return -2;
	}
	__finally
	{
		printf("Termination/Cleanup block is executed with %d.\n",
			AbnormalTermination());
		return -5;
	}

EXIT_BYE:
	printf("Exit, Bye!\n");
	return nRet;
}

