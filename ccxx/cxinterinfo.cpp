#include "cxinterinfo.h"

#include "cxcontainer.h"
#include "cxtime.h"
#include "cxthread.h"

#ifdef WIN32
#  include <io.h>
#  include <fcntl.h>
#  include <stdio.h>
#endif


using namespace std;

const CxEndFlag cxEnd = CxEndFlag();
const CxLineFlag cxLine = CxLineFlag();
const CxLineEndFlag cxEndLine = CxLineEndFlag();

static string * f_pConsoleInputLine = NULL;
static string * f_pConsoleInputCommand = NULL;
static std::map<std::string, std::string> * f_pConsoleInputParams = NULL;
static msepoch_t * f_pConsoleInputDtIn = NULL;
static int  * f_pConsoleInputSource = NULL;
static int * f_pConsoleInputInType = NULL;
static int * f_pConsoleInputTag = NULL;

static fn_interinfo_in_line_t f_fnInterinfoIn_line = 0;
static fn_interinfo_in_cmd_t f_fnInterinfoIn_cmd = 0;
static CxInterinfoIn_I * f_oInterinfoIn = NULL;

CxMutex * fn_pConsoleInputSingleThreadLock()
{
    static CxMutex m;
    return & m;
}

CxConsoleInterinfo * fn_oConsoleInterinfoSingleton()
{
    static CxConsoleInterinfo m;
    return & m;
}


///*InterinfoOutSubject*/
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

static fn_void_queue_msg_tlv_t f_fnITCPostDefault = 0;
static fn_void_t f_fnITCSignalDefault = 0;

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


class CxConsoleInputThread : public CxDetachedThread
{
public:
    enum StartConditionEnum {
        StartCondition_None = 0,
        StartCondition_InfoStart = 0x00000001,
        StartCondition_Observer = 0x00000002
    };

    static void startConsoleInputThread(StartConditionEnum eStartCondition);

    static void stopConsoleInputThread();

public:
    CxConsoleInputThread()
    {
        _isStarted = false;
//        ios_base::sync_with_stdio(false);
//        std::cin.tie(NULL);
//        ungetc(ch,stdin);
    }
    ~CxConsoleInputThread()
    {
    }

protected:
    void run()
    {
        std::string sLine;
        while ( _isStarted )
        {
//            std::cin >> sLine;
            std::getline (std::cin, sLine);

            if (_isStarted && std::cin.good())
            {
                std::cout << "CxConsoleInputThread input" << sLine << endl;
                CxConsoleInterinfo::updateConsoleInputString(sLine);
            }
            else
            {
                _isStarted = false;
                std::cout << "CxConsoleInputThread end." << endl;
            }
        }
    }

private:
    volatile bool _isStarted;

    friend class CxInterinfo;
	friend class CxInterinfoIn;

};

int * fn_oConsoleInputThreadStartCondition()
{
    static int m = 0;
    return &m;
}

CxConsoleInputThread * fn_oConsoleInputThreadInit()
{
    static CxConsoleInputThread m;
    return &m;
}

void CxConsoleInputThread::startConsoleInputThread(CxConsoleInputThread::StartConditionEnum eStartCondition)
{
    int * pStartCondition = fn_oConsoleInputThreadStartCondition();
    * pStartCondition |= eStartCondition;
    if (* pStartCondition < ( StartCondition_InfoStart | StartCondition_Observer)) return;

//    if (!(fn_getConsoleInSubjects()->size()>0 && fn_getConsoleInCallbacks_line()->size()>0 && fn_getConsoleInCallbacks_cmd()->size()>0))
//    {
//        return;
//    }
    CxConsoleInputThread * oConsoleInputThread = fn_oConsoleInputThreadInit();
    if (! oConsoleInputThread->_isStarted){}
    {
#ifdef WIN32
#  ifdef __BORLANDC__
        setmode(_fileno(stdin), O_BINARY);
#  else
        _setmode(_fileno(stdin), _O_BINARY);
#  endif
#endif
        oConsoleInputThread->_isStarted = true;
        oConsoleInputThread->start();
        cxPrompt() << "CxInterinfo::startConsoleInputThread";
    }
}

