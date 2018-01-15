// DataTracer.cpp: implementation of the CDataTracer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataTracer.h"
#include <tchar.h>
#include <stdio.h>
#define WINVER 0x0501
#include <windows.h>
#include "calltracer.h"

D4D_API CDataTracer * g_pDataTracer=NULL;

LONG WINAPI DataTracerVectoredHandler(
    struct _EXCEPTION_POINTERS *ExceptionInfo
    )
{
	TCHAR szMsg[MAX_PATH];
	if(ExceptionInfo->ExceptionRecord->ExceptionCode==0x40010006L)
		return EXCEPTION_CONTINUE_SEARCH;

	_stprintf(szMsg, _T("DataTracerVectoredHandler got exception 0x%x at 0x%x in PID %d"),
		ExceptionInfo->ExceptionRecord->ExceptionCode,
		ExceptionInfo->ExceptionRecord->ExceptionAddress,
		GetProcessId(GetCurrentProcess()) );
	OutputDebugString(szMsg);

	if(ExceptionInfo->ExceptionRecord->ExceptionCode
		==STATUS_SINGLE_STEP && g_pDataTracer!=NULL) // 0x80000004L
	{
		g_pDataTracer->HandleEvent(ExceptionInfo);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
    return EXCEPTION_CONTINUE_SEARCH;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataTracer::CDataTracer()
{
	int i;
	for(i=0;i<DBG_REG_COUNT;i++)
	{
		m_VarAddress[i]=0;
	}
	m_pVehHandler=NULL;
	m_hListBox=NULL;
}

CDataTracer::~CDataTracer()
{
	if(m_pVehHandler!=NULL)
		UnRegVeh();
	ClearAllDR();
}
// nLen=00 1 byte
//      01 2 bytes
//      11 4 bytes
// ReadWrite 00 execution, 01 data write only, 10 IO, 11 read and write
// 
HRESULT CDataTracer::AddVar(ULONG ulVarAddress,int nLen, int nReadWrite)
{
	int i;
	HRESULT hr=E_FAIL;

	if(IsVarExisted(ulVarAddress)>=0)
		return S_OK;

	for(i=0;i<DBG_REG_COUNT;i++)
	{
		if(m_VarAddress[i]==0)
		{
			m_VarAddress[i]=ulVarAddress;
			m_VarLength[i]=nLen;
			m_VarReadWrite[i]=nReadWrite;
			hr=S_OK;
		}
	}
	return E_FAIL;
}

HRESULT CDataTracer::RemoveVar(ULONG ulAddress)
{
	int iIndex=IsVarExisted(ulAddress);
	if(iIndex<0)
		return E_FAIL;
	m_VarAddress[iIndex]=0;

	return S_OK;
}

int CDataTracer::IsVarExisted(ULONG ulVarAddress)
{
	int i;
	for(i=0;i<DBG_REG_COUNT;i++)
	{
		if(m_VarAddress[i]==ulVarAddress)
		{
			return i;
		}
	}
	return -1;
}

HRESULT CDataTracer::StartTrace()
{
	CONTEXT cxt;
	HANDLE hThread=GetCurrentThread();

	cxt.ContextFlags=CONTEXT_DEBUG_REGISTERS;//|CONTEXT_FULL;
	if(!GetThreadContext(hThread,&cxt))
	{
		OutputDebugString("Failed to get thread context.\n");
		return E_FAIL;
	}
	cxt.Dr0=m_VarAddress[0];
	cxt.Dr1=m_VarAddress[1];
	cxt.Dr2=m_VarAddress[2];
	cxt.Dr3=m_VarAddress[3];

	cxt.Dr7=0;
	if(m_VarAddress[0]!=0)
		cxt.Dr7|=GetDR7(0,m_VarLength[0],m_VarReadWrite[0]);
	if(m_VarAddress[1]!=0)
		cxt.Dr7|=GetDR7(0,m_VarLength[1],m_VarReadWrite[1]);
	if(m_VarAddress[2]!=0)
		cxt.Dr7|=GetDR7(0,m_VarLength[2],m_VarReadWrite[2]);
	if(m_VarAddress[3]!=0)
		cxt.Dr7|=GetDR7(0,m_VarLength[3],m_VarReadWrite[3]);

	if(!SetThreadContext(hThread,&cxt))
	{
		OutputDebugString("Failed to set thread context.\n");
		return E_FAIL;
	}

	if(m_pVehHandler==NULL && RegVeh()!=S_OK)
		return E_FAIL;

	return S_OK;
}
ULONG CDataTracer::GetDR7(int nDbgRegNo, int nLen, BOOL bReadWrite)
{
	ULONG ulDR7=0;

	ulDR7|= (BIT_LOCAL_ENABLE<<(nDbgRegNo*2));
	// bit 0, 2, 4, 6 are for local breakpoint enable 
	//

	// read write bits 
	if(bReadWrite)
		ulDR7|=BIT_RW_RW<<(16+nDbgRegNo*4);
	else
		ulDR7|=BIT_RW_WO<<(16+nDbgRegNo*4);

	ulDR7|=nLen<<(16+nDbgRegNo*4+2);

	return ulDR7;
}

HRESULT CDataTracer::RegVeh()
{
    m_pVehHandler = AddVectoredExceptionHandler(0,
		DataTracerVectoredHandler);

	return (m_pVehHandler!=NULL)?S_OK:E_FAIL;
}
HRESULT CDataTracer::UnRegVeh()
{
	BOOL bRet=TRUE;
	if(m_pVehHandler!=NULL)
	{
		if(RemoveVectoredExceptionHandler(m_pVehHandler)==0)
			bRet = FALSE;
		else
			m_pVehHandler=NULL;
	}

	return bRet?S_OK:E_FAIL;
}
void ShowStackFrame(LPCTSTR szMsg, PVOID pParam)
{
	CDataTracer * pTracer=(CDataTracer *)pParam;
	if(szMsg==NULL)
		pTracer->ShowString(_T("------"));
	else
		pTracer->ShowString(szMsg);
}

HRESULT CDataTracer::HandleEvent(_EXCEPTION_POINTERS *ExceptionInfo)
{
	ULONG ulDR6;
	TCHAR szMsg[MAX_PATH]=_T("CDataTracer::HandleEvent");

	// check dr6 to see which break point was triggered.
	ulDR6=ExceptionInfo->ContextRecord->Dr6;

	for(int i=0;i<DBG_REG_COUNT;i++)
	{
		if( ( ulDR6&(1<<i) ) != 0) // bit i was set
		{
			_stprintf(szMsg,_T("Data at 0x%08x was accessed by code at 0x%08x."),
				m_VarAddress[i], ExceptionInfo->ExceptionRecord->ExceptionAddress);
			ShowString(szMsg);
		}
	}
	CCallTracer ct;
	ct.SetOptions(CALLTRACE_OPT_INFO_LEAN);
	ct.WalkStack(ShowStackFrame, this, 1000, ExceptionInfo->ContextRecord);

	return S_OK;
}

void CDataTracer::SetListener(HWND hListBox)
{
	m_hListBox=hListBox;
}

HRESULT CDataTracer::ClearAllDR()
{
	CONTEXT cxt;
	HANDLE hThread=GetCurrentThread();

	cxt.ContextFlags=CONTEXT_DEBUG_REGISTERS|CONTEXT_FULL;
	if(!GetThreadContext(hThread,&cxt))
	{
		OutputDebugString("Failed to get thread context.\n");
		return E_FAIL;
	}
	cxt.Dr0=0;
	cxt.Dr1=0;
	cxt.Dr2=0;
	cxt.Dr3=0;

	cxt.Dr7=0;

	if(!SetThreadContext(hThread,&cxt))
	{
		OutputDebugString("Failed to set thread context.\n");
		return E_FAIL;
	}
	return S_OK;
}

void CDataTracer::ShowString(LPCTSTR szMsg)
{
	if(m_hListBox)
	{
		SendMessage(m_hListBox,LB_ADDSTRING,
			0,(LPARAM)szMsg); 
	}
}
