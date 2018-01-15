// HardErrorRaiser.h: interface for the CHardErrorRaiser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HARDERRORRAISER_H__AB42F036_415E_415A_A191_FE345BDBB71F__INCLUDED_)
#define AFX_HARDERRORRAISER_H__AB42F036_415E_415A_A191_FE345BDBB71F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>

typedef ULONG (WINAPI *NTRAISEHARDERROR)(ULONG ulStatus, 
										 ULONG ulParaNum,
										 ULONG ulParaMask,
										 PULONG lpParas,
										 UINT uResOptions,
										 PULONG pdwResponse);
typedef enum _HARDERROR_RESPONSE {
    ResponseReturnToCaller,
    ResponseNotHandled,
    ResponseAbort,
    ResponseCancel,
    ResponseIgnore,
    ResponseNo,
    ResponseOk,
    ResponseRetry,
    ResponseYes
} HARDERROR_RESPONSE, *PHARDERROR_RESPONSE;
typedef enum _HARDERROR_RESPONSE_OPTION 
{
    OptionAbortRetryIgnore,
    OptionOk,
    OptionOkCancel,
    OptionRetryCancel,
    OptionYesNo,
    OptionYesNoCancel,
    OptionShutdownSystem,
    OptionOkNoWait,
    OptionCancelTryContinue
} HARDERROR_RESPONSE_OPTION, *PHARDERROR_RESPONSE_OPTION;

class CHardErrorRaiser  
{
public:
	ULONG HardError(ULONG ulResOptions,LPCTSTR szMsg,PULONG pulResponse);
	BOOL Init();
	CHardErrorRaiser();
	virtual ~CHardErrorRaiser();
	void DbgMsg(LPCTSTR szFormat, ...);
	void SetInfoList(CListBox * pList){m_pListBox=pList;}
protected:
	NTRAISEHARDERROR m_pfnNtRaiseHardError;
	CListBox * m_pListBox;
};

#endif // !defined(AFX_HARDERRORRAISER_H__AB42F036_415E_415A_A191_FE345BDBB71F__INCLUDED_)
