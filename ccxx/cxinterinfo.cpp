#include "cxinterinfo.h"

#include "cxcontainer.h"
#include "cxstring.h"
#include "cxtime.h"
#include "cxthread.h"
#include "cxapplication.h"
#include "cxxml.h"


using namespace std;

const CxEndFlag cxEnd = CxEndFlag();
const CxLineFlag cxLine = CxLineFlag();
const CxLineEndFlag cxEndLine = CxLineEndFlag();

CxConsoleInterinfo * fn_consoleInterinfoStart()
{
    CxApplication::registStartFunction(CxConsoleInterinfo::start);
    CxApplication::registStopFunction(CxConsoleInterinfo::stop);
    static CxConsoleInterinfo consoleInterinfo;
    return & consoleInterinfo;
}


static CxConsoleInterinfo * s_consoleInterinfo = fn_consoleInterinfoStart();


static volatile int f_bConsoleFilter = 0;



CxMutex * fn_getInterinfoOutSubjectsLock()
{
    static CxMutex interInfoLock;
    return & interInfoLock;
}

vector<CxInterinfoOut_I*> * fn_getInterinfoOutSubjects()
{
    static vector<CxInterinfoOut_I*> interinfoSubjects;
    return & interinfoSubjects;
}

static CxMutex * f_interinfoOutSubjectsLock = fn_getInterinfoOutSubjectsLock();









CxInterinfo::PlatformEnum CxInterinfo::platformFrom(const string & sPlatform)
{
    CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None;
    if (sPlatform == "desktop")
        ePlatform = CxInterinfo::Platform_Desktop;
    else if (sPlatform == "cmd")
        ePlatform = CxInterinfo::Platform_Cmd;
    else if (sPlatform == "log")
        ePlatform = CxInterinfo::Platform_Log;
    else if (sPlatform == "net")
        ePlatform = CxInterinfo::Platform_Net;

    return ePlatform;
}

string CxInterinfo::platformToString(CxInterinfo::PlatformEnum e)
{
    switch (e)
    {
    case Platform_Desktop:  return "desktop"; break;
    case Platform_Cmd:      return "cmd"; break;
    case Platform_Log:      return "log"; break;
    case Platform_Net:      return "net"; break;
    default :               return "none"; break;
    }
}

CxInterinfo::TypeEnum CxInterinfo::typeFrom(const string & sType)
{
    CxInterinfo::TypeEnum eType = CxInterinfo::Type_None;

    if (sType ==      "platform")       eType = CxInterinfo::Type_Platform    ;
    else if (sType == "program")        eType = CxInterinfo::Type_Program     ;
    else if (sType == "safe")           eType = CxInterinfo::Type_Safe        ;
    else if (sType == "io_ram")         eType = CxInterinfo::Type_IO_Ram      ;
    else if (sType == "io_rom")         eType = CxInterinfo::Type_IO_Rom      ;
    else if (sType == "io_file")        eType = CxInterinfo::Type_IO_File     ;
    else if (sType == "io_channel")     eType = CxInterinfo::Type_IO_Channel  ;
    else if (sType == "io_tcpclient")   eType = CxInterinfo::Type_IO_Tcpclient;
    else if (sType == "io_tcpserver")   eType = CxInterinfo::Type_IO_Tcpserver;
    else if (sType == "io_udp")         eType = CxInterinfo::Type_IO_Udp      ;
    else if (sType == "io_icmp")        eType = CxInterinfo::Type_IO_Icmp     ;
    else if (sType == "io_serial")      eType = CxInterinfo::Type_IO_Serial   ;
    else if (sType == "protocol")       eType = CxInterinfo::Type_Protocol    ;
    else if (sType == "terminal")       eType = CxInterinfo::Type_Terminal    ;
    else if (sType == "business")       eType = CxInterinfo::Type_Business    ;
    else if (sType == "task")           eType = CxInterinfo::Type_Task        ;

    return eType;
}

