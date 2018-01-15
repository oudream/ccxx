/*---------------------------------------------------------------------
NKLMgr.cpp - Sample to control NT Kernel Logger.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#include "ETW.h"
#include "NKLMgr.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNKLMgr::CNKLMgr()
{
	m_hNklSessionHandle = 0;
	m_pListBox = NULL;
	m_pNklProperties = NULL;
}

CNKLMgr::~CNKLMgr()
{
	if(m_pNklProperties!=NULL)
		free(m_pNklProperties);
}
//#define LOGFILE_PATH L"<DRIVE>:\\<PATH>\\kernellogfile.etl"

BOOL CNKLMgr::StartNKL(LPCTSTR lpszLogFile,DWORD dwEnableFlags)
{
	ULONG BufferSize = 0;
	ULONG rc = 0;

	BufferSize = sizeof(EVENT_TRACE_PROPERTIES) + 
		sizeof(TCHAR)*(_tcslen(lpszLogFile)+1) + sizeof(KERNEL_LOGGER_NAME);
	if(m_pNklProperties==NULL)
		m_pNklProperties = (EVENT_TRACE_PROPERTIES*) malloc(BufferSize);	

	if (NULL == m_pNklProperties)
	{
	  wprintf(L"Unable to allocate %d bytes for properties structure.\n", BufferSize);
	  return FALSE;
	}

	ZeroMemory(m_pNklProperties, BufferSize);
	m_pNklProperties->Wnode.BufferSize = BufferSize;
	m_pNklProperties->Wnode.Flags = WNODE_FLAG_TRACED_GUID;
	m_pNklProperties->Wnode.ClientContext = 1; //QPC clock resolution
	m_pNklProperties->Wnode.Guid = SystemTraceControlGuid;
	m_pNklProperties->EnableFlags = dwEnableFlags; 
	m_pNklProperties->LogFileMode = EVENT_TRACE_FILE_MODE_CIRCULAR 
		| EVENT_TRACE_USE_PAGED_MEMORY;
	m_pNklProperties->MaximumFileSize = 20;
	m_pNklProperties->LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);
	m_pNklProperties->LogFileNameOffset = sizeof(EVENT_TRACE_PROPERTIES) 
		+ sizeof(KERNEL_LOGGER_NAME); 
	_tcscpy((LPTSTR)((char*)m_pNklProperties + m_pNklProperties->LogFileNameOffset),
		lpszLogFile);

	rc = StartTrace((PTRACEHANDLE)&m_hNklSessionHandle,
		KERNEL_LOGGER_NAME, m_pNklProperties);
	if (ERROR_SUCCESS != rc)
	{
		if (ERROR_ALREADY_EXISTS == rc)
			OUTMSG(_T("The NT Kernel Logger session is already in use.\n"));
		else
			OUTMSG(_T("StartTrace() failed, %d.\n"), rc);

		return FALSE;
	}
	OUTMSG(_T("StartTrace() succeeded, %s.\n"), lpszLogFile);

	return TRUE;
}
#define MAX_MESSAGE_SIZE 1024
void CNKLMgr::OUTMSG(LPCTSTR szFormat, ...)
{
	TCHAR szMsg[MAX_MESSAGE_SIZE];

    va_list va;
    va_start( va, szFormat );

    int nReturnValue = _vsntprintf( szMsg, 
		MAX_MESSAGE_SIZE, szFormat, va );

    va_end(va);

	OutputDebugString(szMsg);
	if(m_pListBox)
		m_pListBox->AddString(szMsg);
}

void CNKLMgr::SetListBox(CListBox *pMsgList)
{
	this->m_pListBox=pMsgList;
}

BOOL CNKLMgr::StopNKL()
{
	return StopSession(KERNEL_LOGGER_NAME);
}

BOOL CNKLMgr::StopSession(LPCTSTR szSessionName)
{
	ULONG rc = 0;

	rc = ControlTrace(NULL, szSessionName, m_pNklProperties,
		EVENT_TRACE_CONTROL_STOP);
	if (ERROR_SUCCESS != rc) 
	{
		OUTMSG(_T("ControlTrace(STOP) failed, %d\n"), rc);
	}
	else
		OUTMSG(_T("ControlTrace(STOP) succeeded\n"));

	return (ERROR_SUCCESS == rc); 
}
