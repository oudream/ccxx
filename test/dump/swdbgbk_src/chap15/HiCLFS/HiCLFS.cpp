/*---------------------------------------------------------------------
HiCLFS.cpp - Sample to demonstrate CLFS API.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <Clfsw32.h>
#include <Clfsmgmtw32.h>

void ShowLogInformation(HANDLE hClfsLog,LPCTSTR szTitle)
{
    CLFS_INFORMATION li;
    ULONG ulSize=sizeof(li);
    if(!GetLogFileInformation(hClfsLog,
        &li,&ulSize))
    {
        _tprintf(_T("GetLogFileInformation failed for 0x%x\n"),GetLastError());
        return;
    }
    _tprintf(_T("--------------%s--------------\n"),szTitle); 
    _tprintf(_T("TotalAvailable=0x%I64x,CurrentAvailable=0x%I64x, TotalReservation=0x%I64x\n"),
      li.TotalAvailable,li.CurrentAvailable,li.TotalReservation);
    _tprintf(_T("BaseFileSize=0x%I64x, ContainerSize=0x%I64x, TotalCntnrs=%d, FreeCntnrs=%d\n"),
      li.BaseFileSize,li.ContainerSize,li.TotalContainers,li.FreeContainers);
    _tprintf(_T("TotalClients=%d, Attributes=0x%x, FlushThreshold=%d, SectorSize=%d\n"),
      li.TotalClients,li.Attributes,li.FlushThreshold,li.SectorSize);
    _tprintf(_T("Lsn: MinArchiveTail=0x%I64x, Base=0x%I64x, LastFlushed=0x%I64x, Last=0x%I64x, Restart=0x%I64x\n\n"),
      li.MinArchiveTailLsn,li.BaseLsn,li.LastFlushedLsn,li.LastLsn,li.RestartLsn);
     // GUID Identity;
}
HRESULT   ReadClfsLog(LPCTSTR szLogFile)
{
    PVOID pMarshalContext=NULL; 
    HRESULT hRet=S_OK;
    CLFS_INFORMATION li;
    ULONG ulSize=0;
    CLFS_RECORD_TYPE ulRecordType=0;
    PVOID pReadBuffer,pReadContext;
    CLFS_LSN lsnUndoNext,lsnPrevious,lsnRecord;

    HANDLE hClfsLog=CreateLogFile(szLogFile,
        GENERIC_READ, 0,NULL,OPEN_ALWAYS,0);
    if(hClfsLog==INVALID_HANDLE_VALUE)
    {
        _tprintf(_T("CreateLogFile(%s) failed with 0x%d\n"),
            szLogFile, GetLastError());
        return E_FAIL;
    }
    if(!CreateLogMarshallingArea(hClfsLog,NULL,NULL,NULL,
        512, 2,2,&pMarshalContext))
    {
        _tprintf(_T("CreateLogMarshallingArea failed for 0x%x\n"),GetLastError());
        hRet=-3;
        goto TAG_CLEAN;
    }
    ulSize=sizeof(li);
    if(!GetLogFileInformation(hClfsLog,
        &li,&ulSize))
    {
        _tprintf(_T("GetLogFileInformation failed for 0x%x\n"),GetLastError());
        hRet=-4;
        goto TAG_CLEAN;
    }

    if(!ReadLogRecord(pMarshalContext,
        &(li.BaseLsn),
        ClfsContextForward,
        &pReadBuffer,
        &ulSize,&ulRecordType,
        &lsnUndoNext,&lsnPrevious,
        &pReadContext,NULL))
    {
        _tprintf(_T("ReadLogRecord failed for 0x%x\n"),GetLastError());
        hRet=-5;
        goto TAG_CLEAN;
    }
    _tprintf(_T("Got Record: size=%d, type=%d, lsnUndoNext=0x%I64x,lsnPrev=0x%I64x\n"),
        ulSize,ulRecordType,lsnUndoNext,lsnPrevious);
    _tprintf(_T("Record string: %s\n"),pReadBuffer);

    while(ReadNextLogRecord(pReadContext,
        &pReadBuffer,
        &ulSize,&ulRecordType,
        NULL,
        &lsnUndoNext,&lsnPrevious,&lsnRecord,NULL))
    {
        _tprintf(_T("Got Record: size=%d, type=%d \n"),
            ulSize,ulRecordType);
        _tprintf(_T("lsnUndoNext=0x%I64x,lsnPrev=0x%I64x, lsnRecord=0x%I64x\n"),
             lsnUndoNext,lsnPrevious,lsnRecord);
        _tprintf(_T("Record string: %s\n"),pReadBuffer);
    }
    if(!TerminateReadLog(pReadContext))
        _tprintf(_T("TerminateReadLog failed for 0x%x\n"),GetLastError());

TAG_CLEAN:
    if(hClfsLog!=INVALID_HANDLE_VALUE)
        CloseHandle(hClfsLog);
    if(pMarshalContext!=NULL)
        DeleteLogMarshallingArea(pMarshalContext);

    return hRet;
}

int _tmain(int argc, _TCHAR* argv[])
{
    HANDLE hClfsLog=INVALID_HANDLE_VALUE;
    TCHAR szLogFile[MAX_PATH];
    TCHAR szModuleName[MAX_PATH]={0};
    PVOID pMarshalContext=NULL; 
    int nRet=0;
    CLFS_WRITE_ENTRY ClfsEntry;
    CLFS_LSN  ClfsLSN;
    ULONGLONG cbContainer=512*1024*2;
    ULONGLONG ullContainers=2;
    ULONGLONG ullResultingSize=0;
    LPTSTR lpszContainerPath;
    LPTSTR lpsz;

    GetModuleFileName(NULL,szModuleName,MAX_PATH);
    lpsz=_tcsrchr(szModuleName,'.');
    if(lpsz)
        *lpsz=0;

    _sntprintf(szLogFile, MAX_PATH, _T("LOG:%s"),szModuleName);
    _tprintf(_T("LogFileName: %s\n"),szLogFile);
    hClfsLog=CreateLogFile(szLogFile,
        GENERIC_WRITE| GENERIC_READ, 
        0,NULL,OPEN_ALWAYS,0);
    if(hClfsLog==INVALID_HANDLE_VALUE)
    {
        _tprintf(_T("CreateLogFile failed with 0x%d\n"),GetLastError());
        return -1;
    }
    ShowLogInformation(hClfsLog,_T("After Open/Create"));
   
    lpszContainerPath=_T("%BLF%\\CLFSCON01");
    if(!AddLogContainer(hClfsLog,&cbContainer,lpszContainerPath,NULL))
    {
        _tprintf(_T("AddLogContainer(%s) failed with 0x%d\n"),
            lpszContainerPath, GetLastError());
        nRet=-2;
    }
    else
        ShowLogInformation(hClfsLog,_T("After Add Container 1"));

    lpszContainerPath=_T("%BLF%\\CLFSCON02");
    if(!AddLogContainer(hClfsLog,&cbContainer,lpszContainerPath,NULL))
    {
        _tprintf(_T("AddLogContainer(%s) failed with 0x%d\n"),
            lpszContainerPath,GetLastError());
        nRet=-2;
    }
    else
        ShowLogInformation(hClfsLog,_T("After Add Container 2"));
    
    // the other way to add container is to use the mgr API.
    /*if(!SetLogFileSizeWithPolicy(hClfsLog, 
        &ullContainers, &ullResultingSize))
    {
        _tprintf(_T("SetLogFileSizeWithPolicy(0x%I64x) failed with 0x%d\n"),
            cbContainer,GetLastError());
        nRet=-2;
    }*/

    if(!CreateLogMarshallingArea(hClfsLog,NULL,NULL,NULL,
        512, 2,2,&pMarshalContext))
    {
        _tprintf(_T("CreateLogMarshallingArea failed for 0x%x\n"),GetLastError());
        nRet=-3;
        goto TAG_CLEAN;
    }
    else
        ShowLogInformation(hClfsLog,_T("After Create Marshalling Area"));

    _sntprintf(szModuleName, MAX_PATH, _T("A testing log record at tick 0x%x"),
        GetTickCount());
    ClfsEntry.Buffer=szModuleName;
    ClfsLSN=CLFS_LSN_INVALID;
    ClfsEntry.ByteLength=(_tcslen(szModuleName)+1)*sizeof(TCHAR);
    if(!ReserveAndAppendLog(pMarshalContext, //  PVOID pvMarshal,
        &ClfsEntry, //  PCLFS_WRITE_ENTRY rgWriteEntries,
        1, //  ULONG cWriteEntries,
        &ClfsLSN, //  PCLFS_LSN plsnUndoNext,
        &ClfsLSN, // PCLFS_LSN plsnPrevious,
        0, // ULONG cReserveRecords,
        0, // LONGLONG rgcbReservation[],
        CLFS_FLAG_NO_FLAGS, // ULONG fFlags,
        &ClfsLSN, // PCLFS_LSN plsn,
        NULL //LPOVERLAPPED pOverlapped
        ))
    {
        _tprintf(_T("ReserveAndAppendLog failed for 0x%x\n"),GetLastError());
        nRet=-4;
        goto TAG_CLEAN;
    }
    else
        ShowLogInformation(hClfsLog,_T("After Append a Record"));

    if(!FlushLogBuffers(pMarshalContext,NULL))
    {
        _tprintf(_T("FlushLogBuffers failed for 0x%x\n"),GetLastError());
        nRet=-5;
        goto TAG_CLEAN;
    }
    else
        ShowLogInformation(hClfsLog,_T("After Flush Buffers"));

TAG_CLEAN:
    if(hClfsLog!=INVALID_HANDLE_VALUE)
        CloseHandle(hClfsLog);
    if(pMarshalContext!=NULL)
        DeleteLogMarshallingArea(pMarshalContext);

    if(argc>1)
        ReadClfsLog(szLogFile);

    return nRet;
}

