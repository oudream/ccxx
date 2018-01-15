// HardError.h : main header file for the HARDERROR application
//

#if !defined(AFX_HARDERROR_H__10166BC9_A8F2_4925_8D5B_99588165FA68__INCLUDED_)
#define AFX_HARDERROR_H__10166BC9_A8F2_4925_8D5B_99588165FA68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHardErrorApp:
// See HardError.cpp for the implementation of this class
//

class CHardErrorApp : public CWinApp
{
public:
	CHardErrorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHardErrorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHardErrorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HARDERROR_H__10166BC9_A8F2_4925_8D5B_99588165FA68__INCLUDED_)
