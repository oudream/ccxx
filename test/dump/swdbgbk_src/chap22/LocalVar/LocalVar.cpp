/*---------------------------------------------------------------------
LocalVar.cpp - A simple application for inspecting local variables.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include <string.h> 
#include <stdio.h>
 
int FuncA()
{
	int l,m,n;
	char sz[]="Advanced SW Debugging";
	l=sz[0];
	m=sz[4];
	n=sz[8];
	return l*m*n;
}

void FuncB(char * szPara)
{
	char szTemp[5];
	strncpy(szTemp,szPara,sizeof(szTemp)-1);
	printf("%s;Len=%d.\n",szTemp,strlen(szTemp));
}
#pragma optimize( "", off )
void FuncC(char * szPara)
{
	char szTemp[5];
	strncpy(szTemp,szPara,sizeof(szTemp)-1);
	printf("%s;Len=%d.\n",szTemp,strlen(szTemp));
}
int main()
{
	FuncA();
	FuncB("Dbg");
	FuncC("Dbg");
	return 0;
}
#pragma optimize( "", on )