string CxInterinfo::typeToString(CxInterinfo::TypeEnum e)
{
    switch (e)
    {
    case Type_Platform    :  return "platform"    ; break;
    case Type_Program     :  return "program"     ; break;
    case Type_Safe        :  return "safe"        ; break;
    case Type_IO_Ram      :  return "io_ram"      ; break;
    case Type_IO_Rom      :  return "io_rom"      ; break;
    case Type_IO_File     :  return "io_file"     ; break;
    case Type_IO_Channel  :  return "io_channel"  ; break;
    case Type_IO_Tcpclient:  return "io_tcpclient"; break;
    case Type_IO_Tcpserver:  return "io_tcpserver"; break;
    case Type_IO_Udp      :  return "io_udp"      ; break;
    case Type_IO_Icmp     :  return "io_icmp"     ; break;
    case Type_IO_Serial   :  return "io_serial"   ; break;
    case Type_Protocol    :  return "protocol"    ; break;
    case Type_Terminal    :  return "terminal"    ; break;
    case Type_Business    :  return "business"    ; break;
    case Type_Task        :  return "task"        ; break;
    default :                return "none"         ; break;
    }
}








void CxInterinfoOut::addObserver(CxInterinfoOut_I *oSubject)
{
    CxMutex * oLock = fn_getInterinfoOutSubjectsLock();
    vector<CxInterinfoOut_I*> * oSubjects = fn_getInterinfoOutSubjects();
    CxMutexScope lock(oLock);
    if ( find(oSubjects->begin(), oSubjects->end(), oSubject) ==  oSubjects->end() )
    {
        oSubjects->push_back(oSubject);
    }
}

void CxInterinfoOut::removeObserver(CxInterinfoOut_I *oSubject)
{
    CxMutex * oLock = fn_getInterinfoOutSubjectsLock();
    vector<CxInterinfoOut_I*> * oSubjects = fn_getInterinfoOutSubjects();
    CxMutexScope lock(oLock);
    CxContainer::remove(*oSubjects, oSubject);
}

void CxInterinfoOut::outInfo(const string &sInfo, const std::string& sTitle, int type, int reason, int source, int target, int iTag)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*>::iterator itBegin = fn_getInterinfoOutSubjects()->begin();
    vector<CxInterinfoOut_I*>::iterator itEnd = fn_getInterinfoOutSubjects()->end();
    for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
    {
        CxInterinfoOut_I * oInterinfoOut = (* it);
        bool bIsEnable = oInterinfoOut->filter()->isEnable(type, source, reason);
        if (oInterinfoOut->filter()->getIsReverse())
            bIsEnable = ! bIsEnable;
        if (bIsEnable)
        {
            oInterinfoOut->interinfo_out(sInfo, sTitle, type, reason, source, target, iTag);
        }
    }
}

int CxInterinfoOut::isEnable(CxInterinfo::PlatformEnum ePlatform, int type, int source, int reason)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*>::iterator itBegin = fn_getInterinfoOutSubjects()->begin();
    vector<CxInterinfoOut_I*>::iterator itEnd = fn_getInterinfoOutSubjects()->end();
    for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
    {
        CxInterinfoOut_I * oInterinfoOut = (* it);
        if (oInterinfoOut->platformValue() == ePlatform)
        {
            return oInterinfoOut->filter()->isEnable(type, source, reason);
        }
    }
    return false;
}

void CxInterinfoOut::enableAll(CxInterinfo::PlatformEnum ePlatform)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*>::iterator itBegin = fn_getInterinfoOutSubjects()->begin();
    vector<CxInterinfoOut_I*>::iterator itEnd = fn_getInterinfoOutSubjects()->end();
    if (ePlatform != CxInterinfo::Platform_None)
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            if (oInterinfoOut->platformValue() == ePlatform)
            {
                oInterinfoOut->filter()->enableAll();
            }
        }
    }
    else
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            oInterinfoOut->filter()->enableAll();
        }
    }
}

void CxInterinfoOut::diableAll(CxInterinfo::PlatformEnum ePlatform)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*>::iterator itBegin = fn_getInterinfoOutSubjects()->begin();
    vector<CxInterinfoOut_I*>::iterator itEnd = fn_getInterinfoOutSubjects()->end();
    if (ePlatform != CxInterinfo::Platform_None)
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            if (oInterinfoOut->platformValue() == ePlatform)
            {
                oInterinfoOut->filter()->disableAll();
            }
        }
    }
    else
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            oInterinfoOut->filter()->disableAll();
        }
    }
}

