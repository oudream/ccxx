///////////////////////////////////////////////////////////////////////////////
//
// CallbackOrder.cpp 
//
// This example shows the order of calls to MiniDumpCallback function 
// 
// Author: Oleg Starodumov (www.debuginfo.com)
//
//


///////////////////////////////////////////////////////////////////////////////
// Include files 
//

#include <windows.h>
#include <tchar.h>
#include <dbghelp.h>
#include <stdio.h>
#include <crtdbg.h>


///////////////////////////////////////////////////////////////////////////////
// Directives 
//

#pragma comment ( lib, "dbghelp.lib" )


///////////////////////////////////////////////////////////////////////////////
// Function declarations 
//

void CreateMiniDump( EXCEPTION_POINTERS* pep ); 

BOOL CALLBACK MyMiniDumpCallback(
	PVOID                            pParam, 
	const PMINIDUMP_CALLBACK_INPUT   pInput, 
	PMINIDUMP_CALLBACK_OUTPUT        pOutput 
); 


///////////////////////////////////////////////////////////////////////////////
// Global variables 
//

bool MemoryCallbackCalled = false; 


///////////////////////////////////////////////////////////////////////////////
// Test function 
//

void DoWork() 
{
	*(int*)0 = 1; 
}


///////////////////////////////////////////////////////////////////////////////
// main() function 
//

int main( int argc, char* argv[] ) 
{
	__try 
	{
		DoWork(); 
	}
	__except( CreateMiniDump( GetExceptionInformation() ), EXCEPTION_EXECUTE_HANDLER ) 
	{
	}

	return 0; 
}


///////////////////////////////////////////////////////////////////////////////
// Minidump creation function 
//

void CreateMiniDump( EXCEPTION_POINTERS* pep ) 
{
	// Open the file 

	HANDLE hFile = CreateFile( _T("CallbackOrder.dmp"), GENERIC_READ | GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); 

	if( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) ) 
	{
		// Create the minidump 

		MINIDUMP_EXCEPTION_INFORMATION mdei; 

		mdei.ThreadId           = GetCurrentThreadId(); 
		mdei.ExceptionPointers  = pep; 
		mdei.ClientPointers     = FALSE; 

		MINIDUMP_CALLBACK_INFORMATION mci; 

		mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MyMiniDumpCallback; 
		mci.CallbackParam       = 0; 

		MINIDUMP_TYPE mdt       = MiniDumpNormal; 

		BOOL rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
			hFile, mdt, (pep != 0) ? &mdei : 0, 0, &mci ); 

		if( !rv ) 
			_tprintf( _T("MiniDumpWriteDump failed. Error: %u \n"), GetLastError() ); 
		else 
			_tprintf( _T("Minidump created.\n") ); 

		// Close the file 

		CloseHandle( hFile ); 

	}
	else 
	{
		_tprintf( _T("CreateFile failed. Error: %u \n"), GetLastError() ); 
	}

}


///////////////////////////////////////////////////////////////////////////////
// Custom minidump callback 
//

BOOL CALLBACK MyMiniDumpCallback(
	PVOID                            pParam, 
	const PMINIDUMP_CALLBACK_INPUT   pInput, 
	PMINIDUMP_CALLBACK_OUTPUT        pOutput 
) 
{
	BOOL bRet = FALSE; 


	// Check parameters 

	if( pInput == 0 ) 
		return FALSE; 

	if( pOutput == 0 ) 
		return FALSE; 


	// Process callbacks 

	switch( pInput->CallbackType ) 
	{
		case IncludeModuleCallback: 
		{
			// Include the module into the dump 
			_tprintf( _T("IncludeModuleCallback (module: %08I64x) \n"), 
				pInput->IncludeModule.BaseOfImage); 
			bRet = TRUE; 
		}
		break; 

		case IncludeThreadCallback: 
		{
			// Include the thread into the dump 
			_tprintf( _T("IncludeThreadCallback (thread: %x) \n"), 
				pInput->IncludeThread.ThreadId); 
			bRet = TRUE; 
		}
		break; 

		case ModuleCallback: 
		{
			// Include all available information 
			wprintf( L"ModuleCallback (module: %s) \n", pInput->Module.FullPath ); 
			bRet = TRUE; 
		}
		break; 

		case ThreadCallback: 
		{
			// Include all available information 
			_tprintf( _T("ThreadCallback (thread: %x) \n"), pInput->Thread.ThreadId ); 
			bRet = TRUE;  
		}
		break; 

		case ThreadExCallback: 
		{
			// Include all available information 
			_tprintf( _T("ThreadExCallback (thread: %x) \n"), pInput->ThreadEx.ThreadId ); 
			bRet = TRUE;  
		}
		break; 

		case MemoryCallback: 
		{
			// Let CancelCallback know where to stop 
			MemoryCallbackCalled = true; 

			// We do not include any information here -> return FALSE 
			_tprintf( _T("MemoryCallback\n") ); 
			bRet = FALSE; 
		}
		break; 

		case CancelCallback: 
		{
			_tprintf( _T("CancelCallback\n") ); 

			if( !MemoryCallbackCalled) 
			{
				// Continue receiving CancelCallback callbacks 
				pOutput->Cancel       = FALSE; 
				pOutput->CheckCancel  = TRUE; 
			}
			else 
			{
				// No cancel callbacks anymore 
				pOutput->Cancel       = FALSE; 
				pOutput->CheckCancel  = FALSE; 
			}
			bRet = TRUE; 
		}
		break; 
	}


	return bRet; 

}

