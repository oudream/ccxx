#include "daemon_agent.h"

#include <ccxx/ccxx.h>
#include "./../../DB/svrglobal.h"

using namespace std;
using namespace rapidjson;

const string f_sDaemonTargetName = "daemon";
const string f_sDb2fileTargetName = "db2file";

string f_sNodeID;
string f_sDaemonFileName;
string f_sDaemonFilePath;
string f_sDaemonLogPath;
string f_sDaemonConfigFilePath;
string f_sScanRecordFilePath;
string f_sDb2fileFilePath;
const string f_sTableNameDaemonAppConfig = "ha_appconfig";
const string f_sTbiOrderNo = "OrderNo";
string f_sSqlSelectDaemonAppConfig;

pid_os_t f_pidDaemon = CxProcess::allocEmpty();

const string cs_sql_create_daemon_app_config = //"select * from ha_appconfig;";
    "CREATE TABLE IF NOT EXISTS `ha_appconfig` (\n"
        "  `AppID` int(11) NOT NULL DEFAULT '0',\n"
        "  `NodeID` int(11) DEFAULT '0',\n"
        "  `OrderNo` int(11) DEFAULT '0',\n"
        "  `AppName` char(128),\n"
        "  `AppPath` varchar(512),\n"
        "  `AppParam` varchar(512),\n"
        "  `CanNotRunning` int(11) DEFAULT '0',\n"
        "  `WorkPath` varchar(512),\n"
        "  `AllOver` int(11) DEFAULT '0',\n"
        "  `OperationalCfg` int(11) DEFAULT '0',\n"
        "  `OperationalTm` varchar(512) DEFAULT '0',\n"
        "  `StaySleep` int(11) DEFAULT '0',\n"
        "  `HeartbeatPort` int(11) DEFAULT '0',\n"
        "  `HeartbeatTimeout` int(11) DEFAULT '0',\n"
        "  `HeartbeatData` int(11) DEFAULT '0',\n"
        "  `WithWin` int(11) DEFAULT '0',\n"
        "  `CpuLimit` int(8) DEFAULT '0',\n"
        "  `MemLimit` int(8) DEFAULT '0',\n"
        "  `AppAlias` varchar(512) DEFAULT NULL,\n"
        "  `Supervise` int(8) DEFAULT '0',\n"
        "  `RunMode` int(8) DEFAULT '0',\n"
        "  `AppType` int(11) DEFAULT '0',\n"
        "  PRIMARY KEY (`AppID`)\n"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8";

/*
AppID
NodeID
OrderNo
AppName
AppPath
AppParam
CanNotRunning
WorkPath
AllOver
OperationalCfg
OperationalTm
StaySleep
HeartbeatPort
HeartbeatTimeout
HeartbeatData
WithWin
CpuLimit
MemLimit
AppAlias
Supervise
RunMode
 */
const string cs_sql_select_daemon_app_config =
//    "select AppID, NodeID, OrderNo, AppName, AppPath, AppParam, CanNotRunning, WorkPath, AllOver, OperationalCfg, OperationalTm, StaySleep, HeartbeatPort, HeartbeatTimeout, HeartbeatData, WithWin "
    "select * "
        " from ha_appconfig where NodeID = %s order by OrderNo;";

std::vector<ProcessStatus> f_processStatuses;
std::vector<ProcessAlarm> f_processAlarms;

DaemonAgent::ScanRecord f_currentScanRecord;

std::string DaemonAgent::getDaemonLogFilePath()
{
    string sFilepath = CxFileSystem::extractFileName(DaemonGlobal::getLogFilePath());
    sFilepath = CxFileSystem::mergeFilePath(f_sDaemonLogPath, sFilepath);
    return sFilepath;
}