void CxInterinfoOut::enableType(int type, CxInterinfo::PlatformEnum ePlatform, bool bOnly)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*>::iterator itBegin = fn_getInterinfoOutSubjects()->begin();
    vector<CxInterinfoOut_I*>::iterator itEnd = fn_getInterinfoOutSubjects()->end();
    if (ePlatform != CxInterinfo::Platform_None)
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            if (oInterinfoOut->platformValue() == ePlatform)
            {
                if (bOnly)
                {
                    oInterinfoOut->filter()->enableAll();
                    oInterinfoOut->filter()->setReverse(true);
                }
                else
                {
                    oInterinfoOut->filter()->setReverse(false);
                }
                oInterinfoOut->filter()->enableType(type);
            }
        }
    }
    else
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            if (bOnly)
            {
                oInterinfoOut->filter()->enableAll();
                oInterinfoOut->filter()->setReverse(true);
            }
            else
            {
                oInterinfoOut->filter()->setReverse(false);
            }
            oInterinfoOut->filter()->enableType(type);
        }
    }
}

void CxInterinfoOut::disableType(int type, CxInterinfo::PlatformEnum ePlatform)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*>::iterator itBegin = fn_getInterinfoOutSubjects()->begin();
    vector<CxInterinfoOut_I*>::iterator itEnd = fn_getInterinfoOutSubjects()->end();
    if (ePlatform != CxInterinfo::Platform_None)
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            if (oInterinfoOut->platformValue() == ePlatform)
            {
                oInterinfoOut->filter()->disableType(type);
            }
        }
    }
    else
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            oInterinfoOut->filter()->disableType(type);
        }
    }
}

void CxInterinfoOut::enableType(int type, int source, CxInterinfo::PlatformEnum ePlatform, bool bOnly)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*>::iterator itBegin = fn_getInterinfoOutSubjects()->begin();
    vector<CxInterinfoOut_I*>::iterator itEnd = fn_getInterinfoOutSubjects()->end();
    if (ePlatform != CxInterinfo::Platform_None)
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            if (oInterinfoOut->platformValue() == ePlatform)
            {
                if (bOnly)
                {
                    oInterinfoOut->filter()->enableAll();
                    oInterinfoOut->filter()->setReverse(true);
                }
                else
                {
                    oInterinfoOut->filter()->setReverse(false);
                }
                oInterinfoOut->filter()->enableType(type, source);
            }
        }
    }
    else
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            if (bOnly)
            {
                oInterinfoOut->filter()->enableAll();
                oInterinfoOut->filter()->setReverse(true);
            }
            else
            {
                oInterinfoOut->filter()->setReverse(false);
            }
            oInterinfoOut->filter()->enableType(type, source);
        }
    }
}

void CxInterinfoOut::disableType(int type, int source, CxInterinfo::PlatformEnum ePlatform)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*>::iterator itBegin = fn_getInterinfoOutSubjects()->begin();
    vector<CxInterinfoOut_I*>::iterator itEnd = fn_getInterinfoOutSubjects()->end();
    if (ePlatform != CxInterinfo::Platform_None)
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            if (oInterinfoOut->platformValue() == ePlatform)
            {
                oInterinfoOut->filter()->disableType(type, source);
            }
        }
    }
    else
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            oInterinfoOut->filter()->disableType(type, source);
        }
    }
}

