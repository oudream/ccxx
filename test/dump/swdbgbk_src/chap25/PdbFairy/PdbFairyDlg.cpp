// PdbFairyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PdbFairy.h"
#include "PdbFairyDlg.h"

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
// CPdbFairyDlg dialog

CPdbFairyDlg::CPdbFairyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPdbFairyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPdbFairyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPdbFairyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPdbFairyDlg)
	DDX_Control(pDX, IDC_LIST1, m_ListInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPdbFairyDlg, CDialog)
	//{{AFX_MSG_MAP(CPdbFairyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_OPEN, OnOpen)
	ON_BN_CLICKED(ID_HEADER, OnHeader)
	ON_BN_CLICKED(ID_CLEAR, OnClear)
	ON_BN_CLICKED(ID_SIGNATURE, OnSignature)
	ON_BN_CLICKED(ID_STREAMS, OnStreams)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPdbFairyDlg message handlers

BOOL CPdbFairyDlg::OnInitDialog()
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

void CPdbFairyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPdbFairyDlg::OnPaint() 
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
HCURSOR CPdbFairyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPdbFairyDlg::OnOpen() 
{
 	OPENFILENAME  OpenFileName;
 	TCHAR         szFile[MAX_PATH] = "\0";
 	TCHAR         szFilter[MAX_PATH] = "Program Database Files (*.PDB)|*.PDB|All Files (*.*)|*.*|";
 
 	ZeroMemory( &OpenFileName, sizeof( OPENFILENAME ) );
 
 	UINT nLen=strlen(szFilter);
 	for(UINT i=0;i<nLen;i++)
 	{
 		if(szFilter[i]=='|')
 			szFilter[i]='\0';
 	}
 
  	// Fill in the OPENFILENAME structure to support a template and hook.
 	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
	OpenFileName.hwndOwner         = this->m_hWnd;
	OpenFileName.hInstance         = NULL;
	OpenFileName.lpstrFilter       = szFilter;
	OpenFileName.nFilterIndex      = 0;
	OpenFileName.lpstrFile         = szFile;
	OpenFileName.nMaxFile          = sizeof(szFile);
	OpenFileName.lpstrInitialDir   = NULL;
	OpenFileName.lpstrTitle        = _T("Choose PDB Files");
	OpenFileName.lpstrDefExt       = "PDB";
	OpenFileName.Flags             = OFN_ALLOWMULTISELECT | OFN_EXPLORER |OFN_FILEMUSTEXIST;//| OFN_ENABLEHOOK | OFN_ENABLETEMPLATE;
 
 	// Call the common dialog function.
	if (!GetOpenFileName(&OpenFileName))
	{
 		DWORD nErr = CommDlgExtendedError();
		return;
	}
	if(m_PdbMaster.IsLoaded())
		m_PdbMaster.Cleanup();

	m_PdbMaster.Load(OpenFileName.lpstrFile);
}

void CPdbFairyDlg::OnHeader() 
{
	if(S_OK!=m_PdbMaster.DumpHeader(this->m_ListInfo.m_hWnd)
		)
	{
		this->m_ListInfo.AddString(_T("Not a PDB file"));
		return;
	}
}

void CPdbFairyDlg::OnClear() 
{
	m_ListInfo.ResetContent();	
}

void CPdbFairyDlg::OnSignature() 
{
	m_PdbMaster.DumpSignature(m_ListInfo.m_hWnd);
}

void CPdbFairyDlg::OnStreams() 
{
	if(S_OK!=m_PdbMaster.DumpStreamDirectory(m_ListInfo.m_hWnd))
		return;
	
	m_PdbMaster.DumpStreams(m_ListInfo.m_hWnd);
}
