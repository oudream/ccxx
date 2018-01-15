// MulThrdsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MulThrds.h"
#include "MulThrdsDlg.h"

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
// CMulThrdsDlg dialog

CMulThrdsDlg::CMulThrdsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMulThrdsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMulThrdsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMulThrdsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMulThrdsDlg)
	DDX_Control(pDX, IDC_LIST_THREADS, m_ListThreads);
	DDX_Control(pDX, IDC_LIST_INFO, m_ListBox);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMulThrdsDlg, CDialog)
	//{{AFX_MSG_MAP(CMulThrdsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_NEW, OnNew)
	ON_BN_CLICKED(ID_BREAK, OnBreak)
	ON_BN_CLICKED(ID_SUSPEND, OnSuspend)
	ON_BN_CLICKED(ID_RESUME, OnResume)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMulThrdsDlg message handlers

BOOL CMulThrdsDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMulThrdsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMulThrdsDlg::OnPaint() 
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
HCURSOR CMulThrdsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
DWORD WINAPI DbgeeThreadProc(
  LPVOID lpParameter
)
{
	TCHAR szMsg[MAX_PATH];
	CMulThrdsDlg * pDlg=(CMulThrdsDlg*)lpParameter;
	while(TRUE)
	{
		sprintf(szMsg,_T("Thread %d is runing."),GetCurrentThreadId());
		pDlg->m_ListBox.InsertString(0,szMsg);
		Sleep(1000);
	}
}

void CMulThrdsDlg::OnNew() 
{
	HANDLE hThread;
	DWORD dwThreadID=0;
	TCHAR szMsg[MAX_PATH];
	
	hThread=CreateThread(NULL,0,DbgeeThreadProc,this,0,&dwThreadID);
	if(hThread==NULL)
	{
		m_ListBox.InsertString(0,_T("Create thread failed"));
		return;
	}
	sprintf(szMsg,_T("%d(0x%x)"),dwThreadID,dwThreadID);

	m_ListThreads.AddString(szMsg);
	CloseHandle(hThread);
}

void CMulThrdsDlg::OnBreak() 
{
	DebugBreak();	
}

void CMulThrdsDlg::OnSuspend() 
{
	SusOrRsmThread();
}

void CMulThrdsDlg::OnResume() 
{
	SusOrRsmThread(FALSE);	
}

VOID CMulThrdsDlg::SusOrRsmThread(BOOL bSuspend)
{
	int nSel;
	DWORD dwThreadID,dwSuspendCount;
	TCHAR szBuffer[MAX_PATH];
	HANDLE hThread;

	nSel=m_ListThreads.GetCurSel();
	if(nSel==LB_ERR )
	{	
		m_ListBox.InsertString(0,_T("Please choose the thread in above list to operate."));
		return;
	}
	m_ListThreads.GetText(nSel, szBuffer);
	dwThreadID=atoi(szBuffer);
	hThread=OpenThread(THREAD_SUSPEND_RESUME,FALSE,dwThreadID);
	if(hThread==NULL)
	{
		sprintf(szBuffer,_T("Open thread %d failed."),dwThreadID);
		m_ListBox.InsertString(0,szBuffer);
		return;
	}
	if(bSuspend)
	{
		dwSuspendCount=SuspendThread(hThread);
	}
	else
		dwSuspendCount=ResumeThread(hThread);
	
	sprintf(szBuffer,_T("Previous suspend count of thread %d is %d."),
		dwThreadID,dwSuspendCount);
	m_ListBox.InsertString(0,szBuffer);
	CloseHandle(hThread);
}