void CxInterinfoOut::enableType(int type, int source, int reason, CxInterinfo::PlatformEnum ePlatform, bool bOnly)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*>::iterator itBegin = fn_getInterinfoOutSubjects()->begin();
    vector<CxInterinfoOut_I*>::iterator itEnd = fn_getInterinfoOutSubjects()->end();
    if (ePlatform != CxInterinfo::Platform_None)
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            if (oInterinfoOut->platformValue() == ePlatform)
            {
                if (bOnly)
                {
                    oInterinfoOut->filter()->enableAll();
                    oInterinfoOut->filter()->setReverse(true);
                }
                else
                {
                    oInterinfoOut->filter()->setReverse(false);
                }
                oInterinfoOut->filter()->enableType(type, source, reason);
            }
        }
    }
    else
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            if (bOnly)
            {
                oInterinfoOut->filter()->enableAll();
                oInterinfoOut->filter()->setReverse(true);
            }
            else
            {
                oInterinfoOut->filter()->setReverse(false);
            }
            oInterinfoOut->filter()->enableType(type, source, reason);
        }
    }
}

void CxInterinfoOut::disableType(int type, int source, int reason, CxInterinfo::PlatformEnum ePlatform)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*>::iterator itBegin = fn_getInterinfoOutSubjects()->begin();
    vector<CxInterinfoOut_I*>::iterator itEnd = fn_getInterinfoOutSubjects()->end();
    if (ePlatform != CxInterinfo::Platform_None)
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            if (oInterinfoOut->platformValue() == ePlatform)
            {
                oInterinfoOut->filter()->disableType(type, source, reason);
            }
        }
    }
    else
    {
        for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
        {
            CxInterinfoOut_I * oInterinfoOut = (* it);
            oInterinfoOut->filter()->disableType(type, source, reason);
        }
    }
}

void CxInterinfoOut::updateByXml(const string &sFilePath)
{
    vector<map<string, string> > rows;
    CxXml::loadTable4Level(sFilePath, rows, "", "interinfo", "");
    update(rows);
}

void CxInterinfoOut::updateByXml(const char *pData, int iLength)
{
    vector<map<string, string> > rows;
    CxXml::loadTable4Level(pData, iLength, rows, "", "interinfo", "");
    update(rows);
}

void CxInterinfoOut::update(const string &sContent)
{

    map<string, string> row = CxString::splitToMap(sContent, '=', ';');
    update(row);
}

void CxInterinfoOut::update(const std::vector<std::map<string, string> > &rows)
{
    for (size_t i = 0; i < rows.size(); ++i)
    {
        const map<string, string> & row = rows.at(i);
        update(row);
    }
}

void CxInterinfoOut::update(const std::map<string, string> &row)
{
    const string sPlatform = CxContainer::value(row, string("platform"));
    const string sType = CxContainer::value(row, string("type"));
    const string sSource = CxContainer::value(row, string("source"));
    const string sReason = CxContainer::value(row, string("reason"));

    if (CxContainer::contain(row, string("enable")))
    {
        update(true, sPlatform, sType, sSource, sReason);
    }
    else if (CxContainer::contain(row, string("disable")))
    {
        update(false, sPlatform, sType, sSource, sReason);
    }
}


void CxInterinfoOut::update( bool bEnable, const string &sPlatform, const string &sType, const string &sSource, const string &sReason )
{
    CxInterinfo::PlatformEnum ePlatform = CxInterinfo::platformFrom(sPlatform);
    if (ePlatform == CxInterinfo::Platform_None)
        return;

    CxInterinfo::TypeEnum eType = CxInterinfo::typeFrom(sType);
    if (eType == CxInterinfo::Type_None)
        return;

    int iSource = CxString::toInt32(sSource);
    int iReason = CxString::toInt32(sReason);

    if (iSource > 0)
    {
        if (iReason > 0)
        {
            if (bEnable)
                enableType(eType, iSource, iReason, ePlatform);
            else
                disableType(eType, iSource, iReason, ePlatform);
        }
        else
        {
            if (bEnable)
                enableType(eType, iSource, ePlatform);
            else
                disableType(eType, iSource, ePlatform);
        }
    }
    else
    {
        if (bEnable)
            enableType(eType, ePlatform);
        else
            disableType(eType, ePlatform);
    }
}

CxInterinfoOut_I *CxInterinfoOut::findInterinfoOut(CxInterinfo::PlatformEnum ePlatform)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*>::iterator itBegin = fn_getInterinfoOutSubjects()->begin();
    vector<CxInterinfoOut_I*>::iterator itEnd = fn_getInterinfoOutSubjects()->end();
    for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
    {
        CxInterinfoOut_I * oInterinfoOut = (* it);
        if (oInterinfoOut->platformValue() == ePlatform)
        {
            return oInterinfoOut;
        }
    }
    return NULL;
}

