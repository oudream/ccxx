// D4dObject.h: interface for the CD4dObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_D4DOBJECT_H__08E55E5B_8E9B_4902_B6E0_5C0820DFC313__INCLUDED_)
#define AFX_D4DOBJECT_H__08E55E5B_8E9B_4902_B6E0_5C0820DFC313__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <tchar.h>

#ifdef D4D_EXPORTS
#define D4D_API __declspec(dllexport)
#else
#define D4D_API __declspec(dllimport)
#endif
const TCHAR SZ_NEW_LINE[]=_T("\r\n");
class D4D_API CD4dObject  
{
public:
	CD4dObject();
	virtual ~CD4dObject();

	void Log(LPCTSTR szSrcFile,LPCTSTR szFunction,
		UINT uLogLevel,LPCTSTR szMsgFormat,...);
	void Msg(UINT nResID,...);

	virtual DWORD UnitTest (DWORD dwParaFlags)=0; // 强制实现单元测试方法 
	virtual DWORD Dump(HANDLE hFile)=0;
};

#endif // !defined(AFX_D4DOBJECT_H__08E55E5B_8E9B_4902_B6E0_5C0820DFC313__INCLUDED_)
