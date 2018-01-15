// CallTracer.cpp: implementation of the CCallTracer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CallTracer.h"
#include <stdio.h>
#include <Psapi.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DWORD CCallTracer::m_dwSymClients=0;

CCallTracer::CCallTracer()
{
	m_dwSymClients++;
	if(m_dwSymClients<=1)
	{
		InitSymbols();
	}
	m_dwOptions=CALLTRACE_OPT_INFO_ALL;
}

CCallTracer::~CCallTracer()
{
	m_dwSymClients--;
	if(m_dwSymClients<=0)
	{
		FreeSymbols();
	}
}

HRESULT CCallTracer::InitSymbols()
{
	BOOL bRet=TRUE;
	DWORD dwOptions = SymGetOptions();

	SymSetOptions(dwOptions|SYMOPT_LOAD_LINES
		|SYMOPT_DEFERRED_LOADS
		|SYMOPT_OMAP_FIND_NEAREST);

	bRet = SymInitialize(GetCurrentProcess(),
								NULL,
								TRUE);
	return bRet?S_OK:E_FAIL;
}

HRESULT CCallTracer::FreeSymbols()
{
	BOOL bRet=TRUE;
	bRet=SymCleanup(GetCurrentProcess());
	return bRet?S_OK:E_FAIL;
}

