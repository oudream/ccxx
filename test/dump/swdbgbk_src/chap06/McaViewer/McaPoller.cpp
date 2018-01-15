/*---------------------------------------------------------------------
McaPoller.cpp - Sample to read MCA registers.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/

#include "stdafx.h"
#include "McaViewer.h"
#include "McaPoller.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMcaPoller::CMcaPoller()
{
	m_nTotalBanks=0;
	m_bMCE=0;
	m_bMCA=0;
	m_bMcgCtlPresent=0;
	m_bMcgExtPresent=0;
	m_nMcgExtCount=0;
}

CMcaPoller::~CMcaPoller()
{

}
#define BYTE_MASK 0xFF
#define MASK_MCG_CTL_P (1<<8)
#define MASK_MCG_EXT_P (1<<9)
#define MASK_MCG_COUNT 0xFF0000
void CMcaPoller::DetectMCA(CListBox &lb)
{
	MSR_STRUCT msr;
	TCHAR szMsg[MAX_PATH];

	if(!IsCPUSupported())
	{
		lb.AddString("Only Intel(R) Pentium Pro or newer CPU is supported.");
		return;
	}
	if(!m_DvrAgent.IsInited())
		m_DvrAgent.Init();

	CheckMCASupport();
	sprintf(szMsg,"CPUID: MCA=%d, MCE=%d",m_bMCA,m_bMCE);
	lb.AddString(szMsg);

	msr.MsrNum=IA32_MCG_CAP;
	if(m_DvrAgent.RDMSR(msr)<0)
	{
		lb.AddString("Failed in reading IA32_MCG_CAP registers.");
		return;
	}
	//
	//the lowest byte is the number of error reporting banks 
	m_nTotalBanks=msr.MsrLo&BYTE_MASK;
	sprintf(szMsg,"Total error reporting banks: %d",m_nTotalBanks);
	lb.AddString(szMsg);
	
	m_bMcgCtlPresent=(msr.MsrLo&MASK_MCG_CTL_P)==MASK_MCG_CTL_P;
	m_bMcgExtPresent=(msr.MsrLo&MASK_MCG_EXT_P)==MASK_MCG_EXT_P;
	m_nMcgExtCount=(msr.MsrLo&MASK_MCG_COUNT)>>16;
	sprintf(szMsg,"MCG_CTL_P=%d,MCG_EXT_P=%d,MCG_EXT_COUNT=%d",
		m_bMcgCtlPresent,m_bMcgExtPresent,m_nMcgExtCount);
	lb.AddString(szMsg);

	if(m_bMcgCtlPresent)
	{
		msr.MsrNum=IA32_MCG_CTL;
		if(m_DvrAgent.RDMSR(msr)<0)
		{
			lb.AddString("Failed in reading IA32_MCG_CTL registers.");
		}
	}
}
#define CPU_FEATURE_MCA_BIT (1<<14)//bit 14;
#define CPU_FEATURE_MCE_BIT (1<<7)//bit 7;
void CMcaPoller::CheckMCASupport()
{
	DWORD dwFeatures=0;

	_asm
	{
		MOV EAX, 01
		CPUID
		MOV dwFeatures, EDX
	}
	m_bMCE=(dwFeatures&CPU_FEATURE_MCE_BIT)==CPU_FEATURE_MCE_BIT;
	m_bMCA=(dwFeatures&CPU_FEATURE_MCA_BIT)==CPU_FEATURE_MCA_BIT;
}
#define CPUID_LEN 12
BOOL CMcaPoller::IsCPUSupported()
{
	char szCpuID[CPUID_LEN+1];
	int nCpuID[CPUID_LEN/sizeof(int)];
	int nHighestValue=0;
	szCpuID[CPUID_LEN]=0;
	_asm
	{
		MOV EAX,0
		cpuid
		MOV nHighestValue, EAX
		MOV nCpuID[0 * TYPE int], EBX
		MOV nCpuID[1 * TYPE int], EDX
		MOV nCpuID[2 * TYPE int], ECX
	}
	memcpy(szCpuID,(char *)nCpuID,CPUID_LEN);
	return nHighestValue>=2 && stricmp(szCpuID,"GenuineIntel")==0;
}
#define MCA_MCIBANK_BASE 0x400
void CMcaPoller::PollBanks(CListBox &lb)
{
	MSR_STRUCT msr;
	TCHAR szMsg[MAX_PATH];
	//
	LPCTSTR szBankMSRs[]={"CTRL","STAT","ADDR","MISC"};
	//There are four MSRs per bank, we make the name in 
	// same length to improve display
	//

	if(m_nTotalBanks<=0)
		DetectMCA(lb);
	if(m_nTotalBanks<=0)
		return;

	msr.MsrNum=MCA_MCIBANK_BASE;
	for(int i=0;i<m_nTotalBanks;i++)
	{
		//loop each 
		for(int j=0;j<4;j++)
		{
			if(m_DvrAgent.RDMSR(msr)<0)
				sprintf(szMsg,"Failed in reading [%s]_MSR at bank %d.",
					szBankMSRs[j],i);
			else
				sprintf(szMsg,"[%s]_MSR at bank [%d]:%08X-%08X", 
					szBankMSRs[j],i,msr.MsrHi,msr.MsrLo);
			lb.AddString(szMsg);
			msr.MsrNum++;
		}
	}
}

void CMcaPoller::ClearBanks(CListBox &lb)
{
	MSR_STRUCT msr;
	TCHAR szMsg[MAX_PATH];
	//
	LPCTSTR szBankMSRs[]={"CTRL","STAT","ADDR","MISC"};
	//There are four MSRs per bank, we make the name in 
	// same length to improve display
	//

	if(m_nTotalBanks<=0)
		DetectMCA(lb);
	if(m_nTotalBanks<=0)
		return;

	msr.MsrNum=MCA_MCIBANK_BASE+1;//point to status register
	for(int i=0;i<m_nTotalBanks;i++)
	{
		msr.MsrLo=0;
		msr.MsrHi=0;
		if(m_DvrAgent.WRMSR(msr)<0)
			sprintf(szMsg,"Failed in writing STATUS_MSR at bank %d.",i);
		
		if(m_DvrAgent.RDMSR(msr)>=0)
			sprintf(szMsg,"STATUS_MSR at bank [%d] after write:%08X-%08X", 
				i,msr.MsrHi,msr.MsrLo);
		lb.AddString(szMsg);
		msr.MsrNum+=4;
	}
}
