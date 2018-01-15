// D4dInteger.cpp: implementation of the CD4dInteger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D4dInteger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD4dInteger::CD4dInteger(int nTrackDepth)
	:m_nTrackDepth(nTrackDepth)
{
	m_nTrackerIndex=-1;
	if(m_nTrackDepth<=0)
		m_nTrackDepth=MIN_TRACK_DEPTH;
	m_pTracker=new long[m_nTrackDepth];
}

CD4dInteger::~CD4dInteger()
{
	if(m_pTracker)
		delete [] m_pTracker;
}
CD4dInteger& CD4dInteger::operator =(long nValue)
{
	int nIndex = InterlockedIncrement (&m_nTrackerIndex);
	nIndex %= m_nTrackDepth;

	this->m_nCurValue=nValue;
	this->m_pTracker[nIndex] = nValue;

	return *this;
}
DWORD CD4dInteger::UnitTest (DWORD dwParaFlags)
{
	return S_OK;
}
DWORD CD4dInteger::Dump(HANDLE hFile)
{
	return S_OK;
}
long CD4dInteger::GetTrace(int nBackStep)
{
	nBackStep%=m_nTrackDepth;

	int nIndex=m_nTrackerIndex-nBackStep;

	if(nIndex<0)
		nIndex+=m_nTrackDepth;

	return m_pTracker[nIndex];
}

