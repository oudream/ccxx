// HardErrorDlg.h : header file
//

#if !defined(AFX_HARDERRORDLG_H__06E2616D_EBB1_4CE5_AF0E_C04A05C9A883__INCLUDED_)
#define AFX_HARDERRORDLG_H__06E2616D_EBB1_4CE5_AF0E_C04A05C9A883__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "harderrorraiser.h"

/////////////////////////////////////////////////////////////////////////////
// CHardErrorDlg dialog

class CHardErrorDlg : public CDialog
{
// Construction
public:
	CHardErrorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHardErrorDlg)
	enum { IDD = IDD_HARDERROR_DIALOG };
	CComboBox	m_ComboOptions;
	CListBox	m_ListBox;
	CString	m_strEditMsg;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHardErrorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CHardErrorRaiser m_HardErrorRaiser;

	// Generated message map functions
	//{{AFX_MSG(CHardErrorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HARDERRORDLG_H__06E2616D_EBB1_4CE5_AF0E_C04A05C9A883__INCLUDED_)
