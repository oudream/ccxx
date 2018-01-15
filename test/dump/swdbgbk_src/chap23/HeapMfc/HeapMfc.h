// HeapMfc.h : main header file for the HEAPMFC application
//

#if !defined(AFX_HEAPMFC_H__69EC8A7C_6907_4CD9_B4D0_F9DC450DCF77__INCLUDED_)
#define AFX_HEAPMFC_H__69EC8A7C_6907_4CD9_B4D0_F9DC450DCF77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHeapMfcApp:
// See HeapMfc.cpp for the implementation of this class
//

class CHeapMfcApp : public CWinApp
{
public:
	CHeapMfcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeapMfcApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHeapMfcApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEAPMFC_H__69EC8A7C_6907_4CD9_B4D0_F9DC450DCF77__INCLUDED_)
