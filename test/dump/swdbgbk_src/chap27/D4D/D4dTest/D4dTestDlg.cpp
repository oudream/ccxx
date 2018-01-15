// D4dTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "D4dTest.h"
#include "D4dTestDlg.h"
#include "../calltracer.h"
#include "../fasttracer.h"
#include "../datatracer.h"

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
// CD4dTestDlg dialog

CD4dTestDlg::CD4dTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CD4dTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CD4dTestDlg)
	m_nInteger = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CD4dTestDlg::~CD4dTestDlg()
{
	if(g_pDataTracer)
		delete g_pDataTracer;
}
void CD4dTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CD4dTestDlg)
	DDX_Control(pDX, IDC_LIST_INFO, m_ListInfo);
	DDX_Text(pDX, IDC_EDIT_INTEGER, m_nInteger);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CD4dTestDlg, CDialog)
	//{{AFX_MSG_MAP(CD4dTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDB_ASSIGN, OnAssign)
	ON_BN_CLICKED(IDB_TRACE, OnTrace)
	ON_BN_CLICKED(IDB_STACKTRACE, OnStacktrace)
	ON_BN_CLICKED(IDB_COPY, OnCopy)
	ON_BN_CLICKED(IDB_CLEAR, OnClear)
	ON_BN_CLICKED(IDB_FAST_TRACE, OnFastTrace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD4dTestDlg message handlers

BOOL CD4dTestDlg::OnInitDialog()
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
	
	if(g_pDataTracer==NULL)
	{
		g_pDataTracer=new CDataTracer();
		g_pDataTracer->SetListener(m_ListInfo.m_hWnd);
		g_pDataTracer->AddVar((ULONG)&m_nInteger, 0, TRUE);
		g_pDataTracer->StartTrace();
	}	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CD4dTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CD4dTestDlg::OnPaint() 
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
HCURSOR CD4dTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CD4dTestDlg::OnAssign() 
{
	//TRACE0("OnAssign"); 
 	//CObject::Dump(afxDump);
	//CDialog::Dump(afxDump);
	if(!UpdateData())
		return;

	m_D4dInteger=this->m_nInteger;
	// m_ListInfo.AddString(_T("Assigned"));
}

void CD4dTestDlg::OnTrace() 
{
	int i=0;
	TCHAR szMsg[MAX_PATH];

	int nCount=m_D4dInteger.GetTrackDepth();
	while(nCount--)
	{
		sprintf(szMsg,"%d",m_D4dInteger.GetTrace(i++));
		this->m_ListInfo.AddString(szMsg);
	}
}
void ShowStackFrame(LPCTSTR szMsg, PVOID pParam)
{
	CD4dTestDlg * pDlg=(CD4dTestDlg *)pParam;
	if(szMsg==NULL)
		pDlg->ShowString(_T("------"));
	else
		pDlg->ShowString(szMsg);
}
void CD4dTestDlg::OnStacktrace() 
{	
	CCallTracer cs;

	cs.WalkStack(ShowStackFrame,this,1000);
}

void CD4dTestDlg::ShowString(LPCTSTR lpszString)
{
	m_ListInfo.AddString(lpszString);
}
const TCHAR szNewLine[]=_T("\r\n");
void CD4dTestDlg::OnCopy() 
{
	int i=0,nCount;
	CSharedFile	sf;
	TCHAR szText[MAX_PATH*2];

	nCount=m_ListInfo.GetCount();
	for(i=0;i<nCount;i++)
	{
		m_ListInfo.GetText(i,szText);
		sf.Write(szText,strlen(szText)*sizeof(TCHAR));
		sf.Write(szNewLine,_tcslen(szNewLine)*sizeof(TCHAR));
	}
	szText[0]=0;
	sf.Write(szText, sizeof(TCHAR));

	SendToClipboard(&sf);	
}

BOOL CD4dTestDlg::SendToClipboard(CSharedFile *pShareFile)
{
 	BOOL bRet=TRUE;
    if (pShareFile->GetLength() <=0) 
	{
		return FALSE;
	}
    // Put the data on the clipboard
    if(!OpenClipboard())
	{
		return FALSE;
	}
    if(!EmptyClipboard())
	{
		return FALSE;
	}
#ifdef _UNICODE
    if(!SetClipboardData(CF_UNICODETEXT, pShareFile->Detach()))
#else
    if(!SetClipboardData(CF_TEXT, pShareFile->Detach()))
#endif
	{
		return FALSE;
	}
    if(!CloseClipboard())
	{
		return FALSE;
	}

	return bRet;
}

void CD4dTestDlg::OnClear() 
{
	m_ListInfo.ResetContent();	
}
const int MAX_TRACE_COUNT=64;
void CD4dTestDlg::OnFastTrace() 
{
	CFastTracer ft;
	DWORD dwRet=0;
	PVOID dwReturnAddress[MAX_TRACE_COUNT];
	TCHAR szMsg[MAX_PATH];

	if(ft.Setup()!=S_OK)
		m_ListInfo.AddString(_T("You may need a higher Windows version."));
	dwRet=ft.GetFrameChain(dwReturnAddress,MAX_TRACE_COUNT);
	for(DWORD i=0;i<dwRet;i++)
	{
		_stprintf(szMsg,_T("Return Address[%d]: 0x%08x"),
			i, dwReturnAddress[i] );
		m_ListInfo.AddString(szMsg);
	}
}
