// Bugs.h: interface for the CBugs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUGS_H__8A7DB7E3_C8B6_473D_B98A_3492CF8BA5A5__INCLUDED_)
#define AFX_BUGS_H__8A7DB7E3_C8B6_473D_B98A_3492CF8BA5A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBugs  
{
public:
	void FireBug(int i);
	void FillListBox(CListBox & lb);
	CBugs();
	virtual ~CBugs();



};

#endif // !defined(AFX_BUGS_H__8A7DB7E3_C8B6_473D_B98A_3492CF8BA5A5__INCLUDED_)