void CxConsoleInputThread::stopConsoleInputThread()
{
    CxConsoleInputThread * oConsoleInputThread = fn_oConsoleInputThreadInit();
    oConsoleInputThread->_isStarted = false;
    std::cin.setstate(std::cin.eofbit);
    std::cout << "CxInterinfo::stopConsoleInputThread." << std::endl;
    {
//		ungetc('\r', stdin);

//        cin.exceptions(std::ifstream::eofbit);

//        std::cin.setstate(std::cin.eofbit);
//        feof(stdin);
//        fclose(stdin);

//		char mybuffer[32768];
//		setvbuf(stdin, mybuffer, _IOFBF, sizeof(mybuffer));
//
//		setvbuf(stdin, NULL, _IOFBF, 16384);
//		setvbuf(stdin, "0", _IOEOF, 1);
//		setvbuf(stdin, mybuffer, _IOEOF, sizeof(mybuffer));

//		if (isatty(fileno(stdin)))
//			printf("stdin is a terminal\n");
//		else
//			printf("stdin is a file or a pipe\n");

//        std::cin.setstate(std::cin.eofbit);
//        close(fileno(stdin));

//        std::cin.fill(_IOEOF);
//        std::cin.fill(0);

//        std::ios::sync_with_stdio(false);
//        std::cin.tie(0);

//        std::ostream *prevstr;
//        std::ofstream ofs;
//        ofs.open("test.txt");
//        std::cout << "tie example:\n";
//        *std::cin.tie() << "This is inserted into cout\n";
//        prevstr = std::cin.tie(&ofs);
//        *std::cin.tie() << "This is inserted into the file\n";
//        std::cin.tie(prevstr);
//        ofs.close();

//        fclose(stdin);
//        CxThread::sleep(100);

//        *std::cin.tie()<< 0 << "This is inserted into cout\r\n";
//        std::cout << "CxInterinfo::stopInterInfo." << std::endl;
    }
}

void CxInterinfo::startInterInfo(fn_void_queue_msg_tlv_t fnITCPostDefault, fn_void_t fnITCSignalDefault, bool bRunInputThread)
{
    f_fnITCPostDefault = fnITCPostDefault;
    f_fnITCSignalDefault = fnITCSignalDefault;

    CxInterinfoOut::addObserver(fn_oConsoleInterinfoSingleton());
    cxPrompt() << "CxInterinfo::startInterInfo";
    if (bRunInputThread)
    {
        CxConsoleInputThread::startConsoleInputThread(CxConsoleInputThread::StartCondition_InfoStart);
    }
}

void CxInterinfo::stopInterInfo()
{
    CxConsoleInputThread::stopConsoleInputThread();
    CxInterinfoOut::removeObserver(fn_oConsoleInterinfoSingleton());
    std::cout << "CxInterinfo::stopInterInfo." << std::endl;
}


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
    else if (sPlatform == "tty")
        ePlatform = CxInterinfo::Platform_Tty;

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
    case Platform_Tty:      return "tty"; break;
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

CxInterinfo::LevelEnum CxInterinfo::levelFrom(const string &sLevel)
{
    CxInterinfo::LevelEnum eLevel = CxInterinfo::LevelNone;
    if (sLevel ==      "fatal")         eLevel = CxInterinfo::LevelFatal;
    else if (sLevel == "error")         eLevel = CxInterinfo::LevelError;
    else if (sLevel == "warn")          eLevel = CxInterinfo::LevelWarn ;
    else if (sLevel == "info")          eLevel = CxInterinfo::LevelInfo ;
    else if (sLevel == "debug")         eLevel = CxInterinfo::LevelDebug;
    return eLevel;
}

string CxInterinfo::levelToString(CxInterinfo::LevelEnum e)
{
    switch (e)
    {
    case LevelFatal    :  return "fatal"    ; break;
    case LevelError    :  return "error"    ; break;
    case LevelWarn     :  return "warn"     ; break;
    case LevelInfo     :  return "info"     ; break;
    case LevelDebug    :  return "debug"    ; break;
    default :             return "none"     ; break;
    }
}

string CxInterinfo::levelUnmberToString(int eNum)
{
    switch (eNum & 0x00FF0000)
    {
    case LevelFatal    :  return "fatal"    ; break;
    case LevelError    :  return "error"    ; break;
    case LevelWarn     :  return "warn"     ; break;
    case LevelInfo     :  return "info"     ; break;
    case LevelDebug    :  return "debug"    ; break;
    default :             return "none"     ; break;
    }
}







