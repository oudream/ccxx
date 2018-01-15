// UdmpView.h : main header file for the UDMPVIEW application
//

#if !defined(AFX_UDMPVIEW_H__BFE3572F_F751_4CFB_B759_E0E37603C4DB__INCLUDED_)
#define AFX_UDMPVIEW_H__BFE3572F_F751_4CFB_B759_E0E37603C4DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUdmpViewApp:
// See UdmpView.cpp for the implementation of this class
//

class CUdmpViewApp : public CWinApp
{
public:
	CUdmpViewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUdmpViewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUdmpViewApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDMPVIEW_H__BFE3572F_F751_4CFB_B759_E0E37603C4DB__INCLUDED_)
