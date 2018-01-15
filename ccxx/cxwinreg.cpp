#include "cxwinreg.h"

#ifdef  GM_OS_WIN

#undef UNICODE
#include <windows.h>
#include <iostream>
#include <strsafe.h>
#include <winreg.h>


HKEY f_getRoot(int nType)
{
    HKEY _hKey = NULL;
    switch(nType)
    {
    case 0:
        _hKey = HKEY_CLASSES_ROOT;
        break;
    case 1:
        _hKey = HKEY_CURRENT_USER;
        break;
    case 2:
        _hKey = HKEY_LOCAL_MACHINE;
        break;
    case 3:
        _hKey = HKEY_USERS;
        break;
    case 4:
        _hKey = HKEY_CURRENT_CONFIG;
        break;
    default:
        break;
    }
    return _hKey;
}
////////////////////////////////////////////////////////////////
CxWinReg::CxWinReg(REG_CLASS e)
{
    _hKey = f_getRoot((int)e);
}

CxWinReg::~CxWinReg()
{
    close();
}

bool CxWinReg::setType(CxWinReg::REG_CLASS e)
{
    close();
    _hKey = f_getRoot((int)e);
	return true;
}



bool CxWinReg::open(const std::string &lpSubKey)
{
    assert(_hKey);

    if(_hKey!=NULL)
    {
        HKEY hKey;
        long lReturn=RegOpenKeyEx(_hKey,lpSubKey.c_str(),0L,KEY_ALL_ACCESS,&hKey);

        if(lReturn==ERROR_SUCCESS)
        {
            _hKey=hKey;
            return TRUE;
        }
    }
    return FALSE;
}

void CxWinReg::close()
{
    if(_hKey)
    {
        RegCloseKey(_hKey);
        _hKey=NULL;
    }
}

bool CxWinReg::createKey(const std::string &lpSubKey)
{
    assert(_hKey);

    HKEY hKey;
    DWORD dw;
    long lReturn=RegCreateKeyEx(_hKey,lpSubKey.c_str(),0L,NULL,REG_OPTION_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,&dw);

    if(lReturn==ERROR_SUCCESS)
    {
        _hKey=hKey;
        return TRUE;
    }

    return FALSE;
}

bool CxWinReg::saveKey(const std::string &lpName)
{
    assert(_hKey);

    long lReturn=RegSaveKey(_hKey,lpName.c_str(),NULL);

    if(lReturn==ERROR_SUCCESS)
        return TRUE;

    return FALSE;
}

bool CxWinReg::restoreKey(const std::string &lpName)
{
    assert(_hKey);

    long lReturn=RegRestoreKey(_hKey,lpName.c_str(),REG_WHOLE_HIVE_VOLATILE);

    if(lReturn==ERROR_SUCCESS)
        return TRUE;

    return FALSE;
}

bool CxWinReg::read(const std::string &lpValueName, std::string *lpVal)
{
    assert(_hKey);

    DWORD dwType;
    DWORD dwSize=200;
    char szString[2550];

    long lReturn=RegQueryValueEx(_hKey,lpValueName.c_str(),NULL,&dwType,(BYTE *)szString,&dwSize);

    if(lReturn==ERROR_SUCCESS)
    {
        *lpVal=szString;
        return TRUE;
    }
    return FALSE;
}

bool CxWinReg::read(const std::string &lpValueName, int *pnVal)
{
    assert(_hKey);

    DWORD dwType;
    DWORD dwSize=sizeof(DWORD);
    DWORD dwDest;
    long lReturn=RegQueryValueEx(_hKey,lpValueName.c_str(),NULL,&dwType,(BYTE *)&dwDest,&dwSize);

    if(lReturn==ERROR_SUCCESS)
    {
        *pnVal=(int)dwDest;
        return TRUE;
    }
    return FALSE;
}

bool CxWinReg::write(const std::string &lpSubKey, std::string lpVal)
{
    assert(_hKey);

    long lReturn=RegSetValueEx(_hKey,lpSubKey.c_str(),0L,REG_SZ,(const BYTE *) lpVal.c_str(),lpVal.length()+1);

    if(lReturn==ERROR_SUCCESS)
        return TRUE;

    return FALSE;
}

bool CxWinReg::write(const std::string &lpSubKey, int nVal)
{
    assert(_hKey);

    DWORD dwVal = nVal;

    long lReturn=RegSetValueEx(_hKey,lpSubKey.c_str(),0L,REG_DWORD,(const BYTE *) &dwVal,sizeof(DWORD));

    if(lReturn==ERROR_SUCCESS)
        return TRUE;

    return FALSE;
}

bool CxWinReg::deleteValue(const std::string &lpValueName)
{
    assert(_hKey);

    long lReturn=RegDeleteValue(_hKey,lpValueName.c_str());

    if(lReturn==ERROR_SUCCESS)
        return TRUE;
    return FALSE;
}




bool CxWinReg::deleteKey(REG_CLASS e, const std::string &lpSubKey)
{
    HKEY hKey = f_getRoot((int)e);

    assert(hKey);

    long lReturn=RegDeleteValue(hKey,lpSubKey.c_str());

    if(lReturn==ERROR_SUCCESS)
        return TRUE;
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////
bool CxWinRegEx::read(const std::string &lpSubKey,const std::string &lpValueName, std::string *lpVal, CxWinReg::REG_CLASS e)
{
    bool ret = false;
    CxWinReg a(e);
    if(a.open(lpSubKey))
    {
       ret = a.read(lpValueName,lpVal);
    }
    a.close();
    return ret;
}

bool CxWinRegEx::write(const std::string &lpSubKey,const std::string &lpValueName, std::string lpVal, CxWinReg::REG_CLASS e)
{
    bool ret = false;
    CxWinReg a(e);
    if(a.createKey(lpSubKey))
    {
       ret = a.write(lpValueName,lpVal);
    }
    a.close();
    return ret;

}
#endif