static volatile int f_iThreadInputStatus = 1;

class ThreadInputPrivate : public CxDetachedThread
{
public:
    void run()
    {
        std::string sLine;
        while ( f_iThreadInputStatus )
        {
            //needtodo 怎么样中断此 stdin
            std::getline (std::cin, sLine);

            if (f_iThreadInputStatus)
            {
                CxConsoleInterinfo::updateInputString(sLine);
            }
        }
    }

};

ThreadInputPrivate f_threadInput;


CxMutex * fn_getConsoleInsLock()
{
    static CxMutex consoleInsLock;
    return & consoleInsLock;
}

vector<fn_interinfo_in_line_t> * fn_getConsoleInCallbacks_line()
{
    static vector<fn_interinfo_in_line_t> m;
    return & m;
}

vector<fn_interinfo_in_cmd_t> * fn_getConsoleInCallbacks_cmd()
{
    static vector<fn_interinfo_in_cmd_t> m;
    return & m;
}

vector<CxInterinfoIn_I*> * fn_getConsoleInSubjects()
{
    static vector<CxInterinfoIn_I*> s_consoleInSubjects;
    return & s_consoleInSubjects;
}

CxMutex * fn_getConsoleInputLock()
{
    static CxMutex consoleInputLock;
    return & consoleInputLock;
}

queue<pair<string, msepoch_t> > * fn_getConsoleInputs()
{
    static queue<pair<string, msepoch_t> > consoleInputs;
    return & consoleInputs;
}

void CxConsoleInterinfo::addObserver(CxInterinfoIn_I *oSubject)
{
    CxMutex * oLock = fn_getConsoleInsLock();
    vector<CxInterinfoIn_I*> * oSubjects = fn_getConsoleInSubjects();
    CxMutexScope lock(oLock);
    if ( find(oSubjects->begin(), oSubjects->end(), oSubject) ==  oSubjects->end() )
    {
        oSubjects->push_back(oSubject);
    }
}

void CxConsoleInterinfo::removeObserver(CxInterinfoIn_I *oSubject)
{
    CxMutex * oLock = fn_getConsoleInsLock();
    vector<CxInterinfoIn_I*> * oSubjects = fn_getConsoleInSubjects();
    CxMutexScope lock(oLock);
    CxContainer::remove(*oSubjects, oSubject);
}

void CxConsoleInterinfo::addObserver(fn_interinfo_in_line_t fn)
{
    CxMutex * oLock = fn_getConsoleInsLock();
    CxMutexScope lock(oLock);
    vector<fn_interinfo_in_line_t> * oCallbacks = fn_getConsoleInCallbacks_line();
    if (!CxContainer::contain(oCallbacks, fn))
    {
        oCallbacks->push_back(fn);
    }
}

void CxConsoleInterinfo::removeObserver(fn_interinfo_in_line_t fn)
{
    CxMutex * oLock = fn_getConsoleInsLock();
    CxMutexScope lock(oLock);
    vector<fn_interinfo_in_line_t> * oCallbacks = fn_getConsoleInCallbacks_line();
    CxContainer::remove(*oCallbacks, fn);
}

void CxConsoleInterinfo::addObserver(fn_interinfo_in_cmd_t fn)
{
    CxMutex * oLock = fn_getConsoleInsLock();
    CxMutexScope lock(oLock);
    vector<fn_interinfo_in_cmd_t> * oCallbacks = fn_getConsoleInCallbacks_cmd();
    if (!CxContainer::contain(oCallbacks, fn))
    {
        oCallbacks->push_back(fn);
    }
}

void CxConsoleInterinfo::removeObserver(fn_interinfo_in_cmd_t fn)
{
    CxMutex * oLock = fn_getConsoleInsLock();
    CxMutexScope lock(oLock);
    vector<fn_interinfo_in_cmd_t> * oCallbacks = fn_getConsoleInCallbacks_cmd();
    CxContainer::remove(*oCallbacks, fn);
}

