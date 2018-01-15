// EvtLogDlg.h : header file
//

#if !defined(AFX_EVTLOGDLG_H__4DA18215_508D_4F79_8182_F2E55F190B07__INCLUDED_)
#define AFX_EVTLOGDLG_H__4DA18215_508D_4F79_8182_F2E55F190B07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "EventLog.h"
/////////////////////////////////////////////////////////////////////////////
// CEvtLogDlg dialog

class CEvtLogDlg : public CDialog
{
// Construction
public:
	CEvtLogDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CEvtLogDlg)
	enum { IDD = IDD_EVTLOG_DIALOG };
	CListBox	m_ListInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEvtLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CEventLog m_EvtLog;

	// Generated message map functions
	//{{AFX_MSG(CEvtLogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLog();
	afx_msg void OnLogOrphan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVTLOGDLG_H__4DA18215_508D_4F79_8182_F2E55F190B07__INCLUDED_)
