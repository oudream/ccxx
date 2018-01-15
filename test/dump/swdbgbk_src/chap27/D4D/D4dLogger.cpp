// D4dLogger.cpp: implementation of the CD4dLogger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D4dLogger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD4dLogger::CD4dLogger()
{

}

CD4dLogger::~CD4dLogger()
{

}
DWORD CD4dLogger::UnitTest (DWORD dwParaFlags)
{
	return S_OK;
}
DWORD CD4dLogger::Dump(HANDLE hFile)
{
	return S_OK;
}
HRESULT CD4dLogger::Log(LPCTSTR lpszModule,	UINT nLogType,	LPCTSTR lpszMessage)
{
	OutputDebugString(lpszModule);
	OutputDebugString(lpszMessage);
	OutputDebugString(SZ_NEW_LINE);
	return S_OK;
}