DaemonAgent::ScanRecord DaemonAgent::loadScanRecord()
{
    DaemonAgent::ScanRecord r;
    r.lastModifyTime = 0;
    r.fileSize = 0;
    r.scanOffset = 0;
    r.filepath = getDaemonLogFilePath();
    string sScanRecord = CxFile::load(f_sScanRecordFilePath);
    if (sScanRecord.size() > 0)
    {
        map<string, string> sScanRecord2 = CxString::splitToMap(sScanRecord, '=', ';');
        string sFilePath = CxContainer::value(sScanRecord2, string("filepath"), string());
        if (CxString::equalCase(r.filepath, sFilePath))
        {
            r.lastModifyTime = CxContainer::valueTo(sScanRecord2, string("lastModifyTime"), ci_longlong_zero);
            r.fileSize = CxContainer::valueTo(sScanRecord2, string("size"), ci_longlong_zero);
            r.scanOffset = CxContainer::valueTo(sScanRecord2, string("scanOffset"), ci_longlong_zero);
        }
    }
    return r;
}

void DaemonAgent::saveCurrentScanRecord()
{
    string sLine = CxString::format("filepath=%s;lastModifyTime=%lld;size=%lld;scanOffset=%lld",
                                    f_currentScanRecord.filepath.c_str(), f_currentScanRecord.lastModifyTime,
                                    f_currentScanRecord.fileSize, f_currentScanRecord.scanOffset);
    CxFile::save(f_sScanRecordFilePath, sLine);
}

bool DaemonAgent::init()
{
    bool r = true;
    f_sDaemonLogPath = CxFileSystem::mergeFilePath(CxFileSystem::mergeFilePath(CxAppEnv::applicationPath(), "log"), f_sDaemonTargetName);
    f_sDaemonConfigFilePath = CxFileSystem::mergeFilePath(CxAppEnv::applicationPath(),
                                                          f_sDaemonTargetName + ".config.ini");
    f_sScanRecordFilePath = CxFileSystem::mergeFilePath(CxAppEnv::tempPath(), "scan.record.txt");
#ifdef GM_OS_WIN
    f_sDaemonFileName = f_sDaemonTargetName + ".exe";
    f_sDb2fileFilePath = CxFileSystem::mergeFilePath(CxAppEnv::applicationPath(), f_sDb2fileTargetName + ".exe");
#else
    f_sDaemonFileName = f_sDaemonTargetName;
    f_sDb2fileFilePath = CxFileSystem::mergeFilePath(CxAppEnv::applicationPath(), f_sDb2fileTargetName);
#endif
    f_sDaemonFilePath = CxFileSystem::mergeFilePath(CxAppEnv::applicationPath(), f_sDaemonFileName);
    f_sNodeID = SvrGlobal::findConfig(CS_SectionSelf, "ServerId", string());
    if (f_sNodeID.size() > 0)
    {
        f_sSqlSelectDaemonAppConfig = CxString::format(cs_sql_select_daemon_app_config.c_str(), f_sNodeID.c_str());
        cxLog() << "f_sSqlSelectDaemonAppConfig: " << f_sSqlSelectDaemonAppConfig;
    }
    else
    {
        cxWarn() << "error error error!!! do not config ServerId.";
        r = false;
    }
    CxDatabase *oDb = CxDatabaseManager::getDefaultDb();
    if (!oDb)
    {
        string sDBSource = SvrGlobal::findConfig(CS_SectionSelf, string("DBSource"), string());
        string sDBType = SvrGlobal::findConfig(CS_SectionSelf, string("DBType"), string());
        if (sDBSource.size() > 0)
        {
            oDb = CxDatabaseManager::createDatabase(sDBSource, sDBType);
            oDb->openDatabase();
        }
    }
    if (oDb && oDb->isOpen())
    {
        int iResutl = oDb->execSql(cs_sql_create_daemon_app_config);
        cxWarn() << "execSql(cs_sql_create_daemon_app_config):" << iResutl;
    }
    else
    {
        cxWarn() << "error error error!!! default db is invalid.";
        r = false;
    }

    f_currentScanRecord = loadScanRecord();

    gs_defaultBinPath = CxAppEnv::applicationPath();

    return r;
}

