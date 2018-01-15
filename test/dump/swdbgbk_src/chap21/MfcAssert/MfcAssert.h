// MfcAssert.h : main header file for the MFCASSERT application
//

#if !defined(AFX_MFCASSERT_H__5B31595B_8EF9_4A86_83C0_8C1CA6B9C09E__INCLUDED_)
#define AFX_MFCASSERT_H__5B31595B_8EF9_4A86_83C0_8C1CA6B9C09E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMfcAssertApp:
// See MfcAssert.cpp for the implementation of this class
//

class CMfcAssertApp : public CWinApp
{
public:
	CMfcAssertApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcAssertApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMfcAssertApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCASSERT_H__5B31595B_8EF9_4A86_83C0_8C1CA6B9C09E__INCLUDED_)
