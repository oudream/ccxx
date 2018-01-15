// NKLMgr.h: interface for the CNKLMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NKLMGR_H__45411EB1_FBB6_49F6_B8EE_806A4A7C7338__INCLUDED_)
#define AFX_NKLMGR_H__45411EB1_FBB6_49F6_B8EE_806A4A7C7338__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define INITGUID  //Include this #define to use SystemTraceControlGuid in Evntrace.h.
#include <Wmistr.h>
#include <initguid.h>
#include <guiddef.h>
#include <evntrace.h>

class CNKLMgr  
{
public:
	BOOL StopSession(LPCTSTR szSessionName);
	BOOL StopNKL();
	void SetListBox(CListBox * pMsgList);
	BOOL StartNKL(LPCTSTR lpszLogFile,DWORD dwEnableFlags);
	void OUTMSG(LPCTSTR szFormat, ...);
	CNKLMgr();
	virtual ~CNKLMgr();
protected:
	TRACEHANDLE m_hNklSessionHandle;
	CListBox * m_pListBox;
	EVENT_TRACE_PROPERTIES* m_pNklProperties;
};

#endif // !defined(AFX_NKLMGR_H__45411EB1_FBB6_49F6_B8EE_806A4A7C7338__INCLUDED_)
