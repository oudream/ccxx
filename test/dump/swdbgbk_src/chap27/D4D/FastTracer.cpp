// FastTracer.cpp: implementation of the CFastTracer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FastTracer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFastTracer::CFastTracer()
{
	m_pfnWalkFrameChain=NULL;
}

CFastTracer::~CFastTracer()
{

}

HRESULT CFastTracer::Setup()
{
	HMODULE hNtDll;
	if(m_pfnWalkFrameChain!=NULL)
		return S_OK;

	hNtDll=LoadLibrary("NTDLL.DLL");
	if(hNtDll==NULL)
	{
		OutputDebugString("GetErrorMode: Failed in loading NTDLL.\n");
		return E_FAIL;
	}
	m_pfnWalkFrameChain=(PFN_RTLWALKFRAMECHAIN)
		GetProcAddress(hNtDll,"RtlWalkFrameChain");
	
	FreeLibrary(hNtDll);
	return S_OK;
}

DWORD CFastTracer::GetFrameChain(PVOID *pReturnAddresses, DWORD dwCount)
{
	if(m_pfnWalkFrameChain==NULL)
		return E_FAIL;

	return m_pfnWalkFrameChain(pReturnAddresses,dwCount,0);
}
