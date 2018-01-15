// D4dLogger.h: interface for the CD4dLogger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_D4DLOGGER_H__C4969768_E329_47C5_8E1A_CF812E8C1B97__INCLUDED_)
#define AFX_D4DLOGGER_H__C4969768_E329_47C5_8E1A_CF812E8C1B97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "D4dObject.h"

class CD4dLogger : public CD4dObject  
{
public:
	CD4dLogger();
	virtual ~CD4dLogger();
	virtual DWORD UnitTest (DWORD dwParaFlags); 
	virtual DWORD Dump(HANDLE hFile);
	HRESULT Log(LPCTSTR lpszModule,	UINT nLogType,	LPCTSTR lpszMessage);
};

#endif // !defined(AFX_D4DLOGGER_H__C4969768_E329_47C5_8E1A_CF812E8C1B97__INCLUDED_)
