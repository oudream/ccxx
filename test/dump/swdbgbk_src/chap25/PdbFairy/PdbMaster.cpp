// PdbMaster.cpp: implementation of the CPdbMaster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PdbFairy.h"
#include "PdbMaster.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPdbMaster::CPdbMaster()
{
	m_hFileHandle=INVALID_HANDLE_VALUE;
	m_nSignatureLength=0;
	m_pPdbHeader=NULL;
	m_nRootStreamOffset=0;
}

CPdbMaster::~CPdbMaster()
{
	Cleanup();
}

// -----------------------------------------------------------------

/*PPDB_HEADER PdbLoad (PWORD pwPath,
                     PBOOL pfInvalid)
{
    HANDLE      hf;
    DWORD       dData, dRead;
    PPDB_HEADER pph = NULL;

    *pfInvalid = FALSE;

    if ((hf = CreateFile (pwPath, GENERIC_READ,
                          FILE_SHARE_READ, NULL, OPEN_EXISTING,
                          FILE_FLAG_SEQUENTIAL_SCAN, NULL))
        != INVALID_HANDLE_VALUE)
        {
        dData = GetFileSize (hf, NULL);

        if ((dData != INVALID_FILE_SIZE) && dData &&
            ((pph = malloc (dData)) != NULL))
            {
            if ((!ReadFile (hf, pph, dData, &dRead, NULL)) ||
                (dData != dRead)                           ||
                (*pfInvalid = !PdbValid (pph, dData)))
                {
                free (pph);
                pph = NULL;
                }
            }
        CloseHandle (hf);
        }
    return pph;
    }

// -----------------------------------------------------------------

BOOL PdbSave (PWORD pwTarget,
              PVOID pData,
              DWORD dData)
    {
    HANDLE hf;
    DWORD  dWritten;
    BOOL   fOk = FALSE;

    printf (L"\r\nSaving \"%s\"... ", pwTarget);

    if ((hf = CreateFile (pwTarget, GENERIC_READ | GENERIC_WRITE,
                          FILE_SHARE_READ, NULL, CREATE_ALWAYS,
                          FILE_FLAG_SEQUENTIAL_SCAN |
                          FILE_ATTRIBUTE_NORMAL, NULL))
        != INVALID_HANDLE_VALUE)
        {
        fOk = WriteFile (hf, pData, dData, &dWritten, NULL) &&
              (dData == dWritten);

        CloseHandle (hf);
        }
    printf (fOk ? L"%lu bytes\r\n" : L"ERROR\r\n", dData);
    return fOk;
    }

// -----------------------------------------------------------------

DWORD PdbLimit (PPDB_HEADER pph)
    {
    return ((DWORD) pph->wStartPage - 1) * pph->dPageSize * 8
                                         * pph->dPageSize;
    }

// -----------------------------------------------------------------

DWORD PdbPages (PPDB_HEADER pph,
                DWORD       dBytes)
    {
    return (dBytes ? (((dBytes-1) / pph->dPageSize) + 1) : 0);
    }

// -----------------------------------------------------------------

PVOID PdbRead (PPDB_HEADER pph,
               DWORD       dBytes,
               PWORD       pwPages)
    {
    DWORD i, j;
    DWORD dPages = PdbPages (pph, dBytes);
    PVOID pPages = malloc (dPages * pph->dPageSize);

    if (pPages != NULL)
        {
        for (i = 0; i < dPages; i++)
            {
            j = pwPages [i];

            CopyMemory ((PBYTE) pPages + (i * pph->dPageSize),
                        (PBYTE) pph    + (j * pph->dPageSize),
                        pph->dPageSize);
            }
        }
    return pPages;
    }

// -----------------------------------------------------------------

PPDB_ROOT PdbRoot (PPDB_HEADER pph,
                   PDWORD      pdBytes)
    {
    DWORD     dBytes, i, n;
    PPDB_ROOT ppr = NULL;
    
    if ((ppr = PdbRead (pph, PDB_ROOT_, pph->awRootPages)) != NULL)
        {
        dBytes = PDB_ROOT__ ((DWORD) ppr->wCount);
        free (ppr);

        if ((ppr = PdbRead (pph, dBytes, pph->awRootPages)) != NULL)
            {
            for (n = i = 0; i < (DWORD) ppr->wCount; i++)
                {
                n += PdbPages (pph, ppr->aStreams [i].dStreamSize);
                }
            dBytes += n * sizeof (WORD);
            free (ppr);

            ppr = PdbRead (pph, dBytes, pph->awRootPages);
            }
        }
    *pdBytes = (ppr != NULL ? dBytes : 0);
    return ppr;
    }

// -----------------------------------------------------------------

PVOID PdbStream (PPDB_HEADER pph,
                 PPDB_ROOT   ppr,
                 DWORD       dStream,
                 PDWORD      pdBytes)
    {
    DWORD dBytes, i;
    PWORD pwPages;
    PVOID pPages = NULL;

    if (dStream < (DWORD) ppr->wCount)
        {
        pwPages = (PWORD) ((PBYTE) ppr +
                           PDB_ROOT__ ((DWORD) ppr->wCount));

        for (i = 0; i < dStream; i++)
            {
            pwPages += PdbPages (pph,
                                 ppr->aStreams [i].dStreamSize);
            }
        dBytes = ppr->aStreams [dStream].dStreamSize;
        pPages = PdbRead (pph, dBytes, pwPages);
        }
    *pdBytes = (pPages != NULL ? dBytes : 0);
    return pPages;
    }

// -----------------------------------------------------------------

VOID PdbInfo (PWORD       pwPath,
              PPDB_HEADER pph,
              PPDB_ROOT   ppr)
    {
    DWORD dFileLimit, dFileBytes, dFilePages, dStreams;
    DWORD dDataBytes, dDataPages, dRootBytes, dRootPages, i, n;
    PWORD pwFile;
    WORD  awPath [MAX_PATH];

    dFileLimit = PdbLimit (pph);
    dFilePages = pph->wFilePages;
    dFileBytes = dFilePages * pph->dPageSize;
    dStreams   = ppr->wCount;

    dDataBytes = 0;
    dDataPages = 0;

    for (i = 0; i < dStreams; i++)
        {
        dDataBytes +=                ppr->aStreams [i].dStreamSize ;
        dDataPages += PdbPages (pph, ppr->aStreams [i].dStreamSize);
        }
    dRootBytes = PDB_ROOT__(dStreams) + (dDataPages * sizeof(WORD));
    dRootPages = PdbPages (pph, dRootBytes);

    n = GetFullPathName (pwPath, MAX_PATH, awPath, &pwFile);

    if ((!n) || (n >= MAX_PATH))
        {
        lstrcpyn (awPath, pwPath, MAX_PATH);
        }
    printf (L"\r\n"
            L"Properties of \"%s\":\r\n"
            L"\r\n"
            L"%13lu byte%s maximum size\r\n"
            L"%13lu byte%s allocated\r\n"
            L"%13lu byte%s used by %lu data stream%s\r\n"
            L"%13lu byte%s used by the root stream\r\n"
            L"%13lu byte%s per page\r\n"
            L"%13lu page%s allocated\r\n"
            L"%13lu page%s used by %lu data stream%s\r\n"
            L"%13lu page%s used by the root stream\r\n",
            awPath,
            COUNT (dFileLimit,     L" ", L"s"),
            COUNT (dFileBytes,     L" ", L"s"),
            COUNT (dDataBytes,     L" ", L"s"),
            COUNT (dStreams,       L"",  L"s"),
            COUNT (dRootBytes,     L" ", L"s"),
            COUNT (pph->dPageSize, L" ", L"s"),
            COUNT (dFilePages,     L" ", L"s"),
            COUNT (dDataPages,     L" ", L"s"),
            COUNT (dStreams,       L"",  L"s"),
            COUNT (dRootPages,     L" ", L"s"));
    return;
    }

// -----------------------------------------------------------------

PWORD PdbTarget (PWORD pwPath,
                 PWORD pwTarget,
                 DWORD dOption)
    {
    DWORD i, n;
    BOOL  fSeparator;
    WORD  awExtension [] = L".9876543210";
    PWORD pwExtension    = NULL;
    PWORD pwBuffer       = NULL;

    switch (dOption & DISPLAY_OPTION_FLAGS)
        {
        case DISPLAY_OPTION_HEADER:
            {
            pwExtension = L".header";
            break;
            }
        case DISPLAY_OPTION_ALLOC:
            {
            pwExtension = L".alloc";
            break;
            }
        case DISPLAY_OPTION_ROOT:
            {
            pwExtension = L".root";
            break;
            }
        case DISPLAY_OPTION_DATA:
            {
            wsprintf (awExtension, L".%03lu",
                      dOption & DISPLAY_OPTION_STREAM);

            pwExtension = awExtension;
            break;
            }
        }
    if (pwExtension != NULL)
        {
        i = lstrlen (pwPath);

        while (i && (pwPath [i-1] != '\\')
                 && (pwPath [i-1] != ':' )) i--;

        n = lstrlen (pwTarget);

        if (fSeparator = (n && (pwTarget [n-1] != '\\')
                            && (pwTarget [n-1] != ':' ))) n++;

        n += (lstrlen (pwPath+i) + lstrlen (pwExtension));

        if ((pwBuffer = malloc ((n+1) * sizeof (WORD))) != NULL)
            {
            lstrcpy (pwBuffer, pwTarget);

            if (fSeparator) lstrcat (pwBuffer, L"\\");

            lstrcat (pwBuffer, pwPath+i);
            lstrcat (pwBuffer, pwExtension);
            }
        }
    return pwBuffer;
    }

// -----------------------------------------------------------------

VOID PdbOutput (PWORD       pwPath,
                PWORD       pwTarget,
                DWORD       dOptions,
                PPDB_HEADER pph,
                PPDB_ROOT   ppr,
                DWORD       dRoot)
    {
    PWORD pwTarget1;
    PVOID pData;
    DWORD dData, dStream;

    if (dOptions & DISPLAY_OPTION_HEADER)
        {
        if ((pwTarget1 = PdbTarget (pwPath, pwTarget,
                                    DISPLAY_OPTION_HEADER))
            != NULL)
            {
            PdbSave (pwTarget1, pph, pph->dPageSize);
            free    (pwTarget1);
            }
        else
            {
            printf (L"\r\nUnable to save the header\r\n");
            }
        }
    if (dOptions & DISPLAY_OPTION_ALLOC)
        {
        if ((pwTarget1 = PdbTarget (pwPath, pwTarget,
                                    DISPLAY_OPTION_ALLOC))
            != NULL)
            {
            pData =  (PBYTE) pph + pph->dPageSize;
            dData = ((DWORD) pph->wStartPage - 1) * pph->dPageSize;

            PdbSave (pwTarget1, pData, dData);
            free    (pwTarget1);
            }
        else
            {
            printf (L"\r\nUnable to save the allocation bits\r\n");
            }
        }
    if (dOptions & DISPLAY_OPTION_ROOT)
        {
        if ((pwTarget1 = PdbTarget (pwPath, pwTarget,
                                    DISPLAY_OPTION_ROOT))
            != NULL)
            {
            PdbSave (pwTarget1, ppr, dRoot);
            free    (pwTarget1);
            }
        else
            {
            printf (L"\r\nUnable to save the root stream\r\n");
            }
        }
    if (dOptions & DISPLAY_OPTION_DATA)
        {
        for (dStream = 0; dStream < ppr->wCount; dStream++)
            {
            pwTarget1 = PdbTarget (pwPath, pwTarget,
                                   DISPLAY_OPTION_DATA | dStream);

            pData = PdbStream (pph, ppr, dStream, &dData);

            if ((pwTarget1 != NULL) && (pData != NULL))
                {
                PdbSave (pwTarget1, pData, dData);
                }
            else
                {
                printf (L"\r\nUnable to save data stream #%lu\r\n",
                        dStream);
                }
            free (pwTarget1);
            free (pData);
            }
        }
    return;
    }

// -----------------------------------------------------------------

VOID PdbExplode (PWORD pwPath,
                 PWORD pwTarget,
                 DWORD dOptions)
    {
    PPDB_HEADER pph;
    PPDB_ROOT   ppr;
    DWORD       dRoot;
    BOOL        fInvalid;

    if ((pph = PdbLoad (pwPath, &fInvalid)) != NULL)
        {
        if ((ppr = PdbRoot (pph, &dRoot)) != NULL)
            {
            PdbInfo   (pwPath, pph, ppr);
            PdbOutput (pwPath, pwTarget, dOptions, pph, ppr, dRoot);

            free (ppr);
            }
        free (pph);
        }
    else
        {
        printf ((fInvalid
                 ? L"\r\n\"%s\" is not a PDB file\r\n"
                 : L"\r\nUnable to load \"%s\"\r\n"),
                pwPath);
        }
    return;
    }
*/
HRESULT CPdbMaster::Cleanup()
{
	if(m_hFileHandle!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileHandle);
		m_hFileHandle=INVALID_HANDLE_VALUE;
		if(m_pPdbHeader)
		{
			delete m_pPdbHeader;
			m_pPdbHeader=NULL;
		}
	}
	return S_OK;
}

