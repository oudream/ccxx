#ifndef CXWINREG_H
#define CXWINREG_H

#include "cxglobal.h"

class GM_CCXX_CORE_API CxWinReg
{
public:
    enum REG_CLASS{
        ROOT=0,
        CURRENT_USER,
        LOCAL_MACHINE,
        USERS,
        CURRENT_CONFIG
    };
    CxWinReg(CxWinReg::REG_CLASS e=CxWinReg::LOCAL_MACHINE);
    ~CxWinReg();    

    bool setType(CxWinReg::REG_CLASS e);

    bool open(const std::string &lpSubKey);

    void close();

    bool createKey(const std::string &lpSubKey);

    bool saveKey(const std::string &lpName);

    bool restoreKey(const std::string &lpName);

    bool read(const std::string &lpValueName, std::string* lpVal);

    bool read(const std::string &lpValueName, int* pnVal);

    bool write(const std::string &lpSubKey, std::string lpVal);

    bool write(const std::string &lpSubKey, int nVal);

    bool deleteKey(CxWinReg::REG_CLASS e, const std::string &lpSubKey);

    bool deleteValue(const std::string &lpValueName);

private:
#ifdef GM_OS_WIN32
    HKEY _hKey;
#endif

};

class GM_CCXX_CORE_API CxWinRegEx
{
public:
    static bool read(const std::string &lpSubKey,const std::string &lpValueName, std::string* lpVal,CxWinReg::REG_CLASS e=CxWinReg::LOCAL_MACHINE);
    static bool write(const std::string &lpSubKey,const std::string &lpValueName, std::string lpVal,CxWinReg::REG_CLASS e=CxWinReg::LOCAL_MACHINE);
};


#endif // CXWINREG_H
