/*---------------------------------------------------------------------
StkUFlow.cpp - A buggy application with stack underflow.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"

#pragma optimize("",off)
void main()
{
	char sz[20];
	sz[2000]=0;
}

