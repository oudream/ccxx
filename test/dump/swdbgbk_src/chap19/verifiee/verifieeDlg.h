// verifieeDlg.h : header file
//

#if !defined(AFX_VERIFIEEDLG_H__BEB752A1_6B29_425E_8BBD_57BC4BFC5A94__INCLUDED_)
#define AFX_VERIFIEEDLG_H__BEB752A1_6B29_425E_8BBD_57BC4BFC5A94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "bugs.h"

/////////////////////////////////////////////////////////////////////////////
// CVerifieeDlg dialog

class CVerifieeDlg : public CDialog
{
// Construction
public:
	CVerifieeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVerifieeDlg)
	enum { IDD = IDD_VERIFIEE_DIALOG };
	CListBox	m_ListBox;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVerifieeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CBugs m_Bugs;
	// Generated message map functions
	//{{AFX_MSG(CVerifieeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBang();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERIFIEEDLG_H__BEB752A1_6B29_425E_8BBD_57BC4BFC5A94__INCLUDED_)
