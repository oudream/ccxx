/*-------------------------------------------------
// Sample to demo usage of WT command
// Part of Software Debugging book by Raymond
//------------------------------------------------*/
#include <windows.h>
int GetRandom(int n)
{
	int m=2*n,nTick;
	nTick=GetTickCount();
	m*=nTick;
	m*=GetVersion();
	return m*n;
}
int main(int argc, char* argv[])
{
	int n=argc*100;
	n=GetRamdom(n);
	return n;
}

