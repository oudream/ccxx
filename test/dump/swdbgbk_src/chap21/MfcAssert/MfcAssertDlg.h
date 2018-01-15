// MfcAssertDlg.h : header file
//

#if !defined(AFX_MFCASSERTDLG_H__A044E048_A05E_47A4_B6BA_0D8D5B035E4B__INCLUDED_)
#define AFX_MFCASSERTDLG_H__A044E048_A05E_47A4_B6BA_0D8D5B035E4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMfcAssertDlg dialog

class CMfcAssertDlg : public CDialog
{
// Construction
public:
	CMfcAssertDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMfcAssertDlg)
	enum { IDD = IDD_MFCASSERT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcAssertDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMfcAssertDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAssert();
	afx_msg void OnAssertKindof();
	afx_msg void OnVerify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCASSERTDLG_H__A044E048_A05E_47A4_B6BA_0D8D5B035E4B__INCLUDED_)
