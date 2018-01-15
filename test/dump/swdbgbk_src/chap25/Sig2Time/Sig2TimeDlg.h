// SigTimeDlg.h : header file
//

#if !defined(AFX_SIGTIMEDLG_H__4282BB98_7F1F_41CD_A409_754C3A9ECE01__INCLUDED_)
#define AFX_SIGTIMEDLG_H__4282BB98_7F1F_41CD_A409_754C3A9ECE01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSigTimeDlg dialog

class CSig2TimeDlg : public CDialog
{
// Construction
public:
	CSig2TimeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSigTimeDlg)
	enum { IDD = IDD_SIGTIME_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSigTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSigTimeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void On2time();
	afx_msg void On2hex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGTIMEDLG_H__4282BB98_7F1F_41CD_A409_754C3A9ECE01__INCLUDED_)
