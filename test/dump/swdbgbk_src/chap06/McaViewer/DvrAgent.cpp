// DvrAgent.cpp: implementation of the CDvrAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "McaViewer.h"
#include "DvrAgent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDvrAgent::CDvrAgent()
{
	ZwSystemDebugControl = NULL;
}

CDvrAgent::~CDvrAgent()
{

}
int CDvrAgent::RDMSR(MSR_STRUCT& msr)
{ 
	return ZwSystemDebugControl(DebugSysReadMsr, &msr, sizeof(msr), NULL, 0, 
		NULL);
} 

int CDvrAgent::WRMSR(MSR_STRUCT& msr)
{
	return ZwSystemDebugControl(DebugSysWriteMsr, &msr, sizeof(msr), NULL, 0, 
		NULL);
}
BOOL EnablePrivilege(HANDLE hToken, LPCSTR lpszName, int enable)
{
	TOKEN_PRIVILEGES tok;

	tok.PrivilegeCount = 1;
	tok.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;

	if(!LookupPrivilegeValue(NULL, lpszName, &tok.Privileges[0].Luid))
	{
		return FALSE;
	}
	return AdjustTokenPrivileges(hToken, FALSE, &tok, sizeof(tok), NULL, NULL);
}
BOOL CDvrAgent::Init()
{
	HMODULE hModule;
	HANDLE hToken;
	hModule = LoadLibrary("ntdll.dll");
	if(hModule == NULL)
	{
		return FALSE;
	}
	ZwSystemDebugControl = (PZwSystemDebugControl)GetProcAddress(hModule, 
		"ZwSystemDebugControl");
	
	if(ZwSystemDebugControl== NULL)
	{
		FreeLibrary(hModule);
		return FALSE;
	}

	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | 
		TOKEN_QUERY, &hToken))
	{
		FreeLibrary(hModule);
		return FALSE;
	}
	if(!EnablePrivilege(hToken, SE_DEBUG_NAME, 1))
	{
		FreeLibrary(hModule);
		return FALSE;
	}
	FreeLibrary(hModule);
	return TRUE;
}

BOOL CDvrAgent::IsInited()
{
	return ZwSystemDebugControl!=NULL;
}
