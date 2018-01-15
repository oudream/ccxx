// Created by Microsoft (R) C/C++ Compiler Version 14.00.50727.42
//
// c:\dig\dbg\author\code\chap25\hiworld\baseclass.mrg.cpp
// compiler-generated file created 03/09/08 at 14:58:35
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

#include "StdAfx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

#pragma message("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "BaseClass.h"

CBaseClass::CBaseClass(void)
{
	//+++ Start Injected Code For Attribute 'event' from CBaseClass::CBaseClass
#injected_line 15 "c:\\dig\\dbg\\author\\code\\chap25\\hiworld\\BaseClass.h"
    __eventHandlerList_CBaseClass_f = 0;

	//--- End Injected Code For Attribute 'event'

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
