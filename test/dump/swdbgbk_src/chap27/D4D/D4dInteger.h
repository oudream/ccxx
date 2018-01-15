// D4dInteger.h: interface for the CD4dInteger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_D4DINTEGER_H__CBB55AB8_34E4_45A1_ABC0_E47FF78F94A2__INCLUDED_)
#define AFX_D4DINTEGER_H__CBB55AB8_34E4_45A1_ABC0_E47FF78F94A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "D4dObject.h"

const int MIN_TRACK_DEPTH=1;
class D4D_API CD4dInteger : public CD4dObject  
{
protected:
	long m_nTrackerIndex;
	long * m_pTracker;
	long m_nTrackDepth;
	long m_nCurValue;
public:
	long CurValue(){return m_nCurValue;};
	long GetTrace(int nBackStep);
	long GetTrackDepth(){return m_nTrackDepth;}
	CD4dInteger& operator =(long nValue);

	CD4dInteger(int nTrackDepth=1024);
	virtual ~CD4dInteger();
	virtual DWORD UnitTest (DWORD dwParaFlags); 
	virtual DWORD Dump(HANDLE hFile);
};

#endif // !defined(AFX_D4DINTEGER_H__CBB55AB8_34E4_45A1_ABC0_E47FF78F94A2__INCLUDED_)
