#include "StdAfx.h"
#include "BaseClass.h"

CBaseClass::CBaseClass(void)
{
}

CBaseClass::~CBaseClass(void)
{
}
int CBaseClass::Setup(LPCTSTR lpszName)
{
	if(lpszName==NULL ||
		_tcslen(lpszName)>=MAX_PATH)
		return FALSE;

	_tcscpy_s(m_szName,MAX_PATH,lpszName);

	return TRUE;
}