void CxConsoleInterinfo::setFilterChannelInfo(bool value)
{
    if (value)
        f_bConsoleFilter |= 0x04000000;
    else
        f_bConsoleFilter &= (~0x04000000);
}

void CxConsoleInterinfo::updateInputString(const string &sLine)
{
    CxApplication::pushProcessCallBack(processInputString, 0, 0, sLine.data(), sLine.size(), 0, 0);
    CxApplication::signalMainThread();
}

static fn_interinfo_in_line_t f_fnInterinfoIn_line = 0;
static fn_interinfo_in_cmd_t f_fnInterinfoIn_cmd = 0;
static CxInterinfoIn_I * f_oInterinfoIn = NULL;

void CxConsoleInterinfo::processInputString(int iEvent, int iTag, const void * pData, int iLength, void * oSource, void * oTarget)
{
    string sLine((const char *)pData, iLength);
    string sCmdParams((const char *)pData, iLength);
    string sCmd = CxString::token(sCmdParams, ' ');
    map<string, string> sParams = CxString::splitToMap_mix(sCmdParams, '=', ' ');
    msepoch_t dtNow = CxTime::currentSystemTime();
    if (f_fnInterinfoIn_line)
    {
        if ( f_fnInterinfoIn_line(sLine, dtNow, 0, 0, 0) != CxInterinfo::InDeal_Lock)
        {
            f_fnInterinfoIn_line = 0;
        }
        return;
    }
    if (f_fnInterinfoIn_cmd)
    {
        if ( f_fnInterinfoIn_cmd(sCmd, sParams, dtNow, 0, 0, 0) != CxInterinfo::InDeal_Lock)
        {
            f_fnInterinfoIn_cmd = 0;
        }
        return;
    }
    if (f_oInterinfoIn)
    {
        int iLine = f_oInterinfoIn->interinfo_in_line(sLine, 0);
        int iCmd = 0;
        if (iLine <= 0)
        {
            iCmd = f_oInterinfoIn->interinfo_in_cmd(sCmd, sParams, dtNow, 0, 0, 0);
        }
        if ( iLine != CxInterinfo::InDeal_Lock && iCmd != CxInterinfo::InDeal_Lock )
        {
            f_fnInterinfoIn_cmd = 0;
        }
        return;
    }

    {
        CxMutexScope lock(fn_getConsoleInsLock());
        bool bDealEd = false;
        for (vector<CxInterinfoIn_I*>::iterator it = fn_getConsoleInSubjects()->begin(); it != fn_getConsoleInSubjects()->end(); ++it)
        {
            CxInterinfoIn_I * oInterinfoIn = (* it);
            int iDeal = oInterinfoIn->interinfo_in_line(sLine, dtNow, 0, 0, 0);
            if ( iDeal > 0 )
            {
                if (iDeal == CxInterinfo::InDeal_Lock)
                {
                    f_oInterinfoIn = oInterinfoIn;
                }
                bDealEd = true;
                break;
            }
        }
        if (! bDealEd)
        {
            for (vector<fn_interinfo_in_line_t>::iterator it = fn_getConsoleInCallbacks_line()->begin(); it != fn_getConsoleInCallbacks_line()->end(); ++it)
            {
                fn_interinfo_in_line_t fn = (* it);
                int iDeal = fn(sLine, dtNow, 0, 0, 0);
                if ( iDeal > 0 )
                {
                    if (iDeal == CxInterinfo::InDeal_Lock)
                    {
                        f_fnInterinfoIn_line = fn;
                    }
                    bDealEd = true;
                    break;
                }
            }
        }
        if (! bDealEd)
        {
            for (vector<fn_interinfo_in_cmd_t>::iterator it = fn_getConsoleInCallbacks_cmd()->begin(); it != fn_getConsoleInCallbacks_cmd()->end(); ++it)
            {
                fn_interinfo_in_cmd_t fn = (* it);
                int iDeal = fn(sCmd, sParams, dtNow, 0, 0, 0);
                if ( iDeal > 0 )
                {
                    if (iDeal == CxInterinfo::InDeal_Lock)
                    {
                        f_fnInterinfoIn_cmd = fn;
                    }
                    bDealEd = true;
                    break;
                }
            }
        }
    }
}

