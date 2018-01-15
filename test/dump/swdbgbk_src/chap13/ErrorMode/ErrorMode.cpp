// ErrorMode.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <stdio.h>
//
// Process Information Classes
//

typedef enum _PROCESSINFOCLASS {
    ProcessBasicInformation,
    ProcessQuotaLimits,
    ProcessIoCounters,
    ProcessVmCounters,
    ProcessTimes,
    ProcessBasePriority,
    ProcessRaisePriority,
    ProcessDebugPort,
    ProcessExceptionPort,
    ProcessAccessToken,
    ProcessLdtInformation,
    ProcessLdtSize,
    ProcessDefaultHardErrorMode,
    ProcessIoPortHandlers,          // Note: this is kernel mode only
    ProcessPooledUsageAndLimits,
    ProcessWorkingSetWatch,
    ProcessUserModeIOPL,
    ProcessEnableAlignmentFaultFixup,
    ProcessPriorityClass,
    ProcessWx86Information,
    ProcessHandleCount,
    ProcessAffinityMask,
    ProcessPriorityBoost,
    ProcessDeviceMap,
    ProcessSessionInformation,
    ProcessForegroundInformation,
    ProcessWow64Information,
    ProcessImageFileName,
    ProcessLUIDDeviceMapsEnabled,
    ProcessBreakOnTermination,
    ProcessDebugObjectHandle,
    ProcessDebugFlags,
    ProcessHandleTracing,
    MaxProcessInfoClass             // MaxProcessInfoClass should always be the last enum
    } PROCESSINFOCLASS;

typedef ULONG
(NTAPI * NTQUERYINFORMATIONPROCESS)(
    IN HANDLE ProcessHandle,
    IN PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN ULONG ProcessInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

UINT GetErrorMode()
{
	HMODULE hNtDll;
	NTQUERYINFORMATIONPROCESS NtQueryInformationProcess=NULL;
	UINT uErrorMode=0;

	hNtDll=LoadLibrary("NTDLL.DLL");
	if(hNtDll==NULL)
	{
		OutputDebugString("GetErrorMode: Failed in loading NTDLL.\n");
		return (UINT)-1;
	}
	NtQueryInformationProcess=(NTQUERYINFORMATIONPROCESS)
		GetProcAddress(hNtDll,"NtQueryInformationProcess");
	
	FreeLibrary(hNtDll);
	if(0==NtQueryInformationProcess((HANDLE)-1,
		ProcessDefaultHardErrorMode,&uErrorMode,
		sizeof(UINT),NULL))
	{
		if (uErrorMode & 1)
			uErrorMode &= ~SEM_FAILCRITICALERRORS;
		else
			uErrorMode |= SEM_FAILCRITICALERRORS;

		return uErrorMode;
	}
	return (UINT)-1;
}
void GPFault()
{
	_asm int 19
}
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	UINT nDefMode;
	TCHAR szMsg[MAX_PATH];
	OFSTRUCT of;

	nDefMode=GetErrorMode();
	sprintf(szMsg,"Current ErrorMode=%X",nDefMode);
	MessageBox(NULL,szMsg,"ErrorMode",MB_OK);

	nDefMode=SetErrorMode(SEM_NOGPFAULTERRORBOX);
	sprintf(szMsg,"Old ErrorMode=%X",nDefMode);
	MessageBox(NULL,szMsg,"ErrorMode",MB_OK);
	//try
	{
		GPFault();
	}//catch(...)
	{
	}
	nDefMode=SetErrorMode(SEM_FAILCRITICALERRORS);
	sprintf(szMsg,"Current ErrorMode=%X",nDefMode);
	MessageBox(NULL,szMsg,"ErrorMode",MB_OK);

	nDefMode=SetErrorMode(	SEM_NOOPENFILEERRORBOX);
	OpenFile("c:\\abc.abc",&of,OF_PROMPT);

	GPFault();

	return 0;
}



