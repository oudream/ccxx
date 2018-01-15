// FlashWinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FlashWin.h"
#include "FlashWinDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlashWinDlg dialog

CFlashWinDlg::CFlashWinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlashWinDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlashWinDlg)
	m_dwCount = 5;
	m_dwTimeout = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFlashWinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlashWinDlg)
	DDX_Control(pDX, ID_CMD, m_BtnCmd);
	DDX_Control(pDX, IDC_COMBO_FLAGS, m_ComboFlags);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_dwCount);
	DDV_MinMaxUInt(pDX, m_dwCount, 0, 4294967295);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT, m_dwTimeout);
	DDV_MinMaxUInt(pDX, m_dwTimeout, 0, 4294967295);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlashWinDlg, CDialog)
	//{{AFX_MSG_MAP(CFlashWinDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_CMD, OnCmd)
	ON_BN_CLICKED(ID_CMD2, OnCmd2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlashWinDlg message handlers

BOOL CFlashWinDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	this->m_ComboFlags.AddString("FLASHW_STOP");
	this->m_ComboFlags.AddString("FLASHW_CAPTION");
	this->m_ComboFlags.AddString("FLASHW_TRAY");
	this->m_ComboFlags.AddString("FLASHW_ALL");
	this->m_ComboFlags.AddString("FLASHW_TIMER");
	this->m_ComboFlags.AddString("FLASHW_TIMERNOFG");
	this->m_ComboFlags.SetCurSel(3);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFlashWinDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFlashWinDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFlashWinDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFlashWinDlg::OnCmd() 
{
	if(!UpdateData())
		return;
	int nSel=m_ComboFlags.GetCurSel();

	FLASHWINFO fwo;

	fwo.cbSize=sizeof(FLASHWINFO);
	fwo.hwnd=m_hWnd;
	fwo.dwFlags=nSel==5?FLASHW_TIMERNOFG:nSel;
	fwo.uCount=this->m_dwCount;
	fwo.dwTimeout=this->m_dwTimeout;

	if(nSel>=FLASHW_TIMER)
	{
		fwo.dwFlags|=FLASHW_ALL;
		fwo.uCount=0;
	}
	FlashWindowEx(&fwo);
}

void CFlashWinDlg::OnCmd2() 
{
	FlashWindow(TRUE);	
}
