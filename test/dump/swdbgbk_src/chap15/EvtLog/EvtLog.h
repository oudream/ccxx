// EvtLog.h : main header file for the EVTLOG application
//

#if !defined(AFX_EVTLOG_H__93918978_2393_45BF_B4AA_94B0A5DBA1E5__INCLUDED_)
#define AFX_EVTLOG_H__93918978_2393_45BF_B4AA_94B0A5DBA1E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEvtLogApp:
// See EvtLog.cpp for the implementation of this class
//

class CEvtLogApp : public CWinApp
{
public:
	CEvtLogApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEvtLogApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEvtLogApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVTLOG_H__93918978_2393_45BF_B4AA_94B0A5DBA1E5__INCLUDED_)
