// D4dTestDlg.h : header file
//

#if !defined(AFX_D4DTESTDLG_H__044BD836_4C5C_4DB3_B3A5_BC2D3E1559BF__INCLUDED_)
#define AFX_D4DTESTDLG_H__044BD836_4C5C_4DB3_B3A5_BC2D3E1559BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../d4dinteger.h"
#include <afxadv.h>

/////////////////////////////////////////////////////////////////////////////
// CD4dTestDlg dialog

class CD4dTestDlg : public CDialog
{
// Construction
public:
	BOOL SendToClipboard(CSharedFile * pShareFile);
	void ShowString(LPCTSTR lpszString);
	CD4dTestDlg(CWnd* pParent = NULL);	// standard constructor
	~CD4dTestDlg();
// Dialog Data
	//{{AFX_DATA(CD4dTestDlg)
	enum { IDD = IDD_D4DTEST_DIALOG };
	CListBox	m_ListInfo;
	long	m_nInteger;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD4dTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CD4dInteger m_D4dInteger;

	// Generated message map functions
	//{{AFX_MSG(CD4dTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAssign();
	afx_msg void OnTrace();
	afx_msg void OnStacktrace();
	afx_msg void OnCopy();
	afx_msg void OnClear();
	afx_msg void OnFastTrace();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D4DTESTDLG_H__044BD836_4C5C_4DB3_B3A5_BC2D3E1559BF__INCLUDED_)
