// PdbFairy.h : main header file for the PDBFAIRY application
//

#if !defined(AFX_PDBFAIRY_H__0DEAC4D0_96EC_4E29_BF44_80886C83B2F1__INCLUDED_)
#define AFX_PDBFAIRY_H__0DEAC4D0_96EC_4E29_BF44_80886C83B2F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPdbFairyApp:
// See PdbFairy.cpp for the implementation of this class
//

class CPdbFairyApp : public CWinApp
{
public:
	CPdbFairyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPdbFairyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPdbFairyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDBFAIRY_H__0DEAC4D0_96EC_4E29_BF44_80886C83B2F1__INCLUDED_)