void DaemonAgent::scan(int iInterval)
{
    CxFileSystem::PathInfo pathInfo = CxFileSystem::getPathInfo(f_currentScanRecord.filepath);
    f_currentScanRecord.lastModifyTime = pathInfo.lastWriteTime;
    f_currentScanRecord.fileSize = pathInfo.fileSize;
    if (pathInfo.fileSize > f_currentScanRecord.scanOffset)
    {
        string sOut;
        FILE *pFile;
        pFile = fopen(pathInfo.filePath().c_str(), "rb");
        if (pFile == NULL) return;
        fpos_t iPos = f_currentScanRecord.scanOffset > 0 ? f_currentScanRecord.scanOffset : 0;
        int64 iSize =
            f_currentScanRecord.scanOffset > 0 ? pathInfo.fileSize - f_currentScanRecord.scanOffset : pathInfo.fileSize;
        //300MB
        if (iPos > pathInfo.fileSize || iPos > 1024 * 1024 * 30 || iSize > 1024 * 1024 * 30)
        {
            cxWarn() << "DaemonAgent::scan capability < 30MB. file tool big.";
        }
        else
        {
            size_t iSize2 = iSize;
            sOut.resize(iSize2);
            fsetpos(pFile, &iPos);
            size_t iResult = fread(const_cast<char *>(sOut.data()), 1, iSize2, pFile);
            cxDebug() << "DaemonAgent::scan read size=" << iResult;
        }
        fclose(pFile);

        int iDealed = publishDaemonData(sOut);
        int64 iScanOffset = f_currentScanRecord.scanOffset + iDealed;
        if (iScanOffset > pathInfo.fileSize)
        {
            cxWarn() << "inter error!!! DaemonAgent::scanOffset=" << iScanOffset;
            iScanOffset = pathInfo.fileSize;
        }

        f_currentScanRecord.scanOffset = iScanOffset;

        cxDebug() << "DaemonAgent::scanFileSize: " << pathInfo.fileSize
                   << "scanOffset: " << f_currentScanRecord.scanOffset;

        saveCurrentScanRecord();
    }
}

