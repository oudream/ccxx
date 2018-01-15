// FlashWinDlg.h : header file
//

#if !defined(AFX_FLASHWINDLG_H__9EEC7A26_C38E_4EF7_AB6C_36695938DFA7__INCLUDED_)
#define AFX_FLASHWINDLG_H__9EEC7A26_C38E_4EF7_AB6C_36695938DFA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFlashWinDlg dialog

class CFlashWinDlg : public CDialog
{
// Construction
public:
	CFlashWinDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFlashWinDlg)
	enum { IDD = IDD_FLASHWIN_DIALOG };
	CButton	m_BtnCmd;
	CComboBox	m_ComboFlags;
	UINT	m_dwCount;
	UINT	m_dwTimeout;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlashWinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFlashWinDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCmd();
	afx_msg void OnCmd2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLASHWINDLG_H__9EEC7A26_C38E_4EF7_AB6C_36695938DFA7__INCLUDED_)
