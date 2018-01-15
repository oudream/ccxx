// D4dObject.cpp: implementation of the CD4dObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D4dObject.h"
#include "D4dLogger.h"
#include <stdio.h>
#include <stdarg.h>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CD4dLogger D4D_API g_MsgLogger;

CD4dObject::CD4dObject()
{

}

CD4dObject::~CD4dObject()
{

}
const int MAX_LOG_SIZE=1024;
void CD4dObject::Log(LPCTSTR szSrcFile,LPCTSTR szFunction, 
					 UINT nLogType,LPCTSTR szMsgFormat,...)
{
	TCHAR szModule[MAX_PATH];
	TCHAR szMsg[MAX_LOG_SIZE];

	strcpy(szModule,szSrcFile);
	strcat(szModule,"::");
	strcat(szModule,szFunction);

    va_list va;
    va_start( va, szMsgFormat );

    int nReturnValue = _vsntprintf( szMsg, 
		MAX_LOG_SIZE, szMsgFormat, va );

    va_end(va);
	g_MsgLogger.Log(szModule,nLogType,szMsg);
}
void CD4dObject::Msg(UINT nResID,...)
{
}
