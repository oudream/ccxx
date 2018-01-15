// HardErrorRaiser.cpp: implementation of the CHardErrorRaiser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HardError.h"
#include "HardErrorRaiser.h"
//#include <ntstatus.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHardErrorRaiser::CHardErrorRaiser()
{
	m_pfnNtRaiseHardError=NULL;
	m_pListBox=NULL;
}

CHardErrorRaiser::~CHardErrorRaiser()
{

}

BOOL CHardErrorRaiser::Init()
{
	HMODULE hNtDll=NULL;
	if(m_pfnNtRaiseHardError)
		return TRUE;

	hNtDll=LoadLibrary("NTDLL.DLL");
	if(hNtDll==NULL)
	{
		DbgMsg("Failed in loading NTDLL with %d",GetLastError());
		return FALSE;
	}
	m_pfnNtRaiseHardError=(NTRAISEHARDERROR)GetProcAddress(
		hNtDll,"NtRaiseHardError");
	
	FreeLibrary(hNtDll);

	return (m_pfnNtRaiseHardError!=NULL);
}
#define MAX_MESSAGE_SIZE 1024
void CHardErrorRaiser::DbgMsg(LPCTSTR szFormat, ...)
{
	TCHAR szMsg[MAX_MESSAGE_SIZE];

    va_list va;
    va_start( va, szFormat );

    int nReturnValue = _vsntprintf( szMsg, 
		MAX_MESSAGE_SIZE, szFormat, va );

    va_end(va);
	if(m_pListBox)
		m_pListBox->AddString(szMsg);

	OutputDebugString(szMsg);
}
/*             retValue =  NtRaiseHardError(
                   STATUS_UNHANDLED_EXCEPTION | 0x10000000,
                   4, 0, &harderr,
                   _BasepAlreadyHadHardError ? 1 : dwDlgOptionFlag,
                   &dwResponse );
*/
#define STATUS_UNHANDLED_EXCEPTION       (0xC0000144L)

ULONG CHardErrorRaiser::HardError(ULONG ulResOptions, 
								  LPCTSTR szMsg, PULONG pulResponse)
{
    ULONG ErrorResponse;
	PWSTR pwsMsg;
	int nLenWS;
	ULONG ErrorParameters[1];

	if(NULL ==m_pfnNtRaiseHardError)
		return -1;

	nLenWS=MultiByteToWideChar(CP_ACP,0,szMsg,-1,NULL,0);
	pwsMsg=(PWSTR)HeapAlloc(GetProcessHeap(),0,nLenWS*sizeof(WCHAR));
	if(pwsMsg==NULL)
	{
		DbgMsg("Failed in allocating memory.");
		return -1;
	}
	MultiByteToWideChar(CP_ACP,0,szMsg,-1,pwsMsg,nLenWS);

    ErrorResponse = ResponseOk;
    ErrorParameters[0] = (ULONG_PTR)pwsMsg;

    /*ULONG ulStatus=m_pfnNtRaiseHardError(
		//STATUS_FLOAT_DIVIDE_BY_ZERO,//|0x10000000,//STATUS_UNHANDLED_EXCEPTION|0x10000000,
		STATUS_UNHANDLED_EXCEPTION,
                      1,
                      1,
                      ErrorParameters,
                      OptionOkCancel,
                      &ErrorResponse
                    );
	*/
    ULONG ulStatus=m_pfnNtRaiseHardError(
		//STATUS_FLOAT_DIVIDE_BY_ZERO,//|0x10000000,//STATUS_UNHANDLED_EXCEPTION|0x10000000,
		STATUS_UNHANDLED_EXCEPTION,
                      1,
                      1,
                      ErrorParameters,
                      OptionOkCancel,
                      &ErrorResponse
                    );
	HeapFree(GetProcessHeap(),0,pwsMsg);
	return ulStatus;
}
