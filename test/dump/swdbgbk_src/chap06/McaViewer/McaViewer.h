// McaViewer.h : main header file for the MCAVIEWER application
//

#if !defined(AFX_MCAVIEWER_H__9E0675ED_E834_459F_976E_F976CD34771A__INCLUDED_)
#define AFX_MCAVIEWER_H__9E0675ED_E834_459F_976E_F976CD34771A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMcaViewerApp:
// See McaViewer.cpp for the implementation of this class
//

class CMcaViewerApp : public CWinApp
{
public:
	CMcaViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMcaViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMcaViewerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCAVIEWER_H__9E0675ED_E834_459F_976E_F976CD34771A__INCLUDED_)
