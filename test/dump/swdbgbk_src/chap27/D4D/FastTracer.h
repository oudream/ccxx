// FastTracer.h: interface for the CFastTracer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASTTRACER_H__FB7E24E8_447A_41CE_A3C2_C918096834E2__INCLUDED_)
#define AFX_FASTTRACER_H__FB7E24E8_447A_41CE_A3C2_C918096834E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "D4dObject.h"

typedef ULONG (WINAPI *PFN_RTLWALKFRAMECHAIN)(PVOID *pReturnAddresses,
    DWORD dwCount, DWORD dwFlags);

class D4D_API CFastTracer  
{
public:
	DWORD GetFrameChain(PVOID * pReturnAddresses, DWORD dwCount);
	HRESULT Setup();
	CFastTracer();
	virtual ~CFastTracer();
protected:
	PFN_RTLWALKFRAMECHAIN m_pfnWalkFrameChain;
};

#endif // !defined(AFX_FASTTRACER_H__FB7E24E8_447A_41CE_A3C2_C918096834E2__INCLUDED_)
