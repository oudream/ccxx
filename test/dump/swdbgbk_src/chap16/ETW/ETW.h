// ETW.h : main header file for the ETW application
//

#if !defined(AFX_ETW_H__4FF5AFBC_B461_4F8D_9C83_CAE96CD6C906__INCLUDED_)
#define AFX_ETW_H__4FF5AFBC_B461_4F8D_9C83_CAE96CD6C906__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CETWApp:
// See ETW.cpp for the implementation of this class
//

class CETWApp : public CWinApp
{
public:
	CETWApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETWApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CETWApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETW_H__4FF5AFBC_B461_4F8D_9C83_CAE96CD6C906__INCLUDED_)
