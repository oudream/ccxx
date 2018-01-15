// UdmpViewDlg.h : header file
//

#if !defined(AFX_UDMPVIEWDLG_H__0A9ABCAA_A7DC_4F8E_B8E5_FF3916FF6187__INCLUDED_)
#define AFX_UDMPVIEWDLG_H__0A9ABCAA_A7DC_4F8E_B8E5_FF3916FF6187__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "mdmpparser.h" 
#include <afxadv.h>

/////////////////////////////////////////////////////////////////////////////
// CUdmpViewDlg dialog

class CUdmpViewDlg : public CDialog
{
// Construction
public:
	void EnableControls(BOOL bIsOpen);
	CUdmpViewDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUdmpViewDlg)
	enum { IDD = IDD_UDMPVIEW_DIALOG };
	CListBox	m_ListStreams;
	CListBox	m_ListInfo;
	CString	m_strRva;
	CString	m_strSize;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUdmpViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	BOOL SendToClipboard(CSharedFile *pShareFile);
	void OffsetChild(UINT nChild, RECT &rcClient,
		BOOL bExtendRight,BOOL bExtendDown);

// Implementation
protected:
	HICON m_hIcon;
	CMdmpParser m_MdmpParser;
	// Generated message map functions
	//{{AFX_MSG(CUdmpViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpen();
	afx_msg void OnSelectAll();
	afx_msg void OnShow();
	afx_msg void OnClose();
	afx_msg void OnCopy();
	afx_msg void OnClear();
	afx_msg void OnCopyAll();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCancelMode();
	afx_msg void OnRawRead();
	afx_msg void OnDblclkListStream();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDMPVIEWDLG_H__0A9ABCAA_A7DC_4F8E_B8E5_FF3916FF6187__INCLUDED_)
