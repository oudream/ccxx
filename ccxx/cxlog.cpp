#include "cxlog.h"

#include "cxstring.h"
#include "cxtime.h"
#include "cxfile.h"
#include "cxappenv.h"

using namespace std;

const string CS_KEY_DIR_LOG = "LogPath";

CxLogRealtime *fn_oLogRealtimeSingleton()
{
    static CxLogRealtime m;
    return &m;
}

CxLogThread *fn_oLogThreadSingleton()
{
    static CxLogThread m;
    return &m;
}

static string f_logDir;
static string f_currentLogDir;
static msepoch_t f_logDirUpdateDatetime = CxTime::currentSystemTime();
static msepoch_t f_logDirNextDatetime = CxTime::currentDayEnd() + 1000;
static const CxLogFileTactics_I *f_logFileTactics = NULL;
static map<string, FILE *> f_logFiles;

string
fn_getLogString(const std::string &sInfo, int type = 0, int reason = 0, int source = 0, int target = 0, int tag = 0)
{
    stringstream sOutInfo;
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
        sOutInfo << CxGlobal::lineString;
    sOutInfo << sInfo;
    return sOutInfo.str();
}

string
fn_getLogString(const std::string &sInfo, const std::string &sTitle, int type = 0, int reason = 0, int source = 0, int target = 0, int tag = 0)
{
    stringstream sOutInfo;
    if (sTitle.size() > 0)
        sOutInfo << "[title]=" << sTitle;
    if (type)
        sOutInfo << "[type]=" << type;
    if (reason)
    {
        if (reason > CxInterinfo::LevelFatal)
        {
            sOutInfo << "[level]=" << CxInterinfo::levelUnmberToString(reason);
            sOutInfo << "[reason]=" << (reason & 0x0000FFFF);
        }
        else
        {
            sOutInfo << "[reason]=" << reason;
        }
    }
    if (source)
        sOutInfo << "[source]=" << source;
    if (target)
        sOutInfo << "[target]=" << target;
    if (tag)
        sOutInfo << "[tag]=" << tag;
    if (sOutInfo.tellp() > 0)
        sOutInfo << CxGlobal::lineString;
    sOutInfo << sInfo;
    return sOutInfo.str();
}

string fn_logFilePathByNow()
{
    tm dt = CxTime::localTm();
    string logFileName = CxString::toString(dt.tm_mday) + "-" + CxString::toString(dt.tm_hour) + ".log";
    //default = log
    string sLogPath = CxFileSystem::fullFilePath(CxAppEnv::applicationDeployPath(), CxAppEnv::findConfig(CS_SectionProgramConfig, "LogPath", std::string()));
    if (sLogPath.empty())
    {
        sLogPath = CxFileSystem::mergeFilePath(CxAppEnv::applicationDeployPath(), "log");
    }
    return CxFileSystem::mergeFilePath(sLogPath, logFileName);
}

void fn_deleteLogFile(const string &sFilePath)
{
    FILE *oFile = NULL;
    string sFileName = CxFileSystem::extractFileName(sFilePath);
    map<string, FILE *>::iterator it = f_logFiles.find(sFileName);
    if (it != f_logFiles.end())
    {
        oFile = it->second;
    }
    if (oFile != NULL)
    {
        fclose(oFile);
        CxFileSystem::deleteFile(sFilePath);
        oFile = fopen(sFilePath.data(), "ab+");
        if (oFile != NULL)
        {
            f_logFiles[sFileName] = oFile;
        }
    }
    else
    {
        CxFileSystem::deleteFile(sFilePath);
    }
}

