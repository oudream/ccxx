// HeapMfcDlg.h : header file
//

#if !defined(AFX_HEAPMFCDLG_H__7B52F1E0_CFDA_4FA9_8606_44B2C6C4922E__INCLUDED_)
#define AFX_HEAPMFCDLG_H__7B52F1E0_CFDA_4FA9_8606_44B2C6C4922E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHeapMfcDlg dialog

class CHeapMfcDlg : public CDialog
{
// Construction
public:
	CHeapMfcDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHeapMfcDlg)
	enum { IDD = IDD_HEAPMFC_DIALOG };
	CListBox	m_ListBox;
	long	m_ulSize;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeapMfcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHeapMfcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNew();
	afx_msg void OnChangeEditSize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEAPMFCDLG_H__7B52F1E0_CFDA_4FA9_8606_44B2C6C4922E__INCLUDED_)
