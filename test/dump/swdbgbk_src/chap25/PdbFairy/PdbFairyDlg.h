// PdbFairyDlg.h : header file
//

#if !defined(AFX_PDBFAIRYDLG_H__44983391_28F8_4626_8334_7CF65482ECFA__INCLUDED_)
#define AFX_PDBFAIRYDLG_H__44983391_28F8_4626_8334_7CF65482ECFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "pdbmaster.h"

/////////////////////////////////////////////////////////////////////////////
// CPdbFairyDlg dialog

class CPdbFairyDlg : public CDialog
{
// Construction
public:
	CPdbFairyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPdbFairyDlg)
	enum { IDD = IDD_PDBFAIRY_DIALOG };
	CListBox	m_ListInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPdbFairyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CPdbMaster m_PdbMaster;

	// Generated message map functions
	//{{AFX_MSG(CPdbFairyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpen();
	afx_msg void OnHeader();
	afx_msg void OnClear();
	afx_msg void OnSignature();
	afx_msg void OnStreams();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDBFAIRYDLG_H__44983391_28F8_4626_8334_7CF65482ECFA__INCLUDED_)