void fn_clearLogFiles()
{
#define GM_LOG_MAX_FILESIZE (1024LL * 1024 * 1024 * 4)
#define GM_LOG_MAX_SAVETIME (1000LL * 60 * 60 * 24 * 31)
#define GM_LOG_MAX_SAVETIME2 (1000LL * 60 * 60 * 24 * 7)

    vector<CxFileSystem::PathInfo> pathInfos;
        CxFileSystem::scanDir(f_logDir, pathInfos);
    int64 r = 0;
    for (size_t i = 0; i < pathInfos.size(); ++i)
    {
        const CxFileSystem::PathInfo &pathInfo = pathInfos.at(i);
        //delete file
        if (pathInfo.fileSize > GM_LOG_MAX_FILESIZE)
        {
            fn_deleteLogFile(pathInfo.filePath());
            cxPrompt() << "log clear : pathInfo.fileSize > GM_LOG_MAX_FILESIZE : " << pathInfo.fileName;
            continue;
        }
        if (abs(CxTime::milliSecondDifferToNow(pathInfo.creationTime)) > GM_LOG_MAX_SAVETIME)
        {
            fn_deleteLogFile(pathInfo.filePath());
            cxPrompt()
                << "log clear : abs(CxTime::milliSecondDifferToNow(pathInfo.creationTime)) > GM_LOG_MAX_SAVETIME : "
                << pathInfo.fileName;
            continue;
        }
        r += pathInfo.fileSize;
    }
    if (r > GM_LOG_MAX_FILESIZE)
    {
        for (size_t i = 0; i < pathInfos.size(); ++i)
        {
            const CxFileSystem::PathInfo &pathInfo = pathInfos.at(i);
            //delete file
            if (abs(CxTime::milliSecondDifferToNow(pathInfo.creationTime)) > GM_LOG_MAX_SAVETIME2)
            {
                fn_deleteLogFile(pathInfo.filePath());
                cxPrompt()
                    << "log clear : abs(CxTime::milliSecondDifferToNow(pathInfo.creationTime)) > GM_LOG_MAX_SAVETIME2 : "
                    << pathInfo.fileName;
                continue;
            }
        }
    }
}

static int f_iLogType = 1;
void CxLogManager::startLog()
{
//#ifdef GM_LOG_REALTIME
    if (f_iLogType == 0)
    {
        CxLogRealtime * oLogRealtime = fn_oLogRealtimeSingleton();
        if (oLogRealtime)
        {
            CxInterinfoOut::addObserver(oLogRealtime);
        }
    }
    else
    {
        CxLogThread *oLogThread = fn_oLogThreadSingleton();
        if (!oLogThread->_isStarted)
        {
            CxInterinfoOut::addObserver(oLogThread);
            //order : after CxInterinfoOut::addObserver(oLogThread);
            loadLogConfig();

            oLogThread->_isStarted = true;
            oLogThread->start();
        }
    }

    cxLogInfo() << "CxLogManager::startLog.";
    cxLogInfo() << "CxAppEnv::configFilePath: " << CxAppEnv::configFilePath();
    cxLogInfo() << "CxAppEnv::logPath: " << CxAppEnv::logPath();
    cxLogInfo() << "CxAppEnv::tempPath: " << CxAppEnv::tempPath();
}

void CxLogManager::stopLog()
{
//#ifdef GM_LOG_REALTIME
    if (f_iLogType == 0)
    {
        CxLogRealtime * oLogRealtime = fn_oLogRealtimeSingleton();
        if (oLogRealtime)
        {
            CxInterinfoOut::removeObserver(oLogRealtime);
            std::cout << "CxLogManager::stopLog." << std::endl;
            cxLogInfo() << "CxLogManager::stopLog.";
        }
    }
    else
    {
        CxLogThread *oLogThread = fn_oLogThreadSingleton();
        if (oLogThread && oLogThread->_isStarted)
        {
            cxLogInfo() << "CxLogManager::stopLog.";
            std::cout << "CxLogManager::stopLog 1." << std::endl;
            CxInterinfoOut::removeObserver(oLogThread);
            std::cout << "CxLogManager::stopLog 2." << std::endl;
            oLogThread->stop();
            std::cout << "CxLogManager::stopLog 3." << std::endl;
            oLogThread->doSaveLog();
            std::cout << "CxLogManager::stopLog." << std::endl;
        }
    }

    for (map<string, FILE *>::iterator it = f_logFiles.begin(); it != f_logFiles.end(); ++it)
    {
        FILE *oFile = it->second;
        if (oFile)
        {
            fclose(oFile);
        }
    }
    f_logFiles.clear();
}

