/*-------------------------------------------------------------------------
// 
// PdbMaster.h: interface for the CPdbMaster class.
// This code referenced the code by Sven B. Schreiber in his article
//  on PDB published in the ITPro magazine.
// 
// Raymond Zhang 2007 April 19th 
// This code is designed in Beijing airport during a travel. :-)
// 
-------------------------------------------------------------------------*/

#if !defined(AFX_PDBMASTER_H__89BBBA99_ABB6_43FF_9717_FD4FD94A9C04__INCLUDED_)
#define AFX_PDBMASTER_H__89BBBA99_ABB6_43FF_9717_FD4FD94A9C04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PDB_SIGNATURE_200 \
        "Microsoft C/C++ program database 2.00\r\n\x1a"
#define PDB_SIGNATURE_700 \
        "Microsoft C/C++ MSF 7.00\r\n\x1a"

#define PDB_STREAM_FREE     -1
#define PDB_STREAM_MASK     0x0000FFFF
#define PDB_STREAM_MAX      (PDB_STREAM_MASK+1)

typedef struct _PDB_STREAM
{
	DWORD dwStreamSize;   
	PWORD pwStreamPages; 
}PDB_STREAM, *PPDB_STREAM, **PPPDB_STREAM;

//	char       szMagic[4]; // magic signature, like JG, RS, etc.

typedef struct _PDB_HEADER20
{
    DWORD      dwPageSize;     // 0x0400, 0x0800, 0x1000
    WORD       wStartPage;     // 0x0002
    WORD       wFilePages;     // file size / dPageSize
    PDB_STREAM RootStream;     // stream directory
}PDB_HEADER20, *PPDB_HEADER20, **PPPDB_HEADER20;

typedef struct _PDB_HEADER70
{
    DWORD      dwPageSize;     // 0x0400, 0x0800, 0x1000
    DWORD      dwStartPage;    // 0x0002
    DWORD      dwFilePages;    // file size / dPageSize
    PDB_STREAM RootStream;     // stream directory
}PDB_HEADER70, *PPDB_HEADER70, **PPPDB_HEADER70;

typedef struct _PDB_ROOT
{
    WORD       wCount;      
    WORD       wReserved;   // 0
}PDB_ROOT, *PPDB_ROOT, **PPPDB_ROOT;

class CPdbMaster  
{
public:
	HRESULT DumpStreams(HWND hWndList);
	HRESULT DumpStreamDirectory(HWND hWndList);
	HRESULT DumpSignature(HWND hWndList);
	HRESULT DumpHeader(HWND hWndListBox);
	BOOL IsPdb(char * szSignature);
	BOOL IsLoaded(){return m_hFileHandle!=INVALID_HANDLE_VALUE;}
	HRESULT Load(LPCTSTR lpszFullFileName);
	HRESULT Cleanup();
	CPdbMaster();
	virtual ~CPdbMaster();
protected:
	HRESULT ReadHeader();
	HANDLE m_hFileHandle;
	long   m_nSignatureLength;
	long   m_nRootStreamOffset;
	PPDB_HEADER70 m_pPdbHeader;
	long   m_nPdbVersion;
};

#endif // !defined(AFX_PDBMASTER_H__89BBBA99_ABB6_43FF_9717_FD4FD94A9C04__INCLUDED_)
