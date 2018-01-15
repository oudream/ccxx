// EventLog.cpp: Implementierung der Klasse CEventLog.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEventLog::CEventLog()
{
  m_hLog = NULL;
}

CEventLog::~CEventLog()
{
  if (m_hLog != NULL)
  {
    DeregisterEventSource(m_hLog);
    m_hLog = NULL;
  }
}


BOOL CEventLog::Initialize(CString csApp)
{
  // Try to add application to EventVwr
  if (AddEventSource(csApp, 3 ) != 0)
  {
    CString cs;
    cs.Format("Unable to register EventLog access for application %s.", cs);
    cs += "  Please log in with admin rights to do this.";
    cs += "  \nApplication will run without event logging";
    AfxMessageBox(cs, MB_ICONEXCLAMATION);
  }

  // Register to write
  m_hLog = ::RegisterEventSource( NULL, csApp);

  return TRUE;
}

HRESULT CEventLog::AddEventSource(CString csName, DWORD dwCategoryCount)
{
	HKEY	hRegKey = NULL; 
	DWORD	dwError = 0;
	TCHAR	szPath[ MAX_PATH ];

	_stprintf( szPath, _T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\%s"), csName );

	// Create the event source registry key
	dwError = RegCreateKey( HKEY_LOCAL_MACHINE, szPath, &hRegKey );
	if (dwError != 0)
	{
		OutMsg("RegCreateKey failed for %d",GetLastError());
		return E_FAIL;
	}

	// Name of the PE module that contains the message resource
	GetModuleFileName( NULL, szPath, MAX_PATH );

	// Register EventMessageFile
	dwError = RegSetValueEx( hRegKey, _T("EventMessageFile"), 0, REG_EXPAND_SZ, (PBYTE) szPath, (_tcslen( szPath) + 1) * sizeof TCHAR ); 
	if (dwError == 0)
	{
		// Register supported event types
		DWORD dwTypes = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE; 
		dwError = RegSetValueEx( hRegKey, _T("TypesSupported"),	0, REG_DWORD, (LPBYTE) &dwTypes, sizeof dwTypes );

		// If we want to support event categories, we have also to register	the CategoryMessageFile.
		// and set CategoryCount. Note that categories need to have the message ids 1 to CategoryCount!

		if(dwError == 0 && dwCategoryCount > 0 ) 
		{
			dwError = RegSetValueEx( hRegKey, _T("CategoryMessageFile"), 0, REG_EXPAND_SZ, (PBYTE) szPath, (_tcslen( szPath) + 1) * sizeof TCHAR );
			if (dwError == 0)
				dwError = RegSetValueEx( hRegKey, _T("CategoryCount"), 0, REG_DWORD, (PBYTE) &dwCategoryCount, sizeof dwCategoryCount );
		}
	}

	RegCloseKey( hRegKey );

	return S_OK;
}

HRESULT CEventLog::RemoveEventSource(CString csApp)
{
	DWORD dwError = 0;
	TCHAR szPath[ MAX_PATH ];
	
	_stprintf( szPath, _T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\%s"), csApp );
	return RegDeleteKey( HKEY_LOCAL_MACHINE, szPath );
}

CString CEventLog::LoadMessage(DWORD dwMsgId, ...)
{
	char    pszBuffer[1024];
	DWORD   cchBuffer = 1024;

	va_list args;
	va_start( args, cchBuffer );

	if (FormatMessage( 
		FORMAT_MESSAGE_FROM_HMODULE,
		NULL,					// Module (e.g. DLL) to search for the Message. NULL = own .EXE
		dwMsgId,				// Id of the message to look up (aus "Messages.h")
		LANG_NEUTRAL,			// Language: LANG_NEUTRAL = current thread's language
		pszBuffer,				// Destination buffer
		cchBuffer,				// Character count of destination buffer
		&args					// Insertion parameters
	))
		return pszBuffer;
	else
		return "";
}


BOOL CEventLog::Fire(WORD wType, WORD wCategory, DWORD dwEventID, ...)
{
	va_list args;
	va_start( args, dwEventID );

	CString cs;
	int iCount = 0;

	while(1)
	{
		char *p = va_arg( args, char *);
		if (p!=NULL && *p != '\0')
		  iCount++;
		else
		  break;
	}

	va_start( args, dwEventID );

	if (m_hLog == NULL)
		return FALSE;

	BOOL bRet = ReportEvent(m_hLog, wType, wCategory, dwEventID,
		NULL, iCount, 0, (LPCTSTR *)args,	NULL);
	va_end(args);
	return bRet;
}

BOOL CEventLog::FireWithData(WORD wType, WORD wCategory, DWORD dwEventID, DWORD dwData, LPVOID ptr,...)
{
	va_list args;
	va_start( args, ptr );

	CString cs;
	int iCount = 0;

	while(1)
	{
		char *p = va_arg( args, char *);
		if (*p != '\0')
		  iCount++;
		else
		  break;
	}

	va_start( args, ptr );

	if (m_hLog == NULL)
		return FALSE;

	BOOL bRet = ReportEvent(m_hLog, wType, wCategory, dwEventID,
		NULL, iCount, dwData, (LPCTSTR *)args,	ptr);
	va_end(args);
	return bRet;
}

PSID CEventLog::GetUserSID(PSID * ppSid)
{
	BOOL bRet = FALSE;
	const DWORD INITIAL_SIZE = MAX_PATH;

	CString csAccName;
	DWORD size = INITIAL_SIZE;

	::GetUserName(csAccName.GetBufferSetLength(size), &size);
	csAccName.ReleaseBuffer(size);

	// Validate the input parameters.
	if (csAccName.IsEmpty() || ppSid == NULL)
	{
		return NULL;
	}

	// Create buffers that may be large enough.
	// If a buffer is too small, the count parameter will be set to the size needed.
	DWORD cbSid = 0;
	DWORD dwErrorCode = 0;
	DWORD dwSidBufferSize = INITIAL_SIZE;
	DWORD cchDomainName = INITIAL_SIZE;
	CString csDomainName;
	SID_NAME_USE eSidType;
	HRESULT hr = 0;


	// Create buffers for the SID and the domain name.
	*ppSid = (PSID) new BYTE[dwSidBufferSize];
	if (*ppSid == NULL)
	{
		return NULL;
	}
	memset(*ppSid, 0, dwSidBufferSize);

	// Obtain the SID for the account name passed.
	for ( ; ; )
	{
	// Set the count variables to the buffer sizes and retrieve the SID.
	cbSid = dwSidBufferSize;
	bRet = LookupAccountName(NULL, csAccName, *ppSid, &cbSid, csDomainName.GetBufferSetLength(cchDomainName), &cchDomainName,&eSidType);
	csDomainName.ReleaseBuffer();
	if (bRet)
	{
	  if (IsValidSid(*ppSid) == FALSE)
	  {
		CString csMsg;
		csMsg.Format("The SID for %s is invalid.\n", csAccName);
		AfxMessageBox(csMsg, MB_ICONSTOP);
		bRet = FALSE;
	  }
	  break;
	}
	dwErrorCode = GetLastError();

	// Check if one of the buffers was too small.
	if (dwErrorCode == ERROR_INSUFFICIENT_BUFFER)
	{
	  if (cbSid > dwSidBufferSize)
	  {
		// Reallocate memory for the SID buffer.
		TRACE("The SID buffer was too small. It will be reallocated.\n");
		FreeSid(*ppSid);
		*ppSid = (PSID) new BYTE[cbSid];
		if (*ppSid == NULL)
		{
		  return NULL; 
		}
		memset(*ppSid, 0, cbSid);
		dwSidBufferSize = cbSid;
	  }
	}
	else
	{
	  CString csMsg;
	  csMsg.Format("LookupAccountNameW failed. GetLastError returned: %d\n", dwErrorCode);
	  AfxMessageBox(csMsg, MB_ICONSTOP);
	  hr = HRESULT_FROM_WIN32(dwErrorCode);
	  break;
	}
	}

	// If we had an error, free memory of SID
	if (!bRet && *ppSid != NULL)
	{
	delete [] *ppSid;
	*ppSid = NULL; 
	}

	return *ppSid;
}
// "System"
BOOL CEventLog::DisplayLogInfo(LPCTSTR lpszLogName)
{
   HANDLE h;
   DWORD cRecords; 
   DWORD dwOldest;

   // Open the System event log. 
 
   h = OpenEventLog(NULL,  // uses local computer 
             lpszLogName);     // source name 
   if (h == NULL) 
   {    
       OutMsg("Could not open the System event log."); 
       return 0;
   }
 
   // Display the number of records and the oldest record. 
   if (GetNumberOfEventLogRecords(h, &cRecords)) 
       OutMsg("There are %d records in the System log.\n", cRecords); 

   if (GetOldestEventLogRecord(h, &dwOldest))
       OutMsg("The oldest record is: %d\n\n", dwOldest);

   CloseEventLog(h); 
   return TRUE;
}

#define BUFFER_SIZE 1024*64

BOOL CEventLog::DisplayEntries(LPCTSTR lpszLogName)
{
    HANDLE h;
    EVENTLOGRECORD *pevlr; 
    BYTE bBuffer[BUFFER_SIZE]; 
    DWORD dwRead, dwNeeded, dwThisRecord; 
 
    // Open the Application event log. 
 
    h = OpenEventLog( NULL,    // use local computer
             lpszLogName);   // source name
    if (h == NULL) 
    {
        OutMsg("Could not open the Application event log."); 
        return 0;
    }
 
    pevlr = (EVENTLOGRECORD *) &bBuffer; 
 
    // Get the record number of the oldest event log record.

    GetOldestEventLogRecord(h, &dwThisRecord);

    // Opening the event log positions the file pointer for this 
    // handle at the beginning of the log. Read the event log records 
    // sequentially until the last record has been read. 
 
    while (ReadEventLog(h,                // event log handle 
                EVENTLOG_FORWARDS_READ |  // reads forward 
                EVENTLOG_SEQUENTIAL_READ, // sequential read 
                0,            // ignored for sequential reads 
                pevlr,        // pointer to buffer 
                BUFFER_SIZE,  // size of buffer 
                &dwRead,      // number of bytes read 
                &dwNeeded))   // bytes in next record 
    {
        while (dwRead > 0) 
        { 
            // Print the record number, event identifier, type, 
            // and source name. 
 
            OutMsg("%03d  Event ID: 0x%08X  Event type: ", 
                dwThisRecord++, pevlr->EventID); 

            switch(pevlr->EventType)
            {
                case EVENTLOG_ERROR_TYPE:
                    printf("EVENTLOG_ERROR_TYPE\t  ");
                    break;
                case EVENTLOG_WARNING_TYPE:
                    printf("EVENTLOG_WARNING_TYPE\t  ");
                    break;
                case EVENTLOG_INFORMATION_TYPE:
                    printf("EVENTLOG_INFORMATION_TYPE  ");
                    break;
                case EVENTLOG_AUDIT_SUCCESS:
                    printf("EVENTLOG_AUDIT_SUCCESS\t  ");
                    break;
                case EVENTLOG_AUDIT_FAILURE:
                    printf("EVENTLOG_AUDIT_FAILURE\t  ");
                    break;
                default:
                    printf("Unknown ");
                    break;
            }

            OutMsg("Event source: %s\n", 
                (LPSTR) ((LPBYTE) pevlr + sizeof(EVENTLOGRECORD))); 
 
            dwRead -= pevlr->Length; 
            pevlr = (EVENTLOGRECORD *) 
                ((LPBYTE) pevlr + pevlr->Length); 
        } 
 
        pevlr = (EVENTLOGRECORD *) &bBuffer; 
    } 
 
    CloseEventLog(h); 
	return TRUE;
}
//----------------------------------------------------------------------
//
// DbgMsg
//
// Output debug information to a list box or a console which can receive
// debug information generated by OutputDebugString API. 
//
//----------------------------------------------------------------------
#define MAX_MESSAGE_SIZE 1024
void CEventLog::OutMsg(LPCTSTR szMsgFormat,...)
{
	TCHAR szMessage[MAX_MESSAGE_SIZE];

    va_list va;
    va_start( va, szMsgFormat );

    int nReturnValue = _vsntprintf( szMessage, 
		MAX_MESSAGE_SIZE, szMsgFormat, va );

    va_end(va);

	if(m_hWndListener)
		SendMessage(m_hWndListener,
			LB_ADDSTRING,NULL,(LPARAM)szMessage);

#ifdef ADV_DBG
	OutputDebugString(szMessage);
#endif
}

