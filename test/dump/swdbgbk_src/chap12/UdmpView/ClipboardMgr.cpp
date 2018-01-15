// ClipboardMgr.cpp: implementation of the CClipboardMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UdmpView.h"
#include "ClipboardMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClipboardMgr::CClipboardMgr()
:m_ShareFile(GMEM_MOVEABLE | GMEM_SHARE | GMEM_ZEROINIT)
{

}

CClipboardMgr::~CClipboardMgr()
{

}

BOOL CClipboardMgr::Write(const void *lpData, int nBytes)
{
	BOOL bRet=TRUE;
	try
	{
		m_ShareFile.Write(lpData, nBytes);
	}catch(...)
	{
		Msg(_T("Exception raised in CClipboardMgr::Write."));
		bRet=FALSE;
	}
	return bRet;
}

BOOL CClipboardMgr::SendToClipboard()
{
	BOOL bRet=TRUE;
    if (m_ShareFile.GetLength() <=0) 
	{
		Msg(_T("No data to send to clipboard."));
		return FALSE;
	}
    // Put the data on the clipboard
    if(!OpenClipboard(NULL))
	{
		Msg(_T("OpenClipboard failed for %d."),GetLastError());
		return FALSE;
	}
    if(!EmptyClipboard())
	{
		Msg(_T("EmptyClipboard failed for %d."),GetLastError());
		return FALSE;
	}
    if(!SetClipboardData(GetClipboardFormat(), m_ShareFile.Detach()))
	{
		Msg(_T("SetClipboardData failed for %d."),GetLastError());
		return FALSE;
	}
    if(!CloseClipboard())
	{
		Msg(_T("CloseClipboard failed for %d."),GetLastError());
		return FALSE;
	}
    
	return bRet;
}

BOOL CClipboardMgr::DumpList(HWND hWndList,DWORD dwOption)
{
	TCHAR szBuffer[MAX_PATH];
	TCHAR szTab[]=_T("\t");

	int nItemCount=ListView_GetItemCount(hWndList);
	if(nItemCount<=0)
	{
		Msg(_T("List [%x] is empty."),hWndList);
		return FALSE;
	}
	if(dwOption& CB_OPTION_ONLY_SELECTED
		&&	ListView_GetSelectedCount(hWndList)<1)
	{
		Msg(_T("List [%x] has no selection."),hWndList);
		return FALSE;
	}
	int nColumnCount=Header_GetItemCount(ListView_GetHeader(hWndList));
	for(int i=0;i<nItemCount;i++)
	{
		if(dwOption& CB_OPTION_ONLY_SELECTED
			&&	ListView_GetItemState(hWndList, i, LVIS_SELECTED) != LVIS_SELECTED)
			continue;

		for(int j=0;j<nColumnCount;j++)
		{
			szBuffer[0]=0;
			ListView_GetItemText(hWndList,i,j,szBuffer,MAX_PATH);
			if(szBuffer[0]!=0)
			{
				this->Write(szBuffer,_tcslen(szBuffer)*sizeof(TCHAR));
			}
			this->Write(szTab,_tcslen(szTab)*sizeof(TCHAR));
		}
		this->Write(szNewLine,_tcslen(szNewLine)*sizeof(TCHAR));
	}
	szBuffer[0]=0;
	this->Write(szBuffer,sizeof(TCHAR));

	return SendToClipboard();
}

int CClipboardMgr::GetClipboardFormat()
{
#ifdef _UNICODE
	return CF_UNICODETEXT ;
#else
	return CF_TEXT;
#endif
}

BOOL CClipboardMgr::DumpTree(HWND hWndTree, DWORD dwOption)
{
	BOOL bRet=TRUE;
	HTREEITEM hItem=NULL;
	if( (dwOption&CB_OPTION_ONLY_SELECTED)
		|| (dwOption&CB_OPTION_SUBTREE))
		hItem = TreeView_GetSelection(hWndTree);
	else
		hItem= TreeView_GetRoot(hWndTree);
	if(hItem==NULL)
		return FALSE;

	if(!WriteSubTree(hWndTree,hItem,0,dwOption))
	{
		Msg(_T("Failed write sub tree [%x]."),hWndTree);
		return FALSE;
	}

	return SendToClipboard();
}

BOOL CClipboardMgr::WriteSubTree(HWND hWndTree,HTREEITEM hItem, int nIncendent,DWORD dwOption)
{
	TVITEM tvi;
	TCHAR szBuffer[MAX_PATH]=_T("");
	HTREEITEM hChildItem,hSibling;
	const TCHAR szIncedent[]=_T("   ");

	memset(&tvi,0,sizeof(tvi));
	tvi.mask=TVIF_HANDLE|TVIF_TEXT;
	tvi.pszText = szBuffer;
	tvi.cchTextMax = MAX_PATH;
	tvi.hItem = hItem;
	if(!TreeView_GetItem(hWndTree,&tvi))
	{
		Msg(_T("TreeView_GetItem(%x,%x) failed"),hWndTree,hItem);
		return FALSE;
	}
	if(szBuffer[0]!=0)
	{
		for(int i=0;i<nIncendent;i++)
			Write(szIncedent,_tcslen(szIncedent)*sizeof(TCHAR));

		Write(szBuffer,_tcslen(szBuffer)*sizeof(TCHAR));

		this->Write(szNewLine,_tcslen(szNewLine)*sizeof(TCHAR));
	}
	if(dwOption&CB_OPTION_ONLY_SELECTED)
		return TRUE;
	hChildItem=TreeView_GetChild(hWndTree,hItem);
	if(hChildItem)
		WriteSubTree(hWndTree,hChildItem,nIncendent+1,dwOption);

	while( (hSibling=TreeView_GetNextSibling(hWndTree,hChildItem)) !=NULL )
	{
		WriteSubTree(hWndTree,hSibling,nIncendent+1,dwOption);
		hChildItem=hSibling;
	}
	return TRUE;
}
