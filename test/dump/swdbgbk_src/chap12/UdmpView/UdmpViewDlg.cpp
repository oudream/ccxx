/*---------------------------------------------------------------------
MdmpParser.cpp - An utility to read user mode dump file.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include "stdafx.h"
#include "UdmpView.h"
#include "UdmpViewDlg.h"

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
// CUdmpViewDlg dialog

CUdmpViewDlg::CUdmpViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdmpViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUdmpViewDlg)
	m_strRva = _T("");
	m_strSize = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUdmpViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUdmpViewDlg)
	DDX_Control(pDX, IDC_LIST_STREAM, m_ListStreams);
	DDX_Control(pDX, IDC_LIST_INFO, m_ListInfo);
	DDX_Text(pDX, IDC_EDIT_RVA, m_strRva);
	DDV_MaxChars(pDX, m_strRva, 30);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_strSize);
	DDV_MaxChars(pDX, m_strSize, 30);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUdmpViewDlg, CDialog)
	//{{AFX_MSG_MAP(CUdmpViewDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_OPEN, OnOpen)
	ON_BN_CLICKED(ID_SELECT_ALL, OnSelectAll)
	ON_BN_CLICKED(ID_SHOW, OnShow)
	ON_BN_CLICKED(ID_CLOSE, OnClose)
	ON_BN_CLICKED(ID_COPY, OnCopy)
	ON_BN_CLICKED(ID_CLEAR, OnClear)
	ON_BN_CLICKED(ID_COPY_ALL, OnCopyAll)
	ON_WM_SIZE()
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(ID_RAW_READ, OnRawRead)
	ON_LBN_DBLCLK(IDC_LIST_STREAM, OnDblclkListStream)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUdmpViewDlg message handlers

BOOL CUdmpViewDlg::OnInitDialog()
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
	this->m_MdmpParser.SetListener(this->m_ListInfo.m_hWnd);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUdmpViewDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUdmpViewDlg::OnPaint() 
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
HCURSOR CUdmpViewDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUdmpViewDlg::OnOpen() 
{
	TCHAR szFileName[MAX_PATH]="";
	HRESULT hRet;

	OPENFILENAME  OpenFileName;
	CString		  strTitle;	
	TCHAR         szFilter[MAX_PATH] = "User Mode Dump Files (*.DMP)|*.DMP|*.MDMP|*.MDMP|All Files (*.*)|*.*|";

	if(this->m_MdmpParser.IsOpen())
	{
		this->m_ListInfo.AddString("Please close current file before opening the other.");
		return;
	}
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
	OpenFileName.hInstance         = NULL; //theApp.m_hInstance; //NULL;
	OpenFileName.lpstrFilter       = szFilter;
	OpenFileName.nFilterIndex      = 0;
	OpenFileName.lpstrFile         = szFileName;
	OpenFileName.nMaxFile          = sizeof(szFileName);
	OpenFileName.lpstrInitialDir   = NULL;
	OpenFileName.lpstrTitle        = "Select a dump file to view.";
	OpenFileName.lpstrDefExt       = "DMP";
	OpenFileName.Flags             = OFN_EXPLORER |OFN_FILEMUSTEXIST;
	//| OFN_ENABLEHOOK | OFN_ENABLETEMPLATE;

	// Call the common dialog function.
	if (!GetOpenFileName(&OpenFileName))
	{
		return;
	}

	hRet=m_MdmpParser.Open(szFileName,this->m_ListStreams.m_hWnd);
	if(hRet!=S_OK)
	{
		m_ListInfo.AddString("Failed in opening file:");
		m_ListInfo.AddString(szFileName);
	}
	this->EnableControls(m_MdmpParser.IsOpen());
}

void CUdmpViewDlg::OnSelectAll() 
{
	this->m_ListStreams.SelItemRange(TRUE,0,
		m_ListStreams.GetCount()-1);	
}

void CUdmpViewDlg::OnShow() 
{
	m_MdmpParser.ReadStreams(m_ListStreams.m_hWnd);
}

void CUdmpViewDlg::OnClose() 
{
	this->m_MdmpParser.Close();	
	this->EnableControls(m_MdmpParser.IsOpen());
}
const TCHAR szNewLine[]=_T("\r\n");
void CUdmpViewDlg::OnCopy() 
{
	int iSel;
	CSharedFile	sf;
	TCHAR szText[MAX_PATH*2];

	iSel=m_ListInfo.GetCurSel();
	if(iSel==LB_ERR)
	{
		m_ListInfo.InsertString(0,"Please select an item.");
		return;
	}
	m_ListInfo.GetText(iSel,szText);
	sf.Write(szText,strlen(szText)*sizeof(TCHAR));
	sf.Write(szNewLine,_tcslen(szNewLine)*sizeof(TCHAR));

	szText[0]=0;
	sf.Write(szText, sizeof(TCHAR));

	SendToClipboard(&sf);	
}

void CUdmpViewDlg::OnClear() 
{
	this->m_ListInfo.ResetContent();	
}

void CUdmpViewDlg::OnCopyAll() 
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
BOOL CUdmpViewDlg::SendToClipboard(CSharedFile *pShareFile)
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

void CUdmpViewDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
}
void CUdmpViewDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

	RECT rc;
	GetClientRect(&rc);

	OffsetChild(IDC_LIST_STREAM,rc,TRUE,FALSE);
	OffsetChild(IDC_LIST_INFO,rc,TRUE,TRUE);
}
const int GAP=8;
const int BTN_WIDTH=0;
void CUdmpViewDlg::OffsetChild(UINT nChild, 
			RECT &rcClient,BOOL bExtendRight,BOOL bExtendDown)
{
	CWnd * pChild;
	RECT rcChild;

	pChild=GetDlgItem(nChild);
	if(pChild==NULL)
		return;

	pChild->GetWindowRect(&rcChild);
	ScreenToClient(&rcChild);

	if(bExtendRight)
	{
		rcChild.right=rcClient.right-BTN_WIDTH-GAP*2;
	}

	if(bExtendDown)
	{
		rcChild.bottom=rcClient.bottom-GAP;
	}

	pChild->MoveWindow(&rcChild);

}

void CUdmpViewDlg::EnableControls(BOOL bIsOpen)
{
	CWnd * pChild;
	DWORD dwaIDs[]={ID_SHOW,ID_CLOSE,ID_RAW_READ};
	int nIDs=sizeof(dwaIDs)/sizeof(DWORD);

	pChild=GetDlgItem(ID_OPEN);
	pChild->EnableWindow(!bIsOpen);
	for(int i=0;i<nIDs;i++)
	{
		pChild=GetDlgItem(dwaIDs[i]);
		if(pChild)
			pChild->EnableWindow(bIsOpen);
	}
}

void CUdmpViewDlg::OnRawRead() 
{
	ULONG ulRva,ulSize;
	if(!UpdateData())
		return;
	ulRva=_tcstol(this->m_strRva,NULL,0);
	ulSize=_tcstol(this->m_strSize,NULL,0);
	m_MdmpParser.RawRead(ulRva,ulSize);
}

void CUdmpViewDlg::OnDblclkListStream() 
{
	// TODO: Add your control notification handler code here
	
}
