// MulThrds.h : main header file for the MULTHRDS application
//

#if !defined(AFX_MULTHRDS_H__E3DF5798_EC1D_4268_8A93_1BDFDC7D8511__INCLUDED_)
#define AFX_MULTHRDS_H__E3DF5798_EC1D_4268_8A93_1BDFDC7D8511__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMulThrdsApp:
// See MulThrds.cpp for the implementation of this class
//

class CMulThrdsApp : public CWinApp
{
public:
	CMulThrdsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMulThrdsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMulThrdsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTHRDS_H__E3DF5798_EC1D_4268_8A93_1BDFDC7D8511__INCLUDED_)
