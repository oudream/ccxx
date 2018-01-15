// ClipboardMgr.h: interface for the CClipboardMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIPBOARDMGR_H__747457B6_D506_4C61_9266_DABD1573AFC9__INCLUDED_)
#define AFX_CLIPBOARDMGR_H__747457B6_D506_4C61_9266_DABD1573AFC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SmtObject.h"
#include <afxadv.h>
const TCHAR szNewLine[]=_T("\r\n");

#define CB_OPTION_ONLY_SELECTED 1
#define CB_OPTION_SUBTREE 2
#define CB_OPTION_EXPORT_TOFILE 4
class CClipboardMgr : public CSmtObject  
{
public:
	BOOL WriteSubTree(HWND hWndTree,HTREEITEM hItem, int nIncendent,DWORD dwOption);
	BOOL DumpTree(HWND hWndTree,DWORD dwOption);
	int GetClipboardFormat();
	BOOL DumpList(HWND hWndList,DWORD dwOption);
	BOOL SendToClipboard();
	BOOL Write(const void * lpData, int nBytes);
	CClipboardMgr();
	virtual ~CClipboardMgr();
protected:
	CSharedFile m_ShareFile;
};

#endif // !defined(AFX_CLIPBOARDMGR_H__747457B6_D506_4C61_9266_DABD1573AFC9__INCLUDED_)
