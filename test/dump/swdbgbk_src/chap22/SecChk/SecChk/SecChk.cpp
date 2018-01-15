#include <windows.h>
#include <stdio.h>
#include <rtcapi.h>
#ifdef _DEBUG
TCHAR SZ_INPUT[]="\x6a\x01\x33\xc0\
\x50\x50\x50\xba\x0b\x05\xd8\x77\
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
void   __fastcall RTC_CheckStackVars(void *_Esp, _RTC_framedesc *_Fd);
int ng;
#pragma optimize ("",off) 
__declspec(noinline)
void __cdecl _crt_debugger_hook(int _Reserved)
{
    /* assign 0 to _debugger_hook_dummy so that the function is not folded in retail */
    (_Reserved);
	ng=0;
	MessageBox(NULL,"","",MB_OK);
    __asm int 3;
}
void __cdecl MyCrtDbggerHook(int _Reserved)
{
    /* assign 0 to _debugger_hook_dummy so that the function is not folded in retail */
    (_Reserved);
	ng=0;
	MessageBox(NULL,"","",MB_OK);
    __asm int 3;
}
//\xb8\xfe\x12";
/*	_asm
	{
TAG_LOOP:
		push MB_OKCANCEL
        xor eax,eax
        push eax
		push eax
		push eax
		mov edx, 77d8050bh
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
void VarCheck(LPCTSTR lpszInput)
{
	int n;
	TCHAR szFstBuffer[3];
	LPTSTR lpsz;
	TCHAR szSndBuffer[5]="";
	char m=9;
	TCHAR szThdBuffer[3];
	int j;
	j=n=m=strlen(lpszInput);
	sprintf(szFstBuffer,"%d\n",n);
	OutputDebugString(szFstBuffer);

	lpsz=szThdBuffer;
	strcpy(lpsz,lpszInput);
	strcat(szSndBuffer,lpsz);
	OutputDebugString(szSndBuffer);
}
void EmptyFunc()
{
}
int UninitialCheck()
{
	int m,n;
	LPCTSTR lpsz;
	LPCTSTR lpsz3,lpsz2=lpsz;
	//m=n;
    lpsz=lpsz3;
    int j=m;
    return m;
}
int APIENTRY WinMain(HINSTANCE hInstance,
   HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MessageBox(NULL,"A sample to demo security check for buffer overflow attack.",
		"AdvDbg",MB_OK);
	EmptyFunc();
    UninitialCheck();

	// Call VarCheck function, will not cause any overflow
	VarCheck("DB");
	// Call VarCheck function to cause overflow a little bit,
	// this overflow can triger variable check failure, but not GS failure
	VarCheck("DBG");
	//
	// Significant overflow, which will triger GS failure in release build.
	HandleInput(SZ_INPUT);
	_crt_debugger_hook(0);
	return 0;
}
void _RTC_StackFailure(void *,char const*);
void   __fastcall RTC_CheckStackVars(void *_Frame, _RTC_framedesc *_Fd)
{
	DWORD* pdwFense,_RetAddr;

	_RTC_vardesc * pVarDesc= _Fd->variables;

	for( int i=0; i< _Fd->varCount;i++)
	{
		pdwFense=(DWORD*)((DWORD)_Frame+
			(DWORD)pVarDesc->addr-sizeof(DWORD));
		if(*pdwFense!=0xCCCCCCCC)
			goto TAG_CORRUPT_FOUND;
		pdwFense=(DWORD*)((DWORD)_Frame+
			(DWORD)pVarDesc->addr+pVarDesc->size);
		if(*pdwFense!=0xCCCCCCCC)
		{
TAG_CORRUPT_FOUND:
			_asm mov edx,dword ptr [ebp+4] ;
			_asm mov _RetAddr, edx;
			_RTC_StackFailure((void *)_RetAddr,pVarDesc->name);
		}

		pVarDesc++;
	}
}
//  Code use to test above faked RTC_CheckStackVars function.
//  VOID * pEsp;
//	_asm mov pEsp,ebp;
//	RTC_CheckStackVars(pEsp,(_RTC_framedesc *) 0x4115c4);
/*#ifdef _CRT_DEBUGGER_HOOK
#undef _CRT_DEBUGGER_HOOK
#define _CRT_DEBUGGER_HOOK MyCrtDbggerHook
#endif 
*/

