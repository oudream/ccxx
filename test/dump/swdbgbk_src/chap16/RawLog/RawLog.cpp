/*---------------------------------------------------------------------
RawLog.cpp - Sample to publish event log without manifest.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <evntprov.h>           // ETW Publishing header
# pragma comment(lib, "advapi32.lib")
#include <winevt.h>             // EventLog Header.
# pragma comment(lib, "wevtapi.lib") 
// {96B49A84-9396-4b07-A24F-672B1F1079D9}
static const GUID RawCrimson = 
{ 0x96b49a84, 0x9396, 0x4b07, { 0xa2, 0x4f, 0x67, 0x2b, 0x1f, 0x10, 0x79, 0xd9 } };

int _tmain(int argc, _TCHAR* argv[])
{
    REGHANDLE hPublisher = NULL;    //Handle to Publisher

    wprintf(L"Sample to publish event log without manifest \n");
    
    // Register a Publisher
    ULONG ulResult = EventRegister( 
        &RawCrimson,       // provider guid
        NULL,                          // callback; unused for now
        NULL,                          // context
        &hPublisher);                  // handle required to unregister
    
    if ( ulResult != ERROR_SUCCESS) 
    {
        wprintf(L"Publisher Registration Failed!. Error = 0x%x", ulResult);
        return -1;
    }
    ulResult=EventWriteString(hPublisher, 0,
        0, _T("Hello, this a string info output by EventWriteString API\n"));
    
    wprintf(L"A string info is written with result = 0x%x", ulResult);

    EventUnregister(hPublisher);
	return 0;
}

