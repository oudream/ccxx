// verifiee.h : main header file for the VERIFIEE application
//

#if !defined(AFX_VERIFIEE_H__3FB12F81_7939_445C_8078_CFB48FB4698C__INCLUDED_)
#define AFX_VERIFIEE_H__3FB12F81_7939_445C_8078_CFB48FB4698C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVerifieeApp:
// See verifiee.cpp for the implementation of this class
//

class CVerifieeApp : public CWinApp
{
public:
	CVerifieeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVerifieeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVerifieeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERIFIEE_H__3FB12F81_7939_445C_8078_CFB48FB4698C__INCLUDED_)