void CxInterinfoOut::addObserver(CxInterinfoOut_I *oSubject)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*> * oSubjects = fn_getInterinfoOutSubjects();
    if ( find(oSubjects->begin(), oSubjects->end(), oSubject) ==  oSubjects->end() )
    {
        oSubjects->push_back(oSubject);
    }
}

void CxInterinfoOut::removeObserver(CxInterinfoOut_I *oSubject)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*> * oSubjects = fn_getInterinfoOutSubjects();
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
        if (oInterinfoOut->platformValue() == CxInterinfo::Platform_Log)
        {
            if (reason < CxInterinfo::LevelFatal)
            {
                bool bIsEnable = oInterinfoOut->filter()->isEnable(type, source, reason);
                if (oInterinfoOut->filter()->getIsReverse())
                    bIsEnable = ! bIsEnable;
                if (bIsEnable)
                {
                    oInterinfoOut->interinfo_out(sInfo, sTitle, type, reason, source, target, iTag);
                }
            }
            else
            {
                oInterinfoOut->interinfo_out(sInfo, sTitle, type, reason, source, target, iTag);
            }
        }
        else
        {
            bool bIsEnable = oInterinfoOut->filter()->isEnable(type, source, reason);
            if (oInterinfoOut->filter()->getIsReverse())
                bIsEnable = ! bIsEnable;
            if (bIsEnable)
            {
                oInterinfoOut->interinfo_out(sInfo, sTitle, type, reason, source, target, iTag);
            }
        }
    }
}

