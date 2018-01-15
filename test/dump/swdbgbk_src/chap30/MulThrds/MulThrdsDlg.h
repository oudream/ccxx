// MulThrdsDlg.h : header file
//

#if !defined(AFX_MULTHRDSDLG_H__85A222DF_3D5D_4D1D_AD9A_914CACACF4E2__INCLUDED_)
#define AFX_MULTHRDSDLG_H__85A222DF_3D5D_4D1D_AD9A_914CACACF4E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMulThrdsDlg dialog

class CMulThrdsDlg : public CDialog
{
// Construction
public:
	CMulThrdsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMulThrdsDlg)
	enum { IDD = IDD_MULTHRDS_DIALOG };
	CListBox	m_ListThreads;
	CListBox	m_ListBox;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMulThrdsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	VOID SusOrRsmThread(BOOL bSuspend=TRUE);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMulThrdsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNew();
	afx_msg void OnBreak();
	afx_msg void OnSuspend();
	afx_msg void OnResume();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTHRDSDLG_H__85A222DF_3D5D_4D1D_AD9A_914CACACF4E2__INCLUDED_)
