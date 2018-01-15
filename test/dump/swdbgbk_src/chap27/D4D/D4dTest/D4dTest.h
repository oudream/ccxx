// D4dTest.h : main header file for the D4DTEST application
//

#if !defined(AFX_D4DTEST_H__991EC05B_F3B6_4A7B_8D44_7B433CCF010D__INCLUDED_)
#define AFX_D4DTEST_H__991EC05B_F3B6_4A7B_8D44_7B433CCF010D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CD4dTestApp:
// See D4dTest.cpp for the implementation of this class
//

class CD4dTestApp : public CWinApp
{
public:
	CD4dTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD4dTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CD4dTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D4DTEST_H__991EC05B_F3B6_4A7B_8D44_7B433CCF010D__INCLUDED_)
