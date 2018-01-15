///////////////////////////////////////////////////////////////////////////////
//
// WriteUserStream.cpp 
//
// This example shows how to add a user data stream into the minidump 
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


///////////////////////////////////////////////////////////////////////////////
// Directives 
//

#pragma comment ( lib, "dbghelp.lib" )


///////////////////////////////////////////////////////////////////////////////
// Function declarations 
//

void CreateMiniDump( EXCEPTION_POINTERS* pep ); 


///////////////////////////////////////////////////////////////////////////////
// Test data and code 
//

// Contents of the user data streams 
char Message1[] = "This is the first data stream..."; 
char Message2[] = "and this is the second data stream"; 

// Stream identifiers 
// (LastReservedStream constant is defined in MINIDUMP_STREAM_TYPE 
// enumeration in DbgHelp.h; all user data stream identifiers 
// must be larger than LastReservedStream) 
const ULONG32 cFirstStreamID = LastReservedStream + 1; 
const ULONG32 cSecondStreamID = LastReservedStream + 2; 

// Test function 
void DoWork() 
{
	// Access violation 

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

	HANDLE hFile = CreateFile( _T("UserData.dmp"), GENERIC_READ | GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); 

	if( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) ) 
	{
		// Create the minidump 

			// Exception information 

		MINIDUMP_EXCEPTION_INFORMATION mdei; 

		mdei.ThreadId           = GetCurrentThreadId(); 
		mdei.ExceptionPointers  = pep; 
		mdei.ClientPointers     = FALSE; 

			// Minidump type 

		MINIDUMP_TYPE mdt       = MiniDumpNormal; 

			// User data streams 

		MINIDUMP_USER_STREAM UserStreams[2]; 

		UserStreams[0].Type        = cFirstStreamID; 
		UserStreams[0].Buffer      = Message1; 
		UserStreams[0].BufferSize  = sizeof(Message1); 

		UserStreams[1].Type        = cSecondStreamID; 
		UserStreams[1].Buffer      = Message2; 
		UserStreams[1].BufferSize  = sizeof(Message2); 

		MINIDUMP_USER_STREAM_INFORMATION musi; 

		musi.UserStreamCount   = 2; 
		musi.UserStreamArray   = UserStreams; 

			// Call MiniDumpWriteDump 

		BOOL rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
			hFile, mdt, (pep != 0) ? &mdei : 0, &musi, 0 ); 

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