HRESULT CCallTracer::WalkStack(PFN_SHOWFRAME pfnShowFrame,
							   PVOID pParam,int nMaxFrames,
							   PCONTEXT pContext)
{
	HRESULT hr=S_OK;
	STACKFRAME64 frame;
	int nCount=0;
	TCHAR szPath[MAX_PATH];
	DWORD dwTimeMS;

	dwTimeMS=GetTickCount();

	if(pContext==NULL)
		RtlCaptureContext(&m_Context);
	else
		m_Context=*pContext;

	memset(&frame, 0x0, sizeof(frame));

	frame.AddrPC.Offset    = m_Context.Eip;
	frame.AddrPC.Mode      = AddrModeFlat;
	frame.AddrFrame.Offset = m_Context.Ebp;
	frame.AddrFrame.Mode   = AddrModeFlat;
	frame.AddrStack.Offset = m_Context.Esp;
	frame.AddrStack.Mode   = AddrModeFlat;

	while (nCount < nMaxFrames)
	{
		nCount++;
		if (!StackWalk64(IMAGE_FILE_MACHINE_I386,
			GetCurrentProcess(), GetCurrentThread(),
			&frame, &m_Context,
			NULL, 
			SymFunctionTableAccess64, 
			SymGetModuleBase64, NULL))
		{
			// Error occured.
			break;
		}
		ShowFrame(&frame,pfnShowFrame,pParam);
		if (frame.AddrFrame.Offset == 0 || frame.AddrReturn.Offset == 0)
		{
			// End of stack.
			break;
		}
	}

	// Summary
	if(m_dwOptions&CALLTRACE_OPT_INFO_SUMMARY)
	{
		_stprintf(szPath,_T("Total Frames: %d; Spend %d MS"),
			nCount,	GetTickCount()-dwTimeMS);
		pfnShowFrame(szPath, pParam);
	}

	// Show symbol search path at last
	if(m_dwOptions&CALLTRACE_OPT_INFO_SEARCHPATH)
	{
		SymGetSearchPath(GetCurrentProcess(),szPath,MAX_PATH);
		pfnShowFrame(szPath, pParam);
	}
	return hr;
}
const int FRAME_MSG_SIZE=MAX_PATH*2;
const int MAX_SYM_SIZE=MAX_PATH*4;
HRESULT CCallTracer::ShowFrame(STACKFRAME64 * pFrame,
							   PFN_SHOWFRAME pfnShowFrame, PVOID pParam)
{
	HRESULT hr=S_OK;
	TCHAR szFrame[FRAME_MSG_SIZE];
	TCHAR szSym[MAX_SYM_SIZE];
	DWORD dwDisplacement=0;
	DWORD64 dwOffsetFromSmybol=0;
	IMAGEHLP_LINE64 line;

	// NULL indicates a new frame
	if(m_dwOptions & CALLTRACE_OPT_INFO_SEPT)
		pfnShowFrame(NULL, pParam);

	// Child EBP and Return address
	if(m_dwOptions & CALLTRACE_OPT_INFO_CHILDEBP)
	{
		_stprintf(szFrame,_T("Child EBP: 0x%08x, Return Address: 0x%08x"),
			DWORD(pFrame->AddrFrame.Offset),
			DWORD(pFrame->AddrReturn.Offset) );
		pfnShowFrame(szFrame, pParam);
	}

	// get module name
	IMAGEHLP_MODULE64 module;
	memset(&module,0,sizeof(IMAGEHLP_MODULE64));
	module.SizeOfStruct=sizeof(IMAGEHLP_MODULE64);

	if(!SymGetModuleInfo64(GetCurrentProcess(),
		pFrame->AddrPC.Offset,&module))
	{
		_tcscpy(module.ModuleName,_T("Unknown"));
	}
	// find symbols
	if(m_dwOptions & CALLTRACE_OPT_INFO_MODULE_FUNC)
	{
#ifndef USE_SymFromAddr
		PIMAGEHLP_SYMBOL64  pSymbol = (PIMAGEHLP_SYMBOL64 )szSym;
		ZeroMemory (pSymbol , sizeof(IMAGEHLP_SYMBOL64));
		pSymbol->MaxNameLength = MAX_SYM_SIZE-
			sizeof(IMAGEHLP_SYMBOL64)/sizeof(TCHAR);
		pSymbol->Address = pFrame->AddrPC.Offset;

		if (SymGetSymFromAddr64(GetCurrentProcess(),
				pFrame->AddrPC.Offset,
				&dwOffsetFromSmybol,
				pSymbol))
		{
			_stprintf(szFrame, _T("%s!%s"),
				module.ModuleName, pSymbol->Name);
			pfnShowFrame(szFrame, pParam);
		}
#else
		PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)szSym;
		ZeroMemory (pSymbol , sizeof(SYMBOL_INFO));
		pSymbol->MaxNameLen = MAX_SYM_SIZE-sizeof(SYMBOL_INFO)/sizeof(TCHAR);
		pSymbol->Address = pFrame->AddrPC.Offset;

		if (SymFromAddr(GetCurrentProcess(),
				pFrame->AddrPC.Offset,
				&dwOffsetFromSmybol,
				pSymbol))
		{
			_stprintf(szFrame, _T("%s!%s"),
				module.ModuleName, pSymbol->Name);
			pfnShowFrame(szFrame, pParam);
		}
#endif
	}
	// Parameters
	if(m_dwOptions & CALLTRACE_OPT_INFO_PARA)
	{
		_stprintf(szFrame,_T("Parameters: (0x%08x,0x%08x,0x%08x,0x%08x)"),
			pFrame->Params[0],pFrame->Params[1],
			pFrame->Params[2],pFrame->Params[3]);
		pfnShowFrame(szFrame, pParam);
	}
	// source line
	if(m_dwOptions & CALLTRACE_OPT_INFO_SRCLINE)
	{
		ZeroMemory ( &line , sizeof ( IMAGEHLP_LINE64 ) ) ;
		line.SizeOfStruct = sizeof ( IMAGEHLP_LINE64 ) ;

		if (SymGetLineFromAddr64(
				GetCurrentProcess ( )  ,
				pFrame->AddrPC.Offset,
				&dwDisplacement,
				&line))
		{
			pfnShowFrame(line.FileName, pParam);
			_stprintf(szFrame, _T("Line No:%d, Address"),
				line.LineNumber, line.Address);
		}
	}
	// Module& Symbols
	if(m_dwOptions & CALLTRACE_OPT_INFO_MODULE_SYMBOL)
	{
		pfnShowFrame(module.LoadedImageName, pParam);

		_stprintf(szFrame, 
			_T("SymType:%s;PdbUnmtchd:%d,DbgUnmthd:%d,LineNos:%d,GlblSym: %d,TypeInfo:%d"),
			GetSymType(module.SymType), module.PdbUnmatched,
			module.DbgUnmatched,module.LineNumbers, 
			module.GlobalSymbols,module.TypeInfo);
		pfnShowFrame(szFrame, pParam);

		if(module.SymType==SymPdb)
		{
			pfnShowFrame(module.LoadedPdbName, pParam);
		}
	}
	// FPO
	if(m_dwOptions & CALLTRACE_OPT_INFO_FPO)
	{
		if(pFrame->FuncTableEntry)
		{
			PFPO_DATA pFpo=(PFPO_DATA)pFrame->FuncTableEntry;
			_stprintf(szFrame, _T("FPO: Para dwords: %d; Regs: %d; Frame Type: %d "),
				pFpo->cdwParams, pFpo->cbRegs, pFpo->cbFrame);
			pfnShowFrame(szFrame, pParam);
		}
	}
	// misc
	if(m_dwOptions & CALLTRACE_OPT_INFO_MISC)
	{
		_stprintf(szFrame, _T("Far (WOW): %d; Virtual Frame: %d "),
			pFrame->Far, pFrame->Virtual);
		pfnShowFrame(szFrame, pParam);
	}
	return hr;
}



LPCTSTR CCallTracer::GetSymType(DWORD dwSymType)
{
	LPCTSTR lpszReturn=NULL;

	switch(dwSymType)
	{
	case SymNone:
		lpszReturn = "-nosymbols-";
		break;
	case SymCoff:
		lpszReturn = "COFF";
		break;
	case SymCv:
		lpszReturn = "CV";
		break;
	case SymPdb:
		lpszReturn = "PDB";
		break;
	case SymExport:
		lpszReturn = "-exported-";
		break;
	case SymDeferred:
		lpszReturn = "-deferred-";
		break;
	case SymSym:
		lpszReturn = "SYM";
		break;
	case SymVirtual: 
		lpszReturn = "Virtual";
		break;
	case SymDia: 
		lpszReturn = "DIA";
		break;
	default:
		lpszReturn = "-bad type-";
	}
	return lpszReturn;
}
