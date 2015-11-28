#include "cxlog.h"

#include "cxstring.h"
#include "cxtime.h"
#include "cxtimer.h"
#include "cxfile.h"
#include "cxapplication.h"


using namespace std;


const string CS_KEY_DIR_LOG = "LogPath";


CxLogRealtime * fn_logRealtimeInit()
{
//#ifdef DEBUG
//    CxApplication::registInitFunction(CxLogRealtime::startLog);
//    CxApplication::registStopFunction(CxLogRealtime::stopLog);
//    static CxLogRealtime logRealtime;
//    return & logRealtime;
//#else
//    return NULL;
//#endif
    return NULL;
}


CxLogThread * fn_logThreadInit()
{
    CxApplication::registInitFunction(CxLogThread::startLog);
    CxApplication::registStopFunction(CxLogThread::stopLog);
    static CxLogThread logThread;
    return & logThread;
}

static string f_currentLogDirectory;
static msepoch_t f_dirUpdateDatetime = CxTime::currentSystemTime();
static msepoch_t f_dirNextDatetime = CxTime::currentDayEnd() + 1000;
static const CxLogFileTactics_I * f_logFileTactics = NULL;
static map<string, FILE *> f_logFiles;

static CxLogRealtime * f_logRealtime = fn_logRealtimeInit();

static CxLogThread * f_logThread = fn_logThreadInit();


string fn_getLogString(const std::string& sInfo, int type = 0, int reason = 0, int source = 0, int target = 0, int tag = 0)
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

string fn_getLogString(const std::string& sInfo, const std::string& sTitle, int type = 0, int reason = 0, int source = 0, int target = 0, int tag = 0)
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
        sOutInfo << CxGlobal::lineString;
    sOutInfo << sInfo;
    return sOutInfo.str();
}

string fn_logFilePathByNow()
{
    tm dt = CxTime::localTm();
    string logFileName = CxString::toString(dt.tm_mday) + "-" + CxString::toString(dt.tm_hour) + ".log";
    //default = log
    string sLogPath = CxFileSystem::fullFilePath( CxApplication::applicationDeployPath(), CxApplication::findConfig(CS_SectionProgramConfig, "LogPath", std::string()) );
    if (sLogPath.empty())
    {
        sLogPath = CxFileSystem::mergeFilePath( CxApplication::applicationDeployPath() , "log" );
    }
    return CxFileSystem::mergeFilePath(sLogPath, logFileName);
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
        sFileName = CxApplication::applicationTargetName() + ".log";
    }
    return sFileName;
}

FILE *CxLogManager::fileByFileName(const string &sFileName)
{
    msepoch_t dtNow = CxTime::currentSystemTime();
    if (dtNow > f_dirNextDatetime)
    {
        for (map<string,FILE *>::iterator it = f_logFiles.begin(); it != f_logFiles.end(); ++it)
        {
            FILE * oFile = it->second;
            if (oFile)
            {
                fclose(oFile);
            }
        }
        f_logFiles.clear();
        f_dirUpdateDatetime = dtNow;
        f_dirNextDatetime = CxTime::currentDayEnd() + 1000;
        f_currentLogDirectory.clear();
        if (f_logFileTactics)
        {
            f_currentLogDirectory = CxFileSystem::mergeFilePath(f_logFileTactics->log_directory(), CxTime::currentDayString('-'));
            if (! CxFileSystem::isExist(f_currentLogDirectory)) CxFileSystem::createDirMultiLevel(f_currentLogDirectory);
        }
    }
    map<string,FILE *>::iterator it = f_logFiles.find(sFileName);
    if (it != f_logFiles.end())
    {
        return it->second;
    }
    else
    {
        if (f_currentLogDirectory.empty())
        {
            if (f_logFileTactics)
            {
                f_currentLogDirectory = CxFileSystem::mergeFilePath(f_logFileTactics->log_directory(), CxTime::currentDayString('-'));
            }
            else
            {
                f_currentLogDirectory = CxApplication::findConfig(CS_SectionProgramConfig, "LogPath", std::string());
                if (f_currentLogDirectory.empty())
                {
                    f_currentLogDirectory = CxFileSystem::mergeFilePath( CxApplication::applicationDeployPath() , "log" );
                }
                else
                {
                    f_currentLogDirectory = CxFileSystem::fullFilePath( CxApplication::applicationDeployPath() , f_currentLogDirectory );
                }
                f_currentLogDirectory = CxFileSystem::mergeFilePath(f_currentLogDirectory, CxTime::currentDayString('-'));
            }
            if (! CxFileSystem::isExist(f_currentLogDirectory)) CxFileSystem::createDirMultiLevel(f_currentLogDirectory);
        }
        string sFilePath = CxFileSystem::mergeFilePath(f_currentLogDirectory, sFileName);
        FILE * oFile = fopen (sFilePath.data(), "ab+");
        if ( oFile != NULL )
        {
            f_logFiles[sFileName] = oFile;
            return oFile;
        }
    }
    return NULL;
}




