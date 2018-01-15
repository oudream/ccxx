/*---------------------------------------------------------------------
TinyDbgr.cpp - A tiny debugger for applications.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <stdlib.h>
#define WINVER 0x0501
#include <windows.h>
// ************************************************************************
// FUNCTION : DbgNewProcess( LPTSTR, LPTSTR )
// PURPOSE  : starts a new process as a debuggee
// COMMENTS :
// ************************************************************************
BOOL
DbgNewProcess( LPTSTR szCmdLine)
{
	STARTUPINFO           StartupInfo;
	PROCESS_INFORMATION   ProcessInfo;

	memset ( &StartupInfo , NULL , sizeof ( STARTUPINFO ) ) ;
	memset ( &ProcessInfo , NULL , sizeof ( PROCESS_INFORMATION ) ) ;

	StartupInfo.cb = sizeof ( STARTUPINFO ) ;

	//-- create the Debuggee process
	if( !CreateProcess(
		 0L,
		 szCmdLine,
		 (LPSECURITY_ATTRIBUTES) 0L,
		 (LPSECURITY_ATTRIBUTES) 0L,
		 TRUE,
		 DEBUG_PROCESS | CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS,
		 (LPVOID) 0L,
		 (LPTSTR) 0L,
		 &StartupInfo, &ProcessInfo ) ) 
	{
		TCHAR szMsg[MAX_PATH];

		FormatMessage( 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			0, // Default language
			(LPTSTR) szMsg,
			MAX_PATH,
			NULL 
			);

		printf("Failed in CreateProcess() with error:\n  ");
		printf(szMsg);	printf("\n");
		return( FALSE );
	}
	else
	{
		CloseHandle( ProcessInfo.hProcess );
		CloseHandle( ProcessInfo.hThread );
	}

	return( TRUE );
}
#define MAX_DBG_EVENT 9
LPTSTR DbgEventName[ MAX_DBG_EVENT+1] = {
    "EXCEPTION_DEBUG_EVENT",
    "CREATE_THREAD_DEBUG_EVENT",
    "CREATE_PROCESS_DEBUG_EVENT",
    "EXIT_THREAD_DEBUG_EVENT",
    "EXIT_PROCESS_DEBUG_EVENT",
    "LOAD_DLL_DEBUG_EVENT",
    "UNLOAD_DLL_DEBUG_EVENT",
	"OUTPUT_DEBUG_STRING_EVENT",
	"RIP_EVENT",
    "Unknown Debug Event"
};

BOOL DbgMainLoop(DWORD dwWaitMS)
{
	DEBUG_EVENT DbgEvt;                   // debugging event information 
	DWORD dwContinueStatus = DBG_CONTINUE; // exception continuation 
	BOOL bExit=FALSE;

	while(!bExit) 
	{ 
		// Wait for a debugging event to occur. The second parameter indicates 
		// number of milliseconds to wait for a debugging event. If the parameter
		// is INFINITE the function does not return until a debugging event occurs. 
 
		if(!WaitForDebugEvent(&DbgEvt, dwWaitMS))
		{
			printf("WaitForDebugEvent() returned False %d.\n",GetLastError());
			bExit=TRUE;
			continue;
		}
 
		// Process the debugging event code. 
		printf("Debug event received from process %d thread %d: %s.\n",
			DbgEvt.dwProcessId,	DbgEvt.dwThreadId,
			DbgEventName[DbgEvt.dwDebugEventCode>
				MAX_DBG_EVENT?MAX_DBG_EVENT:
				DbgEvt.dwDebugEventCode-1]); 

		switch (DbgEvt.dwDebugEventCode) 
		{ 
			case EXCEPTION_DEBUG_EVENT: 
			// Process the exception code. When handling 
			// exceptions, remember to set the continuation 
			// status parameter (dwContinueStatus). This value 
			// is used by the ContinueDebugEvent function. 
				printf("-Debuggee breaks into debugger; press any key to continue.\n");
				getchar();
				//return TRUE;

				switch (DbgEvt.u.Exception.ExceptionRecord.ExceptionCode) 
				{ 
					case EXCEPTION_ACCESS_VIOLATION: 
					// First chance: Pass this on to the system. 
					// Last chance: Display an appropriate error. 
						break;
 
					case EXCEPTION_BREAKPOINT: 
					// First chance: Display the current 
					// instruction and register values. 
						break;
 
					case EXCEPTION_DATATYPE_MISALIGNMENT: 
					// First chance: Pass this on to the system. 
					// Last chance: Display an appropriate error. 
						break;
 
					case EXCEPTION_SINGLE_STEP: 
					// First chance: Update the display of the 
					// current instruction and register values. 
						break;
 
					case DBG_CONTROL_C: 
					// First chance: Pass this on to the system. 
					// Last chance: Display an appropriate error. 
						break;
 
					default:
					// Handle other exceptions. 
						break;
				} 
 
			case CREATE_THREAD_DEBUG_EVENT: 
			// As needed, examine or change the thread's registers 
			// with the GetThreadContext and SetThreadContext functions; 
			// and suspend and resume thread execution with the 
			// SuspendThread and ResumeThread functions. 
				break;

			case CREATE_PROCESS_DEBUG_EVENT: 
			// As needed, examine or change the registers of the 
			// process's initial thread with the GetThreadContext and 
			// SetThreadContext functions; read from and write to the 
			// process's virtual memory with the ReadProcessMemory and 
			// WriteProcessMemory functions; and suspend and resume 
			// thread execution with the SuspendThread and ResumeThread 
			// functions. Be sure to close the handle to the process image 
			// file with CloseHandle.
				break;
 
			case EXIT_THREAD_DEBUG_EVENT: 
			// Display the thread's exit code. 
				break;
 
			case EXIT_PROCESS_DEBUG_EVENT: 
			// Display the process's exit code. 
				bExit=TRUE;
				break;
 
			case LOAD_DLL_DEBUG_EVENT: 
			// Read the debugging information included in the newly 
			// loaded DLL. Be sure to close the handle to the loaded DLL 
			// with CloseHandle.
				break;
 
			case UNLOAD_DLL_DEBUG_EVENT: 
			// Display a message that the DLL has been unloaded. 
				break;
 
			case OUTPUT_DEBUG_STRING_EVENT: 
			// Display the output debugging string. 
				break;
 
		} 
 
		// Resume executing the thread that reported the debugging event. 
 		ContinueDebugEvent(DbgEvt.dwProcessId, 
			DbgEvt.dwThreadId, dwContinueStatus); 
 	}
	return TRUE;
}
void Help()
{
   printf ( "TinyDbgr <PID of Program to Debug>|\n    "
             "<Full Exe File Name> [Prgram Parameters]\n" ) ;
}
int main(int argc, char* argv[])
{
	if(argc<=1) 
	{
		Help();	return -1;
	}
	if (strstr(strupr(argv[1]),".EXE"))
	{
		TCHAR szCmdLine[ MAX_PATH ] ;
		szCmdLine[ 0 ] = '\0' ;

		for ( int i = 1 ; i < argc ; i++ )
		{   
			strcat ( szCmdLine , argv[ i ] ) ;
			if ( i < argc )
			{
				strcat ( szCmdLine , " " ) ;
			}
		}
		if(!DbgNewProcess(szCmdLine))
		{
			return -2;
		}
	}
	else
	{
		if(!DebugActiveProcess(atoi(argv[1])))
		{
			printf("Failed in DebugActiveProcess() with %d.\n",GetLastError());
			return -2;
		}

		if(argc>2 && stricmp(argv[2],"-e")==0)
		{
			// try the DebugSetProcessKillOnExit() API
			if(!DebugSetProcessKillOnExit(FALSE))
			{
				printf("Failed in DebugSetProcessKillOnExit() with %d.\n",GetLastError());
			}
		}
		if(argc>2 && stricmp(argv[2],"-s")==0)
		{
			DbgMainLoop(10);
			// try the DebugActiveProcessStop() API
			if(!DebugActiveProcessStop(atoi(argv[1])))
			{
				printf("Failed in DebugActiveProcessStop() with %d.\n",GetLastError());
			}
			else
				printf("Detach debuggee successfully.\n");
			return 0;
		}
	}
	return DbgMainLoop(INFINITE);
}