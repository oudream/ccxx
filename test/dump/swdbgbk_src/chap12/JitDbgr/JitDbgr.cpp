/*---------------------------------------------------------------------
JitDbgr.cpp - A simple JIT Debugger.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <stdlib.h>
#include <windows.h>
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
BOOL DbgMainLoop()
{
	DEBUG_EVENT DbgEvt;                   // debugging event information 
	DWORD dwContinueStatus = DBG_CONTINUE; // exception continuation 
	BOOL bExit=FALSE;

	while(!bExit) 
	{ 
		// Wait for a debugging event to occur. The second parameter indicates 
		// that the function does not return until a debugging event occurs. 
 
		if(!WaitForDebugEvent(&DbgEvt, INFINITE))
		{
			printf("WaitForDebugEvent() returned False %d.\n",GetLastError());
			bExit=TRUE;
			continue;
		}
 
		// Process the debugging event code. 
		printf("Debug event received from process %d thread %d: %s.\n",
			DbgEvt.dwProcessId,	DbgEvt.dwThreadId,
			DbgEventName[DbgEvt.dwDebugEventCode>
				MAX_DBG_EVENT?MAX_DBG_EVENT:DbgEvt.dwDebugEventCode-1]); 

		switch (DbgEvt.dwDebugEventCode) 
		{ 
			case EXCEPTION_DEBUG_EVENT: 
			// Process the exception code. When handling 
			// exceptions, remember to set the continuation 
			// status parameter (dwContinueStatus). This value 
			// is used by the ContinueDebugEvent function. 
				printf("-Debuggee breaks into debugger; press any key to continue.\n");
				getchar();
 
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
 		ContinueDebugEvent(DbgEvt.dwProcessId, DbgEvt.dwThreadId, dwContinueStatus); 
 	}
	return TRUE;
}
void Help()
{
   printf ( "JitDbgr -p <PID of Program to Debug> -e <event handle>\n" ) ;
}
int main(int argc, char* argv[])
{
	if(argc<=4) 
	{
		Help();	return -1;
	}
	printf("JitDbgr got parameters: ");
	for(int i=1;i<argc;i++)
		printf("%s ",argv[i]);
	printf("\n");

	if(argc>5)
	{
		SetEvent((HANDLE) atoi(argv[4]));
		return -2;
	}
	if(!DebugActiveProcess(atoi(argv[2])))
	{
		printf("Failed in DebugActiveProcess() with %d.\n",GetLastError());
		return -3;
	}
	printf("Successfully attached debugger, any key to continue.\n");
	getchar();
	SetEvent((HANDLE) atoi(argv[4]));
	return DbgMainLoop();
}