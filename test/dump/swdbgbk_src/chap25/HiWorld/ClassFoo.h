#pragma once
#include "baseclass.h"
#include <string>

class CClassFoo :
	public CBaseClass
{
public:
	CClassFoo(void);
public:
	~CClassFoo(void);
	virtual int Run(LPARAM lParam);
};
