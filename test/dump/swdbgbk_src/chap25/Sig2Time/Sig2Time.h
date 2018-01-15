// Sig2Time.h : main header file for the SIG2TIME application
//

#if !defined(AFX_SIG2TIME_H__05765E17_8D92_4925_9CA5_462B90FD4C81__INCLUDED_)
#define AFX_SIG2TIME_H__05765E17_8D92_4925_9CA5_462B90FD4C81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSig2TimeApp:
// See Sig2Time.cpp for the implementation of this class
//

class CSig2TimeApp : public CWinApp
{
public:
	CSig2TimeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSig2TimeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSig2TimeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIG2TIME_H__05765E17_8D92_4925_9CA5_462B90FD4C81__INCLUDED_)