//LogDisables=type,source,reason;type
//LogDisables=0x01000001
void CxLogManager::loadLogConfig()
{
    //*log out disable
    string sLogDisables; //type,
    sLogDisables = CxString::trim(CxAppEnv::findConfig(CS_SectionProgramConfig, "LogDisables", std::string()));
    if ((sLogDisables.size() == 3) && (CxString::toLower(sLogDisables) == "all"))
    {
        CxInterinfoOut::diableAll(CxInterinfo::Platform_Log);
    }
    else if (sLogDisables.size() > 0)
    {
        vector<string> logDisables = CxString::split(sLogDisables, ';');
        for (size_t i = 0; i < logDisables.size(); ++i)
        {
            const string &sLogDisable = logDisables.at(i);
            string sLine(sLogDisable.c_str());
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
}

void CxLogManager::setLogFileTactics(const CxLogFileTactics_I *oLogFileTactics)
{
    f_logFileTactics = oLogFileTactics;
}

string CxLogManager::fileName(int type, int reason, int source, int target, int tag)
{
    string sFileName;
    if (f_logFileTactics)
    {
        sFileName = f_logFileTactics->log_fileName(type, reason, source, target, tag);
    }
    if (sFileName.empty())
    {
        sFileName = CxAppEnv::applicationTargetName() + ".log";
    }
    return sFileName;
}

FILE *CxLogManager::fileByFileName(const string &sFileName)
{
    msepoch_t dtNow = CxTime::currentSystemTime();
    if (dtNow > f_logDirNextDatetime)
    {
        // change log dir
        for (map<string, FILE *>::iterator it = f_logFiles.begin(); it != f_logFiles.end(); ++it)
        {
            FILE *oFile = it->second;
            if (oFile)
            {
                fclose(oFile);
            }
        }
        f_logFiles.clear();
        f_currentLogDir.clear();
        f_logDirNextDatetime = CxTime::currentDayEnd() + 1000;
    }
    map<string, FILE *>::iterator it = f_logFiles.find(sFileName);
    if (it != f_logFiles.end())
    {
        return it->second;
    }
    else
    {
        if (f_currentLogDir.empty())
        {
            if (f_logFileTactics)
            {
                f_logDir = f_logFileTactics->log_directory();
            }
            else
            {
                f_logDir = CxAppEnv::logPath();
            }
            f_currentLogDir = CxFileSystem::mergeFilePath(f_logDir, CxTime::currentDayString('-'));
            //if (! CxFileSystem::isExist(f_currentLogDir)) CxFileSystem::createDirMultiLevel(f_currentLogDir);
            f_logDirUpdateDatetime = CxTime::currentSystemTime();
        }
//        string sFilePath = CxFileSystem::mergeFilePath(f_currentLogDir, sFileName);
        string sFilePath = CxFileSystem::mergeFilePath(f_logDir,
                                                       CxTime::currentSepochString('-', 'd', '-') + "s-" + sFileName);
        FILE *oFile = fopen(sFilePath.data(), "ab+");
        if (oFile != NULL)
        {
            f_logFiles[sFileName] = oFile;
            return oFile;
        }
    }
    return NULL;
}

void
CxLogManager::outLog(const std::string &sInfo, const std::string &sTitle, int type, int reason, int source, int target, int tag)
{
//#ifdef GM_LOG_REALTIME
    if (f_iLogType == 0)
    {
        CxLogRealtime *oLogRealtime = fn_oLogRealtimeSingleton();
        if (oLogRealtime)
        {
            oLogRealtime->interinfo_out(sInfo, sTitle, type, reason, source, target, tag);
        }
    }
    else
    {
        CxLogThread *oLogThread = fn_oLogThreadSingleton();
        if (oLogThread)
        {
            oLogThread->interinfo_out(sInfo, sTitle, type, reason, source, target, tag);
        }
    }
}

CxLogRealtime::CxLogRealtime()
{
    _filePathRt = "";
    _fileRt = NULL;
    _fileOpenTimeRt = 0;
    _fileCountRt = 0;
    _fileSizeRt = 0;
}

CxLogRealtime::~CxLogRealtime()
{
}

void
CxLogRealtime::interinfo_out(const string &sInfo, const std::string &sTitle, int type, int reason, int source, int target, int tag)
{
    std::string sFileName = CxLogManager::fileName(type, reason, source, target, tag);
    FILE *oFile = CxLogManager::fileByFileName(sFileName);
    if (oFile)
    {
        std::string sText;
        if (f_logFileTactics && !f_logFileTactics->log_hasFileName(type, reason, source, target, tag))
            sText = fn_getLogString(sInfo, type, reason, source, target, tag);
        else
            sText = fn_getLogString(sInfo, sTitle, type, reason, source, target, tag);
        sText = CxTime::currentSystemTimeString() + " " + sText + "\r\n";
        size_t iWrote = fwrite(const_cast<char *>(sText.data()), 1, sText.size(), oFile);
//        fflush(_fileRt);
        _fileCountRt += sText.size();
        _fileSizeRt += iWrote;
    }
}

CxLogThread::CxLogThread()
{
    _isStarted = false;

    _logStringsPush = &_logStrings1;
    _logStringsPop = &_logStrings2;
    _logFilesPush = &_logFiles1;
    _logFilesPop = &_logFiles2;
    _logStringsSize = 0;

    _filePathTh = "";
    _fileTh = NULL;
    _fileOpenTimeTh = 0;
    _fileCountTh = 0;
    _fileSizeTh = 0;

    _lockTh = new CxMutex();
}

CxLogThread::~CxLogThread()
{
    delete _lockTh;
}

void CxLogThread::run()
{
    while (_isStarted)
    {
        if (_logStringsSize > 0)
        {
            doSaveLog();
        }
        else
        {
            //period 1 hour
            static msepoch_t dtNextChecktTime = CxTime::currentSystemTime();
            if (CxTime::currentSystemTime() > dtNextChecktTime)
            {
                dtNextChecktTime = CxTime::currentSystemTime() + (1000 * 60 * 60);
                fn_clearLogFiles();
            }
            CxThread::sleep(15);
        }
    }
}

void
CxLogThread::interinfo_out(const string &sInfo, const std::string &sTitle, int type, int reason, int source, int target, int tag)
{
    CxMutexScope lock(_lockTh);
    std::string sFileName = CxLogManager::fileName(type, reason, source, target, tag);
    FILE *oFile = CxLogManager::fileByFileName(sFileName);
    if (oFile)
    {
        std::string sText = fn_getLogString(sInfo, type, reason, source, target, tag);
        _logStringsPush->push_back(sText);
        _logFilesPush->push_back(oFile);
        _logStringsSize = _logStringsPush->size();
    }
}

void CxLogThread::doSaveLog()
{
    if (_logStringsSize > 0)
    {
        {
            CxMutexScope lock(_lockTh);
            std::vector<std::string> *tmpStrings = _logStringsPush;
            _logStringsPush = _logStringsPop;
            _logStringsPop = tmpStrings;
            std::vector<FILE *> *tmpFiles = _logFilesPush;
            _logFilesPush = _logFilesPop;
            _logFilesPop = tmpFiles;
            _logStringsSize = 0;
        }
        for (size_t i = 0; i < _logStringsPop->size(); ++i)
        {
            const string &sLogString = _logStringsPop->at(i);
            FILE *oFile = _logFilesPop->at(i);

            string sText = CxTime::currentSystemTimeString() + " " + sLogString + "\r\n";
            size_t iWrote = fwrite(const_cast<char *>(sText.data()), 1, sText.size(), oFile);

            fflush(oFile);
            _fileCountTh += sText.size();
            _fileSizeTh += iWrote;
        }
        _logStringsPop->clear();
        _logFilesPop->clear();
    }
}
