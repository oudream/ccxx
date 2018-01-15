// McaViewerDlg.h : header file
//

#if !defined(AFX_MCAVIEWERDLG_H__771BCF30_1230_40F8_B2F1_1A0C5158D7A7__INCLUDED_)
#define AFX_MCAVIEWERDLG_H__771BCF30_1230_40F8_B2F1_1A0C5158D7A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "mcapoller.h"

/////////////////////////////////////////////////////////////////////////////
// CMcaViewerDlg dialog

class CMcaViewerDlg : public CDialog
{
// Construction
public:
	CMcaViewerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMcaViewerDlg)
	enum { IDD = IDD_MCAVIEWER_DIALOG };
	CListBox	m_ListBox;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMcaViewerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CMcaPoller m_McaPoller;
	// Generated message map functions
	//{{AFX_MSG(CMcaViewerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCheckPoll();
	afx_msg void OnDetect();
	afx_msg void OnClear();
	afx_msg void OnPoll();
	afx_msg void OnClearBank();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCAVIEWERDLG_H__771BCF30_1230_40F8_B2F1_1A0C5158D7A7__INCLUDED_)
