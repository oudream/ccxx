/*---------------------------------------------------------------------
SBHeap.cpp - Sample to use small block heap of VC CRT.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <malloc.h>

#define SBH_THRESHOLD 1000 // default is 1016
int _tmain(int argc, _TCHAR* argv[])
{
    char * p1,*p2,*p3,*p4;
    // allcate on normal CRT heap
    printf("allocte 7 bytes at 0x%x\n",p1=new char[7]);
    // enable SBH
    _set_sbh_threshold(SBH_THRESHOLD);
    // do an allocaiton using SBH 
    p2=new char[7];
    // do more allocation to see its evolution
    for(int i=0;i<1000;i++)
    {
        p3=new char[(i+800)%SBH_THRESHOLD];
        printf("allocte %d at 0x%x\n",(i+800)%SBH_THRESHOLD,p3);
    }
    // this allocation is beyond threshold and should be allocated
    // at normal heap
    p4=new char[SBH_THRESHOLD+1];
	return 0;
}