int DaemonAgent::publishDaemonData(const std::string &sDaemonData)
{
    int r = 0;
    vector<string> sDaemonLogs = CxString::split(sDaemonData, ",\r\n");
    if (sDaemonLogs.size() > 100)
    {
        cxDebug() << "DaemonAgent::publishDaemonData begin. count " << sDaemonLogs.size();
    }
    for (size_t i = 0; i < sDaemonLogs.size(); ++i)
    {
        const string &sDaemonLog = sDaemonLogs.at(i);
        if (sDaemonLog.size() > 10)
        {
            Document d;
            d.Parse(sDaemonLog.data());
            if (d.HasParseError())
            {
                r += sDaemonLog.size() + 3;
                if (sDaemonLogs.size() > 100)
                {
                    cxLog() << "DaemonAgent::publishDaemonData d.HasParseError. " << sDaemonLog << cxEndLine;
                }
                else
                {
                    cxWarn() << "DaemonAgent::publishDaemonData d.HasParseError. " << sDaemonLog << cxEndLine;
                }
                continue;
            }
            string sT = CxJson::findMemberToString(d, "t");
            string sC = CxJson::findMemberToString(d, "c");
            string sI = CxJson::findMemberToString(d, "i");
            const rapidjson::Value *vD = CxJson::findMember(d, "d");
            if (!vD || !vD->IsObject())
            {
                r += sDaemonLog.size() + 3;
                if (sDaemonLogs.size() > 100)
                {
                    cxLog() << "DaemonAgent::publishDaemonData d is not object, c=" << sC << ", t=" << sT << ", i="
                            << cxEndLine;
                }
                else
                {
                    cxDebug() << "DaemonAgent::publishDaemonData d is not object, c=" << sC << ", t=" << sT << ", i="
                               << cxEndLine;
                }
                continue;
            }
            if (sC == ProcessStatus::getLogClass())
            {
                ProcessStatus processStatus;
                if (ProcessStatus::fromJson(vD, processStatus))
                {
                    if (fn_add_ProcessStatus)
                    {
                        int iResult = fn_add_ProcessStatus(processStatus);
                        if (iResult <= 0)
                        {
                            break;
                        }
                        if (sDaemonLogs.size() > 100)
                        {
                            cxLog() << "DaemonAgent::publishDaemonData c=" << sC << ", t=" << sT << ", i=" << sI
                                    << ", had publish->result=" << iResult << cxEndLine;
                        }
                        else
                        {
                            cxDebug() << "DaemonAgent::publishDaemonData c=" << sC << ", t=" << sT << ", i=" << sI
                                       << ", had publish->result=" << iResult << cxEndLine;
                        }
                    }
                    addProcessStatus(processStatus);
                }
            }
            else if (sC == ProcessAlarm::getLogClass())
            {
                ProcessAlarm processAlarm;
                if (ProcessAlarm::fromJson(vD, processAlarm))
                {
                    if (fn_add_ProcessAlarm)
                    {
                        int iResult = fn_add_ProcessAlarm(processAlarm);
                        if (iResult <= 0)
                        {
                            break;
                        }
                        if (sDaemonLogs.size() > 100)
                        {
                            cxLog() << "DaemonAgent::publishDaemonData c=" << sC << ", t=" << sT << ", i=" << sI
                                    << ", had publish->result=" << iResult << cxEndLine;
                        }
                        else
                        {
                            cxDebug() << "DaemonAgent::publishDaemonData c=" << sC << ", t=" << sT << ", i=" << sI
                                       << ", had publish->result=" << iResult << cxEndLine;
                        }
                    }
                    addProcessAlarm(processAlarm);
                }
            }
            else
            {
                r += sDaemonLog.size() + 3;
                if (sDaemonLogs.size() > 100)
                {
                    cxLog() << "DaemonAgent::publishDaemonData sC=" << sC << ", has not Log Class!" << cxEndLine;
                }
                else
                {
                    cxDebug() << "DaemonAgent::publishDaemonData sC=" << sC << ", has not Log Class!" << cxEndLine;
                }
                continue;
            }
        }
        r += sDaemonLog.size() + 3;
    }
    if (sDaemonLogs.size() > 100)
    {
        cxDebug() << "DaemonAgent::publishDaemonData end.";
    }
    return r;
};

bool DaemonAgent::startDaemon()
{
//    f_pidDaemon = CxProcess::execAsDetached(f_sDaemonFilePath);
    f_pidDaemon = CxProcess::exec(f_sDaemonFilePath);

    cxDebug() << "startDaemon - f_sDaemonFilePath : " << f_sDaemonFilePath;
#ifdef GM_OS_WIN
    cxDebug() << "startDaemon pid : " << f_pidDaemon.dwProcessId;
#endif

    return CxProcess::isvalid(f_pidDaemon);
}

void DaemonAgent::checkDaemon(int iInterval)
{
    if (!CxProcess::isRunning(f_sDaemonFileName))
    {
        loadConfig2file();
        startDaemon();
    }
    string sDaemonLogFilePath = getDaemonLogFilePath();
    if (f_currentScanRecord.filepath != sDaemonLogFilePath)
    {
        f_currentScanRecord.filepath = sDaemonLogFilePath;
        f_currentScanRecord.lastModifyTime = 0;
        f_currentScanRecord.fileSize = 0;
        f_currentScanRecord.scanOffset = 0;
    }
}

void DaemonAgent::restartDaemon()
{
    cxDebug() << "DaemonAgent::restartDaemon() begin : daemonIsExist:" << CxProcess::isRunning(f_sDaemonFileName);
    CxProcess::kill(f_sDaemonFileName);
    startDaemon();
    cxDebug() << "DaemonAgent::restartDaemon() end : daemonIsExist:" << CxProcess::isRunning(f_sDaemonFileName);
}

