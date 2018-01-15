#pragma once

class CBaseClass
{
private:
	int m_nPrivate;
protected:
	TCHAR m_szName[MAX_PATH];
public:
	CBaseClass(void);
	~CBaseClass(void);
	virtual int Run(LPARAM lpParam)=0;
	__declspec(noinline) virtual int Setup(LPCTSTR szName);
	LPCTSTR GetName()const{return m_szName;} 
	__event void f();
};
