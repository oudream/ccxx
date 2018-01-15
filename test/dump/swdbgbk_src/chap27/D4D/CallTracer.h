// CallTracer.h: interface for the CCallTracer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALLTRACER_H__011E66DE_47A6_4BFA_A4B2_70048EBEA677__INCLUDED_)
#define AFX_CALLTRACER_H__011E66DE_47A6_4BFA_A4B2_70048EBEA677__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "D4dObject.h"
#include <Dbghelp.h>

#define CALLTRACE_OPT_INFO_CHILDEBP           0x1
#define CALLTRACE_OPT_INFO_SEPT               0x4
#define CALLTRACE_OPT_INFO_MODULE_FUNC        0x8
#define CALLTRACE_OPT_INFO_PARA               0x10
#define CALLTRACE_OPT_INFO_SRCLINE            0x40
#define CALLTRACE_OPT_INFO_MODULE_SYMBOL      0x80
#define CALLTRACE_OPT_INFO_FPO                0x100
#define CALLTRACE_OPT_INFO_MISC               0x400
#define CALLTRACE_OPT_INFO_SUMMARY            0x800
#define CALLTRACE_OPT_INFO_SEARCHPATH         0x1000
#define CALLTRACE_OPT_INFO_LEAN               CALLTRACE_OPT_INFO_MODULE_FUNC
#define CALLTRACE_OPT_INFO_ALL                0xFFFFFFFF

typedef void (* PFN_SHOWFRAME)(LPCTSTR szText,PVOID pParam);
class D4D_API CCallTracer //: public CD4dObject  
{
public:
	LPCTSTR GetSymType(DWORD dwSymType);
	HRESULT ShowFrame(STACKFRAME64 * pFrame,
		PFN_SHOWFRAME pfnShowFrame, PVOID pParam);
	HRESULT WalkStack(PFN_SHOWFRAME pfnShowFrame, 
		PVOID pParam,int nMaxFrames,PCONTEXT pContext=NULL);
	HRESULT FreeSymbols();
	DWORD GetOptions(){return m_dwOptions;}
	void SetOptions(DWORD dwOptions){m_dwOptions=dwOptions;}
	HRESULT InitSymbols();
	CCallTracer();
	virtual ~CCallTracer();
	static DWORD m_dwSymClients; // counter of symbol clients 
protected:
	CONTEXT m_Context;
	DWORD   m_dwOptions;
};

#endif // !defined(AFX_CALLTRACER_H__011E66DE_47A6_4BFA_A4B2_70048EBEA677__INCLUDED_)