void CxInterinfoOut::outLog(const string &sInfo, const string &sTitle, int type, int reason, int source, int target, int iTag)
{
    CxMutexScope lock(fn_getInterinfoOutSubjectsLock());
    vector<CxInterinfoOut_I*>::iterator itBegin = fn_getInterinfoOutSubjects()->begin();
    vector<CxInterinfoOut_I*>::iterator itEnd = fn_getInterinfoOutSubjects()->end();
    for (vector<CxInterinfoOut_I*>::iterator it = itBegin; it != itEnd; ++it)
    {
        CxInterinfoOut_I * oInterinfoOut = (* it);
        if (oInterinfoOut->platformValue() == CxInterinfo::Platform_Log)
        {
            bool bIsEnable = oInterinfoOut->filter()->isEnable(type, source, reason);
            if (oInterinfoOut->filter()->getIsReverse())
                bIsEnable = ! bIsEnable;
            if (bIsEnable)
            {
                oInterinfoOut->interinfo_out(sInfo, sTitle, type, reason, source, target, iTag);
            }
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

void CxInterinfoIn::addObserver(CxInterinfoIn_I *oSubject)
{
    CxMutexScope lock(fn_getConsoleInsLock());
    vector<CxInterinfoIn_I*> * oSubjects = fn_getConsoleInSubjects();
    if ( find(oSubjects->begin(), oSubjects->end(), oSubject) ==  oSubjects->end() )
    {
        oSubjects->push_back(oSubject);
    }
}

void CxInterinfoIn::removeObserver(CxInterinfoIn_I *oSubject)
{
    CxMutexScope lock(fn_getConsoleInsLock());
    vector<CxInterinfoIn_I*> * oSubjects = fn_getConsoleInSubjects();
    CxContainer::remove(*oSubjects, oSubject);
}

void CxInterinfoIn::addObserver(fn_interinfo_in_line_t fn)
{
    CxMutexScope lock(fn_getConsoleInsLock());
    vector<fn_interinfo_in_line_t> * oCallbacks = fn_getConsoleInCallbacks_line();
    if (!CxContainer::contain(oCallbacks, fn))
    {
        oCallbacks->push_back(fn);
    }
}

void CxInterinfoIn::removeObserver(fn_interinfo_in_line_t fn)
{
    CxMutexScope lock(fn_getConsoleInsLock());
    vector<fn_interinfo_in_line_t> * oCallbacks = fn_getConsoleInCallbacks_line();
    CxContainer::remove(*oCallbacks, fn);
}

void CxInterinfoIn::addObserver(fn_interinfo_in_cmd_t fn)
{
    CxMutexScope lock(fn_getConsoleInsLock());
    vector<fn_interinfo_in_cmd_t> * oCallbacks = fn_getConsoleInCallbacks_cmd();
    if (!CxContainer::contain(oCallbacks, fn))
    {
        oCallbacks->push_back(fn);
    }
}

void CxInterinfoIn::removeObserver(fn_interinfo_in_cmd_t fn)
{
    CxMutexScope lock(fn_getConsoleInsLock());
    vector<fn_interinfo_in_cmd_t> * oCallbacks = fn_getConsoleInCallbacks_cmd();
    CxContainer::remove(*oCallbacks, fn);
}

string CxConsoleInterinfo::waitInputLine(fn_void_t fnProcessEvents, int iTimeOut, msepoch_t *pDtIn, int *pSource, int *pInType, int *pTag)
{
    CxMutexScope lock(fn_pConsoleInputSingleThreadLock());
    string sLine;
    f_pConsoleInputLine = & sLine;
    f_pConsoleInputCommand = NULL;
    f_pConsoleInputParams = NULL;
    f_pConsoleInputDtIn = pDtIn;
    f_pConsoleInputSource = pSource;
    f_pConsoleInputInType = pInType;
    f_pConsoleInputTag = pTag;
    try
    {
        msepoch_t dtNow = CxTime::currentSystemTime();
        while (1)
        {
            if (sLine.size()>0)
            {
                break;
            }
            if (iTimeOut > 0 && CxTime::milliSecondDifferToNow(dtNow) > iTimeOut)
            {
                break;
            }
            if (fnProcessEvents)
            {
                fnProcessEvents();
            }
            CxThread::sleep(10);
        }
    }
    catch (...)
    {
        f_pConsoleInputLine = NULL;
        f_pConsoleInputCommand = NULL;
        f_pConsoleInputParams = NULL;
        f_pConsoleInputDtIn = NULL;
        f_pConsoleInputSource = NULL;
        f_pConsoleInputInType = NULL;
        f_pConsoleInputTag = NULL;
    }
    f_pConsoleInputLine = NULL;
    f_pConsoleInputCommand = NULL;
    f_pConsoleInputParams = NULL;
    f_pConsoleInputDtIn = NULL;
    f_pConsoleInputSource = NULL;
    f_pConsoleInputInType = NULL;
    f_pConsoleInputTag = NULL;

    return sLine;
}

string CxConsoleInterinfo::waitInputCmd(fn_void_t fnProcessEvents, int iTimeOut, std::map<string, string> *oParams, msepoch_t *pDtIn, int *pSource, int *pInType, int *pTag)
{
    CxMutexScope lock(fn_pConsoleInputSingleThreadLock());
    string sCmd;
    f_pConsoleInputLine = NULL;
    f_pConsoleInputCommand = & sCmd;
    f_pConsoleInputParams = oParams;
    f_pConsoleInputDtIn = pDtIn;
    f_pConsoleInputSource = pSource;
    f_pConsoleInputInType = pInType;
    f_pConsoleInputTag = pTag;
    try
    {

        msepoch_t dtNow = CxTime::currentSystemTime();
        while (1)
        {
            if (sCmd.size()>0)
            {
                break;
            }
            if (iTimeOut > 0 && CxTime::milliSecondDifferToNow(dtNow) > iTimeOut)
            {
                break;
            }
            if (fnProcessEvents)
            {
                fnProcessEvents();
            }
            CxThread::sleep(10);
        }
    }
    catch (...)
    {
        f_pConsoleInputLine = NULL;
        f_pConsoleInputCommand = NULL;
        f_pConsoleInputParams = NULL;
        f_pConsoleInputDtIn = NULL;
        f_pConsoleInputSource = NULL;
        f_pConsoleInputInType = NULL;
        f_pConsoleInputTag = NULL;
    }
    f_pConsoleInputLine = NULL;
    f_pConsoleInputCommand = NULL;
    f_pConsoleInputParams = NULL;
    f_pConsoleInputDtIn = NULL;
    f_pConsoleInputSource = NULL;
    f_pConsoleInputInType = NULL;
    f_pConsoleInputTag = NULL;

    return sCmd;
}

void CxConsoleInterinfo::updateConsoleInputString(const string &sLine)
{
    if (f_fnITCPostDefault && f_fnITCSignalDefault)
    {
        f_fnITCPostDefault(processInputString, 0, 0, sLine.data(), sLine.size(), 0, 0, false);
        f_fnITCSignalDefault();
    }
    else
    {
        //needtodo
    }
}

//*** fn_interinfo_in_line_t fn_interinfo_in_cmd_t CxInterinfoIn_I ***
//*** fn_interinfo_in_line_t fn_interinfo_in_cmd_t CxInterinfoIn_I ***
//*** fn_interinfo_in_line_t fn_interinfo_in_cmd_t CxInterinfoIn_I ***
void CxConsoleInterinfo::processInputString(int iEvent, int iTag, const void * pData, int iLength, void * oSource, void * oTarget)
{
    string sLine((const char *)pData, iLength);
    size_t i1 = sLine.find_first_of("\r\n");
    if (i1 != string::npos) sLine = sLine.substr(0, i1);
    string sCmdParams((const char *)pData, iLength);
    string sCmd;
    map<string, string> sParams;
    if (sCmdParams.find(' ') != string::npos)
    {
        sCmd = CxString::token(sCmdParams, ' ');
        sParams = CxString::splitToMap_mix(sCmdParams, '=', ' ');
    }
    else
    {
        sCmd = sLine;
    }
    msepoch_t dtNow = CxTime::currentSystemTime();

    if (! processInputLine(sLine, dtNow, 0, 0, 0) )
    {
        processInputCmd(sCmd, sParams, dtNow, 0, 0, 0);
    }

    //*{lock input deal; InDeal_Lock
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
    //*}lock input deal

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

int CxConsoleInterinfo::processInputLine(const string &sInfo, const msepoch_t &dtIn, int iSource, int eInType, int iTag)
{
    if (f_pConsoleInputLine)
    {
        * f_pConsoleInputLine = CxString::newString(sInfo);
    }
    else
    {
        return FALSE;
    }
    if (f_pConsoleInputDtIn)
    {
        * f_pConsoleInputDtIn = dtIn;
    }
    if (f_pConsoleInputSource)
    {
        * f_pConsoleInputSource = iSource;
    }
    if (f_pConsoleInputInType)
    {
        * f_pConsoleInputInType = eInType;
    }
    if (f_pConsoleInputTag)
    {
        * f_pConsoleInputTag = iTag;
    }
    return TRUE;
}

int CxConsoleInterinfo::processInputCmd(const string &sCommand, const std::map<string, string> &sParams, const msepoch_t &dtIn, int iSource, int eInType, int iTag)
{
    if (f_pConsoleInputCommand)
    {
        * f_pConsoleInputCommand = CxString::newString(sCommand);
    }
    else
    {
        return FALSE;
    }
    if (f_pConsoleInputParams)
    {
        * f_pConsoleInputParams = CxString::newStrings(sParams);
    }
    if (f_pConsoleInputDtIn)
    {
        * f_pConsoleInputDtIn = dtIn;
    }
    if (f_pConsoleInputSource)
    {
        * f_pConsoleInputSource = iSource;
    }
    if (f_pConsoleInputInType)
    {
        * f_pConsoleInputInType = eInType;
    }
    if (f_pConsoleInputTag)
    {
        * f_pConsoleInputTag = iTag;
    }
    return TRUE;
}

void CxConsoleInterinfo::interinfo_out(const string &sInfo, const std::string& sTitle, int type, int reason, int source, int target, int tag)
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





//type's source
//source's int reason, int target, int iTag
int CxInterinfoFilter::isEnable(int type, int source, int reason)
{
    std::map<int , std::map<int, std::vector<int> > >::const_iterator itType = m_types.find(type);
    if (itType != m_types.end())
    {
        const std::map<int, std::vector<int> > & sourceReasons = itType->second;
        // source
        if (sourceReasons.size() > 0)
        {
            std::map<int, std::vector<int> >::const_iterator itSource = sourceReasons.find(source);
            if (itSource != sourceReasons.end())
            {
                const std::vector<int> & iReasons = itSource->second;
                // reason
                if (iReasons.size() > 0)
                {
                    if (std::find(iReasons.begin(), iReasons.end(), reason) != iReasons.end())
                    {
                        // found reason
                        return FALSE;
                    }
                    else
                    {
                        // no found reason
                        return TRUE;
                    }
                }
                else
                {
                    // all reason
                    return FALSE;
                }
            }
            else
            {
                // no source
                return TRUE;
            }
        }
        else
        {
            // all source
            return FALSE;
        }
    }
    else
    {
        // no type
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
