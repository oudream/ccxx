// DataTracer.h: interface for the CDataTracer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATATRACER_H__0E080E30_E52C_47AC_975B_659F73C2CCF3__INCLUDED_)
#define AFX_DATATRACER_H__0E080E30_E52C_47AC_975B_659F73C2CCF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "D4dObject.h"

const int DBG_REG_COUNT=4; // it can't be less than 4
#define BIT_LOCAL_ENABLE 1
#define BIT_RW_RW 3 // 11
#define BIT_RW_WO 1 // 01 // break on write only
class D4D_API CDataTracer  
{
public:
	HRESULT HandleEvent(struct _EXCEPTION_POINTERS * ExceptionInfo);
	ULONG GetDR7(int nDbgRegNo, int nLen, BOOL bReadWrite);
	HRESULT StartTrace();
	BOOL IsVarExisted(ULONG ulVarAddress);
	HRESULT RemoveVar(ULONG ulAddress);
	HRESULT AddVar(ULONG ulVarAddress,int nLen, int nReadWrite);
	CDataTracer();
	virtual ~CDataTracer();
	void ShowString(LPCTSTR szMsg);
	HRESULT ClearAllDR();
	void SetListener(HWND hListBox);
protected:
	HRESULT RegVeh();
	HRESULT UnRegVeh();
	ULONG m_VarAddress[DBG_REG_COUNT];
	ULONG m_VarLength[DBG_REG_COUNT];
	ULONG m_VarReadWrite[DBG_REG_COUNT];
	PVOID m_pVehHandler;
	HWND m_hListBox;// a list box to recieve data access messages
};
extern D4D_API CDataTracer * g_pDataTracer; 
#endif // !defined(AFX_DATATRACER_H__0E080E30_E52C_47AC_975B_659F73C2CCF3__INCLUDED_)
