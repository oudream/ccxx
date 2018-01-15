// SEH_Trmt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <excpt.h>

void PrintHelp()
{
	printf("seh_trmt <number>\n");
}

int main(int argc, char* argv[])
{
	int nNum=0;
	const char* Year_Months[]={"Jan","Feb","March","April"
		"May","June","July","Aug","Sep","Oct","Nov","Dec"};
	const char* Week_Days[]={"Sunday","Monday","Tuesday",
		"Wednesday","Thursday","Friday","Saturday"};

	printf("Termination Handling of SEH Demonstration!\n\n");
	if(argc<2)
	{
		PrintHelp(); return -1;
	}
	__try
	{
		printf("You entered: %s.\n",argv[1]);
		nNum=atoi(argv[1]);

		printf("It's %d in number.\n",nNum);
		if(nNum<=0)
			__leave;

		if(nNum<=7)
			printf("It's %s in a week.\n",Week_Days[nNum-1]);

		printf("It's %s in a year.\n",Year_Months[nNum-1]);

		return -2;
	}
	__finally
	{
		printf("Termination/Cleanup block is executed with %d.\n",
			AbnormalTermination());
	}

	printf("Exit, Bye!\n");
	return 0;
}