HRESULT CPdbMaster::ReadHeader()
{
	DWORD dwRead=0;

	if(!IsLoaded() 
		|| this->m_nSignatureLength<=0)
		return E_FAIL;

	if(m_pPdbHeader==NULL)
		m_pPdbHeader=new PDB_HEADER70;

	SetFilePointer(m_hFileHandle,m_nSignatureLength,
		0,FILE_BEGIN);

	if(this->m_nPdbVersion==200)
	{
		PDB_HEADER20 ph;
		if ((!ReadFile (m_hFileHandle, &ph, 
			sizeof(PDB_HEADER20), &dwRead, NULL)) ||
			(sizeof(PDB_HEADER20) != dwRead) )
			return E_FAIL;
		m_pPdbHeader->dwFilePages=ph.wFilePages;
		m_pPdbHeader->dwPageSize=ph.dwPageSize;
		m_pPdbHeader->dwStartPage=ph.wStartPage;
		m_pPdbHeader->RootStream=ph.RootStream;
	}
	else
	{
		if ((!ReadFile (m_hFileHandle, m_pPdbHeader, 
			sizeof(PDB_HEADER70), &dwRead, NULL)) ||
			(sizeof(PDB_HEADER70) != dwRead) )
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPdbMaster::Load(LPCTSTR lpszFullFileName)
{
	if(IsLoaded())
		return E_FAIL;

	this->m_hFileHandle = CreateFile (lpszFullFileName,
		GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING,
		 FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if(m_hFileHandle == INVALID_HANDLE_VALUE)
		return E_FAIL;

	return S_OK;
}

BOOL CPdbMaster::IsPdb(char *szSignature)
{
    return ( !strcmp (szSignature, PDB_SIGNATURE_200)
		|| !strcmp (szSignature, PDB_SIGNATURE_700)
           );
}

HRESULT CPdbMaster::DumpHeader(HWND hWndListBox)
{
	TCHAR szMsg[MAX_PATH];

	if(this->m_nSignatureLength<=0)
		DumpSignature(hWndListBox);

	if(m_pPdbHeader==NULL &&
		ReadHeader()!=S_OK)
		return E_FAIL;

	_stprintf(szMsg, _T("PageSize: 0x%x [%d]"),
		m_pPdbHeader->dwPageSize,m_pPdbHeader->dwPageSize);
	SendMessage(hWndListBox, LB_ADDSTRING,0,(LPARAM)szMsg);

	_stprintf(szMsg, _T("File Pages: 0x%x [%d]"),
		m_pPdbHeader->dwFilePages,m_pPdbHeader->dwFilePages);
	SendMessage(hWndListBox, LB_ADDSTRING,0,(LPARAM)szMsg);

	_stprintf(szMsg, _T("Start Page: 0x%x [%d]"),
		m_pPdbHeader->dwStartPage,m_pPdbHeader->dwStartPage);
	SendMessage(hWndListBox, LB_ADDSTRING,0,(LPARAM)szMsg);

	_stprintf(szMsg, _T("Root Stream Size: 0x%x [%d]"),
		m_pPdbHeader->RootStream.dwStreamSize,m_pPdbHeader->RootStream.dwStreamSize);
	SendMessage(hWndListBox, LB_ADDSTRING,0,(LPARAM)szMsg);

	_stprintf(szMsg, _T("Root Stream Pages Pointer: 0x%x [%d]"),
		m_pPdbHeader->RootStream.pwStreamPages,m_pPdbHeader->RootStream.pwStreamPages);
	SendMessage(hWndListBox, LB_ADDSTRING,0,(LPARAM)szMsg);

	return S_OK;
}

HRESULT CPdbMaster::DumpSignature(HWND hWndList)
{
	DWORD dwRead=0;
	char szSignature[MAX_PATH],*lpsz;
	TCHAR szMsg[MAX_PATH];

	SetFilePointer(m_hFileHandle,0,0,FILE_BEGIN);

	if ((!ReadFile (m_hFileHandle, szSignature, 
		sizeof(szSignature), &dwRead, NULL)) )
		return E_FAIL;

	szSignature[MAX_PATH-1]=0;

#ifdef _UNICODE
	_stprintf(szMsg, _T("Signature: %S"),szSignature);
#else
	_stprintf(szMsg, _T("Signature: %s"),szSignature);
#endif // _UNICODE
	SendMessage(hWndList, LB_ADDSTRING,0,(LPARAM)szMsg);

	lpsz=strchr(szSignature,'\x1a');
	if(lpsz==NULL)
		return E_FAIL;

	*(++lpsz)=0;
	if(stricmp(szSignature, PDB_SIGNATURE_200)==0)
		m_nPdbVersion=200;
	else if(stricmp(szSignature, PDB_SIGNATURE_700)==0)
		m_nPdbVersion=700;
	else
		return E_FAIL;

	m_nSignatureLength=lpsz-szSignature;
	
	// skip the magic signature
	m_nSignatureLength+=4;

	// align by dword
	if(m_nSignatureLength%4)
		m_nSignatureLength+=4-m_nSignatureLength%4;

	return 	S_OK;
}

HRESULT CPdbMaster::DumpStreamDirectory(HWND hWndList)
{
	int nPages=0,i;
	WORD * pWord=NULL;
	TCHAR szMsg[MAX_PATH];
	DWORD dwRead=0,dwOffset,*pDWord,dwPageNo4RSPA;
	// page no to store the page no array of root page stream 
	
	if(this->m_pPdbHeader==NULL
		&&	S_OK!=DumpHeader(hWndList) )
		return E_FAIL;

	nPages=m_pPdbHeader->RootStream.dwStreamSize
		/m_pPdbHeader->dwPageSize;
	if(m_pPdbHeader->RootStream.dwStreamSize
		%m_pPdbHeader->dwPageSize)
		nPages++;

	dwOffset=m_nSignatureLength;
	dwOffset+=this->m_nPdbVersion==200?sizeof(PDB_HEADER20):
		sizeof(PDB_HEADER70);

	SetFilePointer(m_hFileHandle,
		dwOffset,
		0,FILE_BEGIN);

	if(this->m_nPdbVersion==200)
	{
		pWord=new WORD[nPages];
		if(!ReadFile(m_hFileHandle,pWord,nPages*sizeof(WORD),&dwRead,NULL)
			&& dwRead!=nPages*sizeof(WORD))
			return E_FAIL;

		for(i=0;i<nPages;i++)
		{
			dwOffset=*(pWord+i);

			if(i==0)
				m_nRootStreamOffset=dwOffset*m_pPdbHeader->dwPageSize;

			_stprintf(szMsg, _T("Root Page [%d]: no = 0x%x, byte offset = 0x%x"),
				i, dwOffset, dwOffset*m_pPdbHeader->dwPageSize);
			SendMessage(hWndList, LB_ADDSTRING,0,(LPARAM)szMsg);
		}

		delete pWord;
	}
	else
	{
		// read a dword
		if(!ReadFile(m_hFileHandle,&dwPageNo4RSPA,sizeof(DWORD),&dwRead,NULL)
			&& dwRead!=sizeof(DWORD))
			return E_FAIL;

		dwOffset=dwPageNo4RSPA*m_pPdbHeader->dwPageSize;
		SetFilePointer(m_hFileHandle,
			dwOffset,
			0,FILE_BEGIN);

		pDWord=new DWORD[nPages];
		if(!ReadFile(m_hFileHandle,pDWord,nPages*sizeof(DWORD),&dwRead,NULL)
			&& dwRead!=nPages*sizeof(DWORD))
			return E_FAIL;

		for(i=0;i<nPages;i++)
		{
			dwOffset=*(pDWord+i);

			if(i==0)
				m_nRootStreamOffset=dwOffset*m_pPdbHeader->dwPageSize;

			_stprintf(szMsg, _T("Root Page [%d]: no = 0x%x, byte offset = 0x%x"),
				i, dwOffset, dwOffset*m_pPdbHeader->dwPageSize);
			SendMessage(hWndList, LB_ADDSTRING,0,(LPARAM)szMsg);
		}

		delete pDWord;
	}
	
	return S_OK;
}

HRESULT CPdbMaster::DumpStreams(HWND hWndList)
{
	PDB_ROOT root;
	DWORD dwRead=0,i;
	TCHAR szMsg[MAX_PATH];
	PDB_STREAM stream;

	if(m_nRootStreamOffset<=0)
		DumpStreamDirectory(hWndList);

	SetFilePointer(m_hFileHandle,
		m_nRootStreamOffset,
		0,FILE_BEGIN);

	if(!ReadFile(m_hFileHandle,&root,sizeof(PDB_ROOT),&dwRead,NULL)
		&& dwRead!=sizeof(PDB_ROOT))
		return E_FAIL;

	_stprintf(szMsg, _T("Total streams: [%d]"),
		root.wCount);

	SendMessage(hWndList, LB_ADDSTRING,0,(LPARAM)szMsg);

	for(i=0;i<root.wCount;i++)
	{
		if(!ReadFile(m_hFileHandle,&stream,sizeof(PDB_STREAM),&dwRead,NULL)
			&& dwRead!=sizeof(PDB_STREAM))
			break;

		_stprintf(szMsg, _T("Stream[%d], size=%d, pages=0x%x"),
			i,stream.dwStreamSize,stream.pwStreamPages);

		SendMessage(hWndList, LB_ADDSTRING,0,(LPARAM)szMsg);
	}

	return S_OK;
}
