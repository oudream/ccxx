// McaPoller.h: interface for the CMcaPoller class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCAPOLLER_H__C5B66703_50DA_4594_8B70_5E423371A4F7__INCLUDED_)
#define AFX_MCAPOLLER_H__C5B66703_50DA_4594_8B70_5E423371A4F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "dvragent.h"

#define IA32_MCG_CAP	0x179
#define IA32_MCG_STATUS 0x17A
#define IA32_MCG_CTL	0x17B

class CMcaPoller  
{
public:
	void ClearBanks(CListBox&lb);
	void PollBanks(CListBox&lb);
	BOOL IsCPUSupported();
	void CheckMCASupport();
	void DetectMCA(CListBox & lb);
	CMcaPoller();
	virtual ~CMcaPoller();
protected:
	CDvrAgent m_DvrAgent;
	BOOL	m_bMCE;
	BOOL	m_bMCA;
	int m_nTotalBanks;
	BOOL	m_bMcgCtlPresent;
	BOOL	m_bMcgExtPresent;
	int		m_nMcgExtCount;
};

#endif // !defined(AFX_MCAPOLLER_H__C5B66703_50DA_4594_8B70_5E423371A4F7__INCLUDED_)