void CxConsoleInterinfo::interinfo_out(const string &sInfo, const std::string& sTitle, int type, int reason, int source, int target, int tag)
{
    if ((f_bConsoleFilter & type) == 0)
    {
        if (reason < 6)
        {
            stringstream sOutInfo;
            if (sTitle.size()>0)
                sOutInfo << "[title]=" << sTitle;
            if (type)
                sOutInfo << "[type]=" << type;
            if (reason)
                sOutInfo << "[reason]=" << reason;
            if (source)
                sOutInfo << "[source]=" << source;
            if (target)
                sOutInfo << "[target]=" << target;
            if (tag)
                sOutInfo << "[tag]=" << tag;
            if (sOutInfo.tellp() > 0)
                sOutInfo << "\n";
            sOutInfo << sInfo;
            ::cout << sOutInfo.str();
        }
    }
}


//LogDisables=type,source,reason;type
//LogDisables=0x01000001
void CxConsoleInterinfo::start()
{
    //*log out disable
    string sLogDisables; //type,
    sLogDisables = CxString::trim(CxApplication::findConfig( CS_SectionProgramConfig , "LogDisables", std::string()));
    if ((sLogDisables.size() == 3) && (CxString::toLower(sLogDisables) == "all"))
    {
        CxInterinfoOut::diableAll(CxInterinfo::Platform_Log);
    }
    else if (sLogDisables.size()>0)
    {
        vector<string> logDisables = CxString::split(sLogDisables, ';');
        for (size_t i = 0; i < logDisables.size(); ++i)
        {
            string sLine = logDisables.at(i);
            string sType = CxString::token(sLine, ',');
            string sSource = CxString::token(sLine, ',');
            string sReason = sLine;
            bool bOk;
            int iType = CxString::toInt32(sType, &bOk);
            if (bOk)
            {
                int iSource = CxString::toInt32(sSource, &bOk);
                if (bOk)
                {
                    int iReason = CxString::toInt32(sReason, &bOk);
                    if (bOk)
                    {
                        CxInterinfoOut::disableType(iType, iSource, iReason, CxInterinfo::Platform_Log);
                    }
                    else
                    {
                        CxInterinfoOut::disableType(iType, iSource, CxInterinfo::Platform_Log);
                    }
                }
                else
                {
                    CxInterinfoOut::disableType(iType, CxInterinfo::Platform_Log);
                }
            }
        }
    }
    if (CxApplication::projectType() == GM_PROJECT_TYPE_APP_CONSOLE)
    {
        CxInterinfoOut::addObserver(s_consoleInterinfo);
        f_threadInput.start();
    }
}

void CxConsoleInterinfo::stop()
{
    CxInterinfoOut::removeObserver(s_consoleInterinfo);
    f_iThreadInputStatus = 0;
}

bool fn_waitInputBoolean()
{
    bool r;
    ::cin >> r; ::getchar();
    return r;
}

int fn_waitInputInteger()
{
    int r;
    ::cin >> r; ::getchar();
    return r;
}

double fn_waitInputDouble()
{
    double r;
    ::cin >> r; ::getchar();
    return r;
}

string fn_waitInputString()
{
    string r;
    ::cin >> r; ::getchar();
    return r;
//    char str[1024];
//    memset(str, 0, 1024);
//    gets(str);
//    s = string(str);
}

tm fn_waitInputTm()
{
    string rs;
    ::cin >> rs; ::getchar();
    tm r;
    CxTime::fromString(rs, r);
    return r;
}

//void fn_temp_cxinterinfo(CxEndFlag & aEnd, CxLineFlag & aLine, CxLineEndFlag & aEndLine)
//{
//    if ( & aEnd == & cxEnd || & aLine == & cxLine || & aEndLine == & cxEndLine )
//    {
//        ::cout << "true" << ::endl;
//    }
//}

