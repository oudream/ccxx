// FlashWin.h : main header file for the FLASHWIN application
//

#if !defined(AFX_FLASHWIN_H__49C29B90_1907_4DFD_8DBF_19191D5CA42E__INCLUDED_)
#define AFX_FLASHWIN_H__49C29B90_1907_4DFD_8DBF_19191D5CA42E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFlashWinApp:
// See FlashWin.cpp for the implementation of this class
//

class CFlashWinApp : public CWinApp
{
public:
	CFlashWinApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlashWinApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFlashWinApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLASHWIN_H__49C29B90_1907_4DFD_8DBF_19191D5CA42E__INCLUDED_)
