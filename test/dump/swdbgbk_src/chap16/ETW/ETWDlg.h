// ETWDlg.h : header file
//

#if !defined(AFX_ETWDLG_H__66B25AC2_9687_4ACB_AD90_4D4472B031F9__INCLUDED_)
#define AFX_ETWDLG_H__66B25AC2_9687_4ACB_AD90_4D4472B031F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "nklmgr.h"

/////////////////////////////////////////////////////////////////////////////
// CETWDlg dialog

class CETWDlg : public CDialog
{
// Construction
public:
	CETWDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CETWDlg)
	enum { IDD = IDD_ETW_DIALOG };
	CListBox	m_ListBox;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETWDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CNKLMgr m_NklMgr;
	// Generated message map functions
	//{{AFX_MSG(CETWDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnStartNkl();
	afx_msg void OnStopNkl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETWDLG_H__66B25AC2_9687_4ACB_AD90_4D4472B031F9__INCLUDED_)
