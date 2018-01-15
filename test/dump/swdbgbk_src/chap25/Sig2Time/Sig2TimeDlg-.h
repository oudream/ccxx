// Sig2TimeDlg.h : header file
//

#if !defined(AFX_SIG2TIMEDLG_H__EEA96EC9_70F2_4673_B714_CFFBC3447F6B__INCLUDED_)
#define AFX_SIG2TIMEDLG_H__EEA96EC9_70F2_4673_B714_CFFBC3447F6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSig2TimeDlg dialog

class CSig2TimeDlg : public CDialog
{
// Construction
public:
	CSig2TimeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSig2TimeDlg)
	enum { IDD = IDD_SIG2TIME_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSig2TimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSig2TimeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIG2TIMEDLG_H__EEA96EC9_70F2_4673_B714_CFFBC3447F6B__INCLUDED_)