void CxLogRealtime::startLog()
{
    if (f_logRealtime)
    {
        CxInterinfoOut::addObserver(f_logRealtime);
    }
}

void CxLogRealtime::stopLog()
{
    if (f_logRealtime)
    {
        CxInterinfoOut::removeObserver(f_logRealtime);
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

void CxLogRealtime::interinfo_out(const string &sInfo, const std::string& sTitle, int type, int reason, int source, int target, int tag)
{
    std::string sFileName = CxLogManager::fileName(type, reason, source, target, tag);
    FILE * oFile = CxLogManager::fileByFileName(sFileName);
    if (oFile)
    {
        std::string sText;
        if (f_logFileTactics && ! f_logFileTactics->log_hasFileName(type, reason, source, target, tag))
            sText = fn_getLogString(sInfo, type, reason, source, target, tag);
        else
            sText = fn_getLogString(sInfo, sTitle, type, reason, source, target, tag);
        //增加时标和换行回车
        sText = CxTime::currentMsepochString()+" "+sText+"\r\n";

        size_t iWrote = fwrite (const_cast<char *>(sText.data()) , 1, sText.size(), oFile);
        fflush (_fileRt);
        _fileCountRt += sText.size();
        _fileSizeRt += iWrote;
    }
}





void CxLogThread::startLog()
{
    if (f_logThread)
    {
        CxInterinfoOut::addObserver(f_logThread);
        f_logThread->_stop = false;
        f_logThread->start();
    }
}

void CxLogThread::stopLog()
{
    if (f_logThread)
    {
        CxInterinfoOut::removeObserver(f_logThread);
        f_logThread->_stop = true;
        f_logThread->join();
    }
}

CxLogThread::CxLogThread()
{
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
}

CxLogThread::~CxLogThread()
{
}

void CxLogThread::run()
{
    while (! _stop)
    {
        if (_logStringsSize > 0)
        {
            {
                CxMutexScope lock(_lockTh);
                std::vector<std::string> * tmpStrings = _logStringsPush;
                _logStringsPush = _logStringsPop;
                _logStringsPop = tmpStrings;
                std::vector<FILE*> * tmpFiles = _logFilesPush;
                _logFilesPush = _logFilesPop;
                _logFilesPop = tmpFiles;
                _logStringsSize = 0;
            }
            for (size_t i = 0; i < _logStringsPop->size(); ++i)
            {
                string sText = _logStringsPop->at(i);
                FILE* oFile = _logFilesPop->at(i);

                //增加时标和换行回车
                sText = CxTime::currentMsepochString()+" "+sText+"\r\n";
                size_t iWrote = fwrite (const_cast<char *>(sText.data()) , 1, sText.size(), oFile);

                fflush (oFile);
                _fileCountTh += sText.size();
                _fileSizeTh += iWrote;
            }
            _logStringsPop->clear();
            _logFilesPop->clear();
        }
        else
        {
            CxThread::sleep(10);
        }
    }
}

void CxLogThread::interinfo_out(const string &sInfo, const std::string& sTitle, int type, int reason, int source, int target, int tag)
{
    CxMutexScope lock(_lockTh);
    std::string sFileName = CxLogManager::fileName(type, reason, source, target, tag);
    FILE * oFile = CxLogManager::fileByFileName(sFileName);
    if (oFile)
    {
        std::string sText = fn_getLogString(sInfo, type, reason, source, target, tag);
        _logStringsPush->push_back(sText);
        _logFilesPush->push_back(oFile);
        _logStringsSize = _logStringsPush->size();
    }
}
