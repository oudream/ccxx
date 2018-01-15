// MdmpParser.h: interface for the CMdmpParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDMPPARSER_H__4BD79939_F449_4FD6_821A_EABD677EFF54__INCLUDED_)
#define AFX_MDMPPARSER_H__4BD79939_F449_4FD6_821A_EABD677EFF54__INCLUDED_
#include "dbghelp.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef struct _STREAM_TYPE
{
	LPCTSTR lpszStreamType;
	DWORD   dwStreamType;
}STREAM_TYPE,*PSTREAM_TYPE;
class CMdmpParser  
{
public:
	HRESULT RawRead(ULONG ulRVA, ULONG ulSize);
	void ShowLocation(LPCTSTR szObject, MINIDUMP_LOCATION_DESCRIPTOR* pMemLocation);
	HRESULT ShowThreadExList(PMINIDUMP_THREAD_EX_LIST pThreadExListStream,ULONG ulStreamSize);
	HRESULT ShowSysInfo(PMINIDUMP_SYSTEM_INFO pSysInfoStream,ULONG ulStreamSize);
	HRESULT ShowException(PMINIDUMP_EXCEPTION_STREAM pExceptionStream,ULONG ulStreamSize);
	HRESULT ShowMemoryList(PMINIDUMP_MEMORY_LIST pMemoryStream,ULONG ulStreamSize);
	HRESULT ShowModuleList(PMINIDUMP_MODULE_LIST pModuleListStream,ULONG ulStreamSize);
	HRESULT ShowThreadList(PMINIDUMP_THREAD_LIST pThreadListStream,ULONG ulStreamSize);
	LPCTSTR GetStreamType(ULONG nStreamType);
	HRESULT ReadStream(ULONG nStreamType);
	HRESULT ReadStreams(HWND hLBStreamTypes);
	void Close();
	void SetListener(HWND hWndListBox);
	HRESULT Open(LPCTSTR lpszFileName,HWND hWndStreamList);
	CMdmpParser();
	virtual ~CMdmpParser();
	void OutMsg(LPCTSTR szMsgFormat,...);
	BOOL IsOpen(){return m_hDumpFile!=INVALID_HANDLE_VALUE;}
protected:
	void FillStreamList(HWND hListStream);
	HWND m_hWndListener;
	HANDLE m_hDumpFile;
	HANDLE m_hDumpMapFile;
	PVOID  m_pBaseofDump;
	ULONG64 m_ulFileSize;
};

#endif // !defined(AFX_MDMPPARSER_H__4BD79939_F449_4FD6_821A_EABD677EFF54__INCLUDED_)