void DaemonAgent::loadConfigAndRestartDaemon()
{
    DaemonAgent::loadConfig2file();
    CxFileSystem::PathInfo pathInfo = CxFileSystem::getPathInfo(DaemonAgent::daemonConfigFilePath());
    bool bLoad = CxTime::milliSecondDifferToNow(pathInfo.lastWriteTime) < 1000 * 3;
    cxDebug() << "DaemonAgent::loadConfig2file() : " << bLoad;
    if (bLoad)
    {
        DaemonAgent::restartDaemon();
    }
}

int DaemonAgent::loadConfig2file()
{
    CxDatabase *oDb = CxDatabaseManager::getDefaultDb();
    if (oDb && oDb->isOpen())
    {
#ifdef GM_OS_WIN
        system("Net user administrator /active:yes");
#endif
        string sCmd = CxString::format("%s -dbs %s -sql \"%s=%s\" -tbi %s=%s -fp \"%s\"",
                                       f_sDb2fileFilePath.c_str(),
                                       oDb->connectSource().c_str(),
                                       f_sTableNameDaemonAppConfig.c_str(), f_sSqlSelectDaemonAppConfig.c_str(),
                                       f_sTableNameDaemonAppConfig.c_str(), f_sTbiOrderNo.c_str(),
                                       CxFileSystem::convertPathStyle(f_sDaemonConfigFilePath).c_str());
        system(sCmd.c_str());
        cxDebug() << "Run LoadConfig2file - CMD : " << sCmd;
    }
    return 0;
}

std::string DaemonAgent::daemonFilePath()
{
    return f_sDaemonFilePath;
}

std::string DaemonAgent::daemonConfigFilePath()
{
    return f_sDaemonConfigFilePath;
}

const DaemonAgent::ScanRecord &DaemonAgent::currentScanRecord()
{
    return f_currentScanRecord;
}

const std::vector<ProcessStatus> &DaemonAgent::processStatuses()
{
    return f_processStatuses;
}

const ProcessStatus *DaemonAgent::processStatuse(const std::string &sProcessName)
{
    for (int i = 0; i < f_processStatuses.size(); ++i)
    {
        ProcessStatus &pss = f_processStatuses.at(i);
        if (pss.process == sProcessName)
        {
            return &pss;
        }
    }
    return NULL;
}

const ProcessStatus *DaemonAgent::processStatuse(int iAppid)
{
    return findProcessStatus(iAppid);
}

void DaemonAgent::addProcessStatus(const ProcessStatus &processStatus)
{
    ProcessStatus *oProcessStatus = findProcessStatus(processStatus.aid);
    if (oProcessStatus == NULL)
    {
        f_processStatuses.push_back(processStatus);
    }
    else
    {
        if (oProcessStatus->recordTime != processStatus.recordTime)
        {
            oProcessStatus->assignedFrom(processStatus);
        }
    }
}

ProcessStatus *DaemonAgent::findProcessStatus(int iAppid)
{
    for (int i = 0; i < f_processStatuses.size(); ++i)
    {
        ProcessStatus &pss = f_processStatuses.at(i);
        if (pss.aid == iAppid)
        {
            return &pss;
        }
    }
    return NULL;
}

const std::vector<ProcessAlarm> &DaemonAgent::processAlarms()
{
    return f_processAlarms;
}

const ProcessAlarm *DaemonAgent::processAlarm(const std::string &sProcessName)
{
    for (int i = 0; i < f_processAlarms.size(); ++i)
    {
        ProcessAlarm &pss = f_processAlarms.at(i);
        if (pss.process == sProcessName)
        {
            return &pss;
        }
    }
    return NULL;
}

const ProcessAlarm *DaemonAgent::processAlarm(int iAppid)
{
    return findProcessAlarm(iAppid);
}

void DaemonAgent::addProcessAlarm(const ProcessAlarm &processAlarm)
{
    ProcessAlarm *oProcessAlarm = findProcessAlarm(processAlarm.aid);
    if (oProcessAlarm == NULL)
    {
        f_processAlarms.push_back(processAlarm);
    }
    else
    {
        if (oProcessAlarm->alarmTime != processAlarm.alarmTime)
        {
            oProcessAlarm->assignedFrom(processAlarm);
        }
    }

    if (f_processAlarms.size() > 1000)
    {
        f_processAlarms = vector<ProcessAlarm>(f_processAlarms.begin(), f_processAlarms.begin() + 1000);
    }
}

