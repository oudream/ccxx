/*---------------------------------------------------------------------
SEH_Trmt.cpp - Sample to explore nested termination handling
   and exception handling of SEH. 
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#include <excpt.h>

void main(void) 
{ 
    __try 
    { 
        __try 
        { 
			int n=0;
			int i=1/n;
        } 
        __finally 
        { 
            printf("Executing terminating block.\n");
        } 
    } 
    __except (printf("Exceuting ExcpFilter.\n"),
		EXCEPTION_EXECUTE_HANDLER) 
    { 
        printf("Executing exception handling block.\n");
    } 
}
