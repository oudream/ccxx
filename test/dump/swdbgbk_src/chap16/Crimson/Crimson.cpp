/*---------------------------------------------------------------------
Crimson.cpp - Sample to demonstrate Crimson API.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <evntprov.h>           // ETW Publishing header
# pragma comment(lib, "advapi32.lib")
#include <winevt.h>             // EventLog Header.
# pragma comment(lib, "wevtapi.lib") 
#include "crimson.h"
#include <comdef.h>
#include <sddl.h>
#include <iostream>
#include <tchar.h>
#include <string>
#include <vector>
#include <winevt.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    REGHANDLE hPublisher = NULL;    //Handle to Publisher

    wprintf(L"Sample to publish event log to AdvDbg-Book-WELPublisher... \n");
    
    // Register a Publisher
    ULONG ulResult = EventRegister( 
    &ADVDBG_WEL_PUBLISHER,       // provider guid
        NULL,                          // callback; unused for now
        NULL,                          // context
        &hPublisher);                  // handle required to unregister
    
    if ( ulResult != ERROR_SUCCESS) 
    {
        wprintf(L"Publisher Registration Failed!. Error = 0x%x", ulResult);
        return -1;
    }

    // EventData
    std::vector<EVENT_DATA_DESCRIPTOR> EventDataDesc;
    EVENT_DATA_DESCRIPTOR EvtData;
    
    // inType="win:UnicodeString"
    PWSTR pws = L"Sample Unicode string";
    EventDataDescCreate(&EvtData, pws, ((ULONG)wcslen(pws)+1)*sizeof(WCHAR));
    EventDataDesc.push_back( EvtData );

    // inType="win:AnsiString"
    CHAR * ps = "Sample ANSI string";
    EventDataDescCreate(&EvtData, ps, ((ULONG)strlen(ps)+1)*sizeof(CHAR));
    EventDataDesc.push_back( EvtData );

    // inType="win:Int8"
    INT8 i8 = 0x7F;
    EventDataDescCreate(&EvtData, &i8, sizeof(i8));
    EventDataDesc.push_back( EvtData );

    // inType="win:UInt8"
    UINT8 ui8 = 0xFF;
    EventDataDescCreate(&EvtData, &ui8, sizeof(ui8));
    EventDataDesc.push_back( EvtData );

    // inType="win:Int16"
    INT16 i16 = 0x7FFF;
    EventDataDescCreate(&EvtData, &i16, sizeof(i16));
    EventDataDesc.push_back( EvtData );

    // inType="win:UInt16"
    UINT16 ui16 = 0xFFFF;
    EventDataDescCreate(&EvtData, &ui16, sizeof(ui16));
    EventDataDesc.push_back( EvtData );

    // inType="win:Int32"
    INT32 i32 = 0x7FFFFFFF;
    EventDataDescCreate(&EvtData, &i32, sizeof(i32));
    EventDataDesc.push_back( EvtData );

    // inType="win:UInt32"
    UINT32 ui32 = 0xFFFFFFFF;
    EventDataDescCreate(&EvtData, &ui32, sizeof(ui32));
    EventDataDesc.push_back( EvtData );

    // inType="win:Int64"
    INT64 i64 = 0x7FFFFFFFFFFFFFFFi64;
    EventDataDescCreate(&EvtData, &i64, sizeof(i64));
    EventDataDesc.push_back( EvtData );

    // inType="win:UInt64"
    UINT64 ui64 = 0xFFFFFFFFFFFFFFFFui64;
    EventDataDescCreate(&EvtData, &ui64, sizeof(ui64));
    EventDataDesc.push_back( EvtData );

    // inType="win:Float"
    FLOAT f = -3.1415926e+23f;
    EventDataDescCreate(&EvtData, &f, sizeof(f));
    EventDataDesc.push_back( EvtData );

    // inType="win:Double"
    DOUBLE d = -2.7182818284590452353602874713527e-101;
    EventDataDescCreate(&EvtData, &d, sizeof(d));
    EventDataDesc.push_back( EvtData );

    // inType="win:Boolean"
    BOOL b = TRUE;
    EventDataDescCreate(&EvtData, &b, sizeof(b));
    EventDataDesc.push_back( EvtData );

    // inType="win:GUID"
    GUID guid;  
    EventDataDescCreate(&EvtData, &guid, sizeof(guid));
    EventDataDesc.push_back( EvtData );

    // inType="win:Pointer"
    PVOID p = NULL;
    EventDataDescCreate(&EvtData, &p, sizeof(p));
    EventDataDesc.push_back( EvtData );

    // inType="win:FILETIME"
    SYSTEMTIME st;
    FILETIME ft;

    GetSystemTime(&st);
    SystemTimeToFileTime(&st, &ft);
    EventDataDescCreate(&EvtData, &ft, sizeof(ft));
    EventDataDesc.push_back( EvtData );

    // inType="win:SYSTEMTIME"
    GetSystemTime(&st);
    EventDataDescCreate(&EvtData, &st, sizeof(st));
    EventDataDesc.push_back( EvtData );

    // inType="win:SID"
    PSID pSid = NULL;
    ConvertStringSidToSid(L"S-1-5-19", &pSid); // LocalService
    
    UINT32 sidLength = GetLengthSid(pSid);
    EventDataDescCreate(&EvtData, &sidLength, sizeof(sidLength));
    EventDataDesc.push_back( EvtData );

    EventDataDescCreate(&EvtData, pSid, GetLengthSid(pSid));
    EventDataDesc.push_back( EvtData );

    // inType="win:Binary"
    // Note: if you change the size of this array 
    // you'll have to change the
    // "length" attribute in the manifest too.
    BYTE ab[] = {0,1,2,3,4,5,4,3,2,1,0};
    EventDataDescCreate(&EvtData, ab, sizeof(ab));
    EventDataDesc.push_back( EvtData );

    if ( EventEnabled(hPublisher, &PROCESS_INFO_EVENT) ) 
    {
        ulResult = EventWrite(hPublisher,
                                        &PROCESS_INFO_EVENT,
                                        (ULONG)EventDataDesc.size(),
                                        &EventDataDesc[0]
                                        );
        if (ulResult != ERROR_SUCCESS) 
        {
            //Get Extended Error Information
            wprintf(L"EvtWrite Failed. Not able to fire event. Error = 0x%x", ulResult);

            LocalFree(pSid);
            
            //Close the Publisher Handle
            EventUnregister(hPublisher);
            
            return -2;
        }
    }
    else {
        wprintf(L"Disabled");
    }
    wprintf(L"Success\n");

     LocalFree(pSid);

    //Close the Publisher Handle
    EventUnregister(hPublisher);
	return 0;
}