ProcessAlarm *DaemonAgent::findProcessAlarm(int iAppid)
{
    for (int i = 0; i < f_processAlarms.size(); ++i)
    {
        ProcessAlarm &pss = f_processAlarms.at(i);
        if (pss.aid == iAppid)
        {
            return &pss;
        }
    }
    return NULL;
}

int DaemonAgent::getServerid()
{
    return CxString::toInt32(f_sNodeID);
}

std::vector<std::string> DaemonAgent::reportSelf()
{
    vector<string> sReports;

    string sLine;
    sReports.push_back(sLine);
    sLine = CxString::format("f_sDaemonTargetName : %s", f_sDaemonTargetName.c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("f_sDb2fileTargetName : %s", f_sDb2fileTargetName.c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("f_sNodeID : %s", f_sNodeID.c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("f_sDaemonFileName : %s", f_sDaemonFileName.c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("f_sDaemonFilePath : %s", f_sDaemonFilePath.c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("f_sDaemonLogPath : %s", f_sDaemonLogPath.c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("f_sDaemonConfigFilePath : %s", f_sDaemonConfigFilePath.c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("f_sScanRecordFilePath : %s", f_sScanRecordFilePath.c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("f_sDb2fileFilePath : %s", f_sDb2fileFilePath.c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("f_sTableNameDaemonAppConfig : %s", f_sTableNameDaemonAppConfig.c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("f_sTbiOrderNo : %s", f_sTbiOrderNo.c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("f_sSqlSelectDaemonAppConfig : %s", f_sSqlSelectDaemonAppConfig.c_str());
#ifdef GM_OS_WIN
    sReports.push_back(sLine);
    sLine = CxString::format("f_pidDaemon.hProcess : %d", f_pidDaemon.hProcess);
#else
    r.push_back(sLine); sLine = CxString::format("f_pidDaemon : %d", f_pidDaemon);
#endif
    sReports.push_back(sLine);
    sLine = CxString::format("cs_sql_create_daemon_app_config : %s", cs_sql_create_daemon_app_config.c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("cs_sql_select_daemon_app_config : %s", cs_sql_select_daemon_app_config.c_str());
    sReports.push_back(sLine);
    sLine = "\r\n";

    sReports.push_back(sLine);
    sLine = "f_processStatuses->begin:";
    sReports.push_back(sLine);
    sLine = CxString::format("count : %d", f_processStatuses.size());
    for (int i = 0; i < f_processStatuses.size(); ++i)
    {
        ProcessStatus &pss = f_processStatuses.at(i);
        sReports.push_back(sLine);
        sLine = pss.toJson();
    }
    sReports.push_back(sLine);
    sLine = "f_processStatuses->end.";

    sReports.push_back(sLine);
    sLine = "f_processAlarms->begin:";
    sReports.push_back(sLine);
    sLine = CxString::format("count : %d", f_processAlarms.size());
    for (int i = 0; i < f_processAlarms.size(); ++i)
    {
        ProcessAlarm &psa = f_processAlarms.at(i);
        sReports.push_back(sLine);
        sLine = psa.toJson();
    }
    sReports.push_back(sLine);
    sLine = "f_processAlarms->end.";

    sReports.push_back(sLine);
    sLine = CxString::format("f_currentScanRecord.filepath : %s", f_currentScanRecord.filepath.c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("f_currentScanRecord.filepath : %lld", CxTime::toString(f_currentScanRecord.lastModifyTime).c_str());
    sReports.push_back(sLine);
    sLine = CxString::format("f_currentScanRecord.fileSize : %lld", f_currentScanRecord.fileSize);
    sReports.push_back(sLine);
    sLine = CxString::format("f_currentScanRecord.scanOffset : %lld", f_currentScanRecord.scanOffset);

    sReports.push_back(sLine);

    string sFilePath = CxFileSystem::mergeFilePath(CxAppEnv::tempPath(), CxAppEnv::applicationTargetName()
                                                                         + CxTime::currentSystemTimeString('-', 't', '-')
                                                                         + "-report.txt");
    CxFile::save(sFilePath, sReports);
    cxDebug() << "DaemonAgent::reportSelf and save to file : " << sFilePath;

    return sReports;
}

//map - update
template<typename _Key, typename _Tp>
static std::map<_Key, _Tp> convert(const std::map<_Key, _Tp> &source, const std::map<_Key, _Tp> &keyMap)
{
    std::map<_Key, _Tp> r;
    for (typename std::map<_Key, _Tp>::const_iterator it = source.begin(); it != source.end(); ++it)
    {
        typename std::map<_Key, _Tp>::const_iterator itFind = keyMap.find(it->first);
        if (itFind != keyMap.end())
            r[itFind->second] = it->second;
        else
            r[it->first] = it->second;
    }
    return r;
}

std::vector<ProcessConfig> DaemonAgent::loadDb2ProcessConfigs()
{
    CxDatabase *oDb = CxDatabaseManager::getDefaultDb();
    if (oDb && oDb->isOpen())
    {
        std::vector<std::map<std::string, std::string> > processConfigLines = oDb->queryToMapVector(f_sSqlSelectDaemonAppConfig);

        std::map<std::string, std::string> columnCoverts = DaemonAgent::getColumnCoverts();
        std::vector<std::map<std::string, std::string> > processConfigLinesNew;
        cxLog() << "loadDb2ProcessConfigs.processConfigLines, count: " << processConfigLines.size();
        for (int i = 0; i < processConfigLines.size(); ++i)
        {
            const std::map<std::string, std::string> &processConfigLine = processConfigLines.at(i);
            if (columnCoverts.size() > 0)
            {
                std::map<std::string, std::string> processConfigLineNew = convert(processConfigLine, columnCoverts);
                processConfigLinesNew.push_back(processConfigLineNew);
                cxLog() << "converted: true, " << processConfigLineNew;
            }
            else
            {
                processConfigLinesNew.push_back(processConfigLine);
            }
            cxLog() << processConfigLine;
        }

        std::vector<ProcessConfig> processConfigs = ProcessConfig::prepareProcessers(processConfigLinesNew);

        cxLog() << "loadDb2ProcessConfigs.processConfigs, count: " << processConfigs.size();
        for (int i = 0; i < processConfigs.size(); ++i)
        {
            const ProcessConfig &processConfig = processConfigs.at(i);
            cxLog() << "exeFilePath: " << processConfig.exeFilePath;
            cxLog() << "commandLine: " << processConfig.commandLine;
            cxLog() << "localPort: " << processConfig.localPort
                    << ", heartHexString: " << processConfig.heartHexString
                    << ", startMode: " << processConfig.startMode
                    << ", aid: " << processConfig.aid
                    << ", ord: " << processConfig.ord
                    << ", AppAlias: " << processConfig.AppAlias
                    << ", RunMode: " << processConfig.RunMode
                    << ", AppType: " << processConfig.AppType;
        }

        return processConfigs;
    }

    return std::vector<ProcessConfig>();
}

std::map<std::string, std::string> DaemonAgent::getColumnCoverts()
{
    map<string, string> r;

    string sFilePath = CxFileSystem::mergeFilePath(CxAppEnv::configPath(), "db2file.config.json");

    CxSkverBase *oSkv = CxSkverManager::beginUse(sFilePath);
    if (oSkv)
    {
        string sColumnConverts = oSkv->getValue("ini_convert_entrynames", "ha_appconfig", string());
        if (sColumnConverts.size() > 0)
        {
            r = CxString::splitToMap(sColumnConverts, '=', ',');
        }
        CxSkverManager::endUse(oSkv);
    }

    return r;
}