//type 下的 source
//source 下的 int reason, int target, int iTag
int CxInterinfoFilter::isEnable(int type, int source, int reason)
{
    std::map<int , std::map<int, std::vector<int> > >::const_iterator itType = m_types.find(type);
    if (itType != m_types.end())
    {
        const std::map<int, std::vector<int> > & sourceReasons = itType->second;
        //控制会细到 source
        if (sourceReasons.size() > 0)
        {
            std::map<int, std::vector<int> >::const_iterator itSource = sourceReasons.find(source);
            if (itSource != sourceReasons.end())
            {
                const std::vector<int> & iReasons = itSource->second;
                //控制会细到 reason
                if (iReasons.size() > 0)
                {
                    if (std::find(iReasons.begin(), iReasons.end(), reason) != iReasons.end())
                    {
                        //找到要控制的 reason
                        return FALSE;
                    }
                    else
                    {
                        //没有要控制的 reason
                        return TRUE;
                    }
                }
                else
                {
                    //即要控制全部 reason
                    return FALSE;
                }
            }
            else
            {
                //没有要控制的 source
                return TRUE;
            }
        }
        else
        {
            //即要控制全部 source
            return FALSE;
        }
    }
    else
    {
        //没有要控制的 type
        return TRUE;
    }
}

void CxInterinfoFilter::enableAll()
{
    m_types.clear();
    m_isReverse = false;
}

void CxInterinfoFilter::disableAll()
{
    m_types.clear();
    m_isReverse = true;
}

void CxInterinfoFilter::enableType(int type)
{
    CxContainer::remove(m_types, type);
}

void CxInterinfoFilter::disableType(int type)
{
//    std::vector<int> iReasons;
//    std::map<int, std::vector<int> > sourceReason;
    m_types[type] = std::map<int, std::vector<int> >();
}

void CxInterinfoFilter::enableType(int type, int source)
{
    std::map<int , std::map<int, std::vector<int> > >::iterator itType = m_types.find(type);
    if (itType != m_types.end())
    {
        std::map<int, std::vector<int> > & sourceReasons = itType->second;
        CxContainer::remove(sourceReasons, source);
    }
}

void CxInterinfoFilter::disableType(int type, int source)
{
    std::map<int , std::map<int, std::vector<int> > >::iterator itType = m_types.find(type);
    if (itType != m_types.end())
    {
        std::map<int, std::vector<int> > & sourceReasons = itType->second;
        sourceReasons[source] = std::vector<int>();
    }
    else
    {
        std::map<int, std::vector<int> > sourceReason;
        sourceReason[source] = std::vector<int>();
        m_types[type] = sourceReason;
    }
}

void CxInterinfoFilter::enableType(int type, int source, int reason)
{
    std::map<int , std::map<int, std::vector<int> > >::iterator itType = m_types.find(type);
    if (itType != m_types.end())
    {
        std::map<int, std::vector<int> > & sourceReasons = itType->second;
        std::map<int, std::vector<int> >::iterator itSource = sourceReasons.find(source);
        if (itSource != sourceReasons.end())
        {
            std::vector<int> & iReasons = itSource->second;
            CxContainer::remove(iReasons, reason);
        }
    }
}

void CxInterinfoFilter::disableType(int type, int source, int reason)
{
    std::map<int , std::map<int, std::vector<int> > >::iterator itType = m_types.find(type);
    if (itType != m_types.end())
    {
        std::map<int, std::vector<int> > & sourceReasons = itType->second;
        std::map<int, std::vector<int> >::iterator itSource = sourceReasons.find(source);
        if (itSource != sourceReasons.end())
        {
            std::vector<int> & iReasons = itSource->second;
            iReasons.push_back(reason);
        }
        else
        {
            std::vector<int> iReasons;
            iReasons.push_back(reason);
            sourceReasons[source] = iReasons;
        }
    }
    else
    {
        std::vector<int> iReasons;
        iReasons.push_back(reason);
        std::map<int, std::vector<int> > sourceReasons;
        sourceReasons[source] = iReasons;
        m_types[type] = sourceReasons;
    }
}


