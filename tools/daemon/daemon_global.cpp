#include "daemon_global.h"

#if defined(WINVER) && WINVER <= 0x0501
#else
#include <Winternl.h>
#endif

using namespace std;


string gs_defaultBinPath;
fn_add_ProcessStatus_t fn_add_ProcessStatus = 0;
fn_add_ProcessAlarm_t fn_add_ProcessAlarm = 0;

#ifdef GM_OS_WIN

#if defined(WINVER) && WINVER <= 0x0501

DWORD GetRemoteCommandLine(HANDLE hProcess, LPWSTR pszBuffer, UINT bufferLength)
{

#ifndef NTSTATUS
	typedef LONG NTSTATUS;
#endif

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

    typedef struct _UNICODE_STRING {
		USHORT Length;
		USHORT MaximumLength;
		PWSTR  Buffer;
	} UNICODE_STRING;
	typedef UNICODE_STRING *PUNICODE_STRING;
	typedef const UNICODE_STRING *PCUNICODE_STRING;

	typedef enum _PROCESSINFOCLASS {
		ProcessBasicInformation = 0,
		ProcessDebugPort = 7,
		ProcessWow64Information = 26,
		ProcessImageFileName = 27,
		ProcessBreakOnTermination = 29
	} PROCESSINFOCLASS;

	typedef struct _PEB_LDR_DATA {
		BYTE Reserved1[8];
		PVOID Reserved2[3];
		LIST_ENTRY InMemoryOrderModuleList;
	} PEB_LDR_DATA, *PPEB_LDR_DATA;

	//typedef __success(return >= 0) LONG NTSTATUS;

	typedef struct _RTL_USER_PROCESS_PARAMETERS {
		BYTE Reserved1[16];
		PVOID Reserved2[10];
		UNICODE_STRING ImagePathName;
		UNICODE_STRING CommandLine;
	} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

	typedef
		VOID
		(NTAPI *PPS_POST_PROCESS_INIT_ROUTINE) (
			VOID
			);

	typedef struct _PEB {
		BYTE Reserved1[2];
		BYTE BeingDebugged;
		BYTE Reserved2[1];
		PVOID Reserved3[2];
		PPEB_LDR_DATA Ldr;
		PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
		BYTE Reserved4[104];
		PVOID Reserved5[52];
		PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
		BYTE Reserved6[128];
		PVOID Reserved7[1];
		ULONG SessionId;
	} PEB, *PPEB;

	typedef struct _PROCESS_BASIC_INFORMATION {
		PVOID Reserved1;
		PPEB PebBaseAddress;
		PVOID Reserved2[2];
		ULONG_PTR UniqueProcessId;
		PVOID Reserved3;
	} PROCESS_BASIC_INFORMATION;
	typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;

	typedef NTSTATUS(NTAPI* NtQueryInformationProcessPtr)(
		IN HANDLE ProcessHandle,
		IN PROCESSINFOCLASS ProcessInformationClass,
		OUT PVOID ProcessInformation,
		IN ULONG ProcessInformationLength,
		OUT PULONG ReturnLength OPTIONAL);

	typedef ULONG(NTAPI* RtlNtStatusToDosErrorPtr)(NTSTATUS Status);

	// Locating functions
	HINSTANCE hNtDll = GetModuleHandleW(L"ntdll.dll");
	NtQueryInformationProcessPtr NtQueryInformationProcess = (NtQueryInformationProcessPtr)GetProcAddress(hNtDll, "NtQueryInformationProcess");
	RtlNtStatusToDosErrorPtr RtlNtStatusToDosError = (RtlNtStatusToDosErrorPtr)GetProcAddress(hNtDll, "RtlNtStatusToDosError");

	if (!NtQueryInformationProcess || !RtlNtStatusToDosError)
	{
		printf("Functions cannot be located.\n");
		return 0;
	}

    // Get PROCESS_BASIC_INFORMATION
    PROCESS_BASIC_INFORMATION pbi;
    ULONG len;
    NTSTATUS status = NtQueryInformationProcess(
        hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &len);
    SetLastError(RtlNtStatusToDosError(status));
    if (!NT_SUCCESS(status) || !pbi.PebBaseAddress)
    {
        printf("NtQueryInformationProcess(ProcessBasicInformation) failed.\n");
        return 0;
    }

    // Read PEB memory block
    SIZE_T bytesRead = 0;
    PEB peb;
    if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), &bytesRead))
    {
        printf("Reading PEB failed.\n");
        return 0;
    }

    // Obtain size of commandline string
    RTL_USER_PROCESS_PARAMETERS upp;
    if (!ReadProcessMemory(hProcess, peb.ProcessParameters, &upp, sizeof(upp), &bytesRead))
    {
        printf("Reading USER_PROCESS_PARAMETERS failed.\n");
        return 0;
    }

    if (!upp.CommandLine.Length)
    {
        printf("Command line length is 0.\n");
        return 0;
    }

    // Check the buffer size
    DWORD dwNeedLength = (upp.CommandLine.Length + 1) / sizeof(wchar_t) + 1;
    if (bufferLength < dwNeedLength)
    {
        printf("Not enough buffer.\n");
        return dwNeedLength;
    }

    // Get the actual command line
    pszBuffer[dwNeedLength - 1] = L'\0';
    if (!ReadProcessMemory(hProcess, upp.CommandLine.Buffer, pszBuffer, upp.CommandLine.Length, &bytesRead))
    {
        printf("Reading command line failed.\n");
        return 0;
    }

    return bytesRead / sizeof(wchar_t);
}

#else

DWORD GetRemoteCommandLine(HANDLE hProcess, LPWSTR pszBuffer, UINT bufferLength)
{
    // Get PROCESS_BASIC_INFORMATION
    PROCESS_BASIC_INFORMATION pbi;
    ULONG len;
    NTSTATUS status = NtQueryInformationProcess(
        hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &len);
    SetLastError(RtlNtStatusToDosError(status));
    if (!NT_SUCCESS(status) || !pbi.PebBaseAddress)
    {
        printf("NtQueryInformationProcess(ProcessBasicInformation) failed.\n");
        return 0;
    }

    // Read PEB memory block
    SIZE_T bytesRead = 0;
    PEB peb;
    if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), &bytesRead))
    {
        printf("Reading PEB failed.\n");
        return 0;
    }

    // Obtain size of commandline string
    RTL_USER_PROCESS_PARAMETERS upp;
    if (!ReadProcessMemory(hProcess, peb.ProcessParameters, &upp, sizeof(upp), &bytesRead))
    {
        printf("Reading USER_PROCESS_PARAMETERS failed.\n");
        return 0;
    }

    if (!upp.CommandLine.Length)
    {
        printf("Command line length is 0.\n");
        return 0;
    }

    // Check the buffer size
    DWORD dwNeedLength = (upp.CommandLine.Length + 1) / sizeof(wchar_t) + 1;
    if (bufferLength < dwNeedLength)
    {
        printf("Not enough buffer.\n");
        return dwNeedLength;
    }

    // Get the actual command line
    pszBuffer[dwNeedLength - 1] = L'\0';
    if (!ReadProcessMemory(hProcess, upp.CommandLine.Buffer, pszBuffer, upp.CommandLine.Length, &bytesRead))
    {
        printf("Reading command line failed.\n");
        return 0;
    }

    return bytesRead / sizeof(wchar_t);
}

#endif

int DaemonGlobal::fillCommandLine(std::vector<CxProcess::ProcessInfo> &processInfos)
{
    int r = 0;
    WCHAR pszBuffer[4096];
    UINT bufferLength = 4096;
    for (int i = 0; i < processInfos.size(); ++i)
    {
        CxProcess::ProcessInfo &processInfo = processInfos.at(i);
        if (processInfo.pid.dwProcessId > 0)
        {
            HANDLE hProcess = OpenProcess(
                PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processInfo.pid.dwProcessId);
            if( hProcess == NULL )
            {
                std::cout << "OpenProcess : " << processInfo.pid.dwProcessId << " error" << std::endl;
            }
            else
            {
                DWORD iCommandLineLength = GetRemoteCommandLine(hProcess, pszBuffer, bufferLength);
                if (iCommandLineLength > 0)
                {
                    string sCommandLine = CxString::fromWstring(wstring(pszBuffer, iCommandLineLength));
                    sCommandLine = CxString::replace(sCommandLine, "\\\\", "\\");
                    sCommandLine = CxString::trim(sCommandLine);
                    sCommandLine = CxString::trim(sCommandLine, '"');
                    sCommandLine = CxString::trim(sCommandLine);
                    if (sCommandLine.size() > 0 && sCommandLine[0] == '\\')
                    {
                        sCommandLine = sCommandLine.substr(1);
                    }
                    sCommandLine = CxString::trim(sCommandLine);
                    size_t iPos = sCommandLine.find(processInfo.name);
                    if (iPos != string::npos)
                    {
                        size_t iNumber = iPos + processInfo.name.size();
                        processInfo.filePath = sCommandLine.substr(0, iNumber);
                        processInfo.filePath = CxFileSystem::normalize(processInfo.filePath);
                        if (iNumber + 1 < sCommandLine.size())
                        {
                            processInfo.runParam = sCommandLine.substr(iNumber + 1);
                        }
                        else
                        {
                            processInfo.runParam = string();
                        }
                    }
                    ++r;
                }
                CloseHandle( hProcess );
            }
        }
    }
    return r;
}

#else

int DaemonGlobal::fillCommandLine(std::vector<CxProcess::ProcessInfo> &processInfos)
{
    return 0;
}

#endif

std::string DaemonGlobal::getLogFilePath()
{
    msepoch_t lMsepoch = CxTime::currentSystemTime();
    int y, m, d, h, mi, se, ms;
    CxTime::decodeLocalTm(lMsepoch, y, m, d, h, mi, se, ms);
    return CxFileSystem::mergeFilePath(CxAppEnv::logPath(), CxString::format("%04d-%02d-%02d.json", y, m, d));
}

void DaemonGlobal::log(std::string sLogClass, std::string jsonLogContent, string sId)
{
    std::string sLogFilePath = DaemonGlobal::getLogFilePath();
    FILE *oFile = fopen(sLogFilePath.c_str(), "ab+");
    if (oFile)
    {
        std::string sBegin = "{\"t\":\"%s\",\"c\":\"%s\",\"i\":\"%s\",\"d\":";
        std::string sEnd = "},\r\n";
        std::string sNow = CxTime::currentSystemTimeString();
        std::string sLine = CxString::format(sBegin.c_str(), sNow.c_str(), sLogClass.c_str(), sId.c_str());
        fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), oFile);
        fwrite(const_cast<char *>(jsonLogContent.data()), 1, jsonLogContent.size(), oFile);
        fwrite(const_cast<char *>(sEnd.data()), 1, sEnd.size(), oFile);
        fflush(oFile);
        fclose(oFile);
    }
}

const CxProcess::ProcessInfo *
DaemonGlobal::findProcessInfo(const std::vector<CxProcess::ProcessInfo> &processInfos, const ProcessConfig *oProcessConfig)
{
    if (oProcessConfig == NULL)
        return NULL;
    string sConfigFilePath = oProcessConfig->exeFilePath;
    string sProcessName = CxFileSystem::extractFileName(oProcessConfig->exeFilePath);
    string sRunParam = oProcessConfig->runParam;
    for(std::vector<CxProcess::ProcessInfo>::const_iterator it = processInfos.begin(); it != processInfos.end();)
    {
        const CxProcess::ProcessInfo &processInfo = *it;
        if (sRunParam.size() > 0 && processInfo.runParam.size() > 0)
        {
            vector<string> ss1 = CxString::split(sRunParam, ' ');
            vector<string> ss2 = CxString::split(processInfo.runParam, ' ');
            if (CxString::equal(ss1, ss2, true, true, true))
            {
                return & processInfo;
            }
        }
        else
        {
            if (CxString::equalCase(processInfo.name, sProcessName))
            {
                return & processInfo;
            }
        }
        ++it;
    }
    return NULL;
}

ProcessConfig ProcessConfig::createProcessConfig(const std::string & sProcessFilePath, const std::string &  sProcessWorkPath, int iWaitMillisecond,
                                         int iProcessType, int iLocalPort, const std::string &  sHeartBuffer, int iTimeOut_receive,
                                         const std::string &  sProcessParam, int iMode, int iAid, int iOrd, int iDm,
                                         int iCpuLimit, int iMemLimit, const std::string & sAppAlias, int iSupervise, int iRunMode, int iAppType) {
    ProcessConfig r;

    r.exeFilePath = CxString::trim(sProcessFilePath);
    r.workPath = (CxFileSystem::hasRootPath(sProcessWorkPath)) ? sProcessWorkPath : CxFileSystem::extractPath(r.exeFilePath);
    r.exeFileName = CxFileSystem::extractFileName(r.exeFilePath);
    r.processType = iProcessType;
    r.waitFirstMillisecond = iWaitMillisecond;

    r.localPort = iLocalPort;
    r.heartBuffer = sHeartBuffer;
    r.heartHexString = CxString::toHexstring(sHeartBuffer.data(), sHeartBuffer.size(), false);
    r.timeOut_receive = (unsigned int)iTimeOut_receive;
    if (r.timeOut_receive < 10000) r.timeOut_receive = 10000;

    r.startMode = iMode;
    r.runParam = sProcessParam;
    if (CxString::equalCase(r.runParam, "null") || CxString::equalCase(r.runParam, "0"))
    {
        r.runParam = string();
    }

    r.commandLine = r.exeFilePath;
    if (r.runParam.size() > 0)
    {
        r.commandLine.push_back(' ');
        r.commandLine.append(r.runParam);
    }
    r.aid = iAid;
    r.ord = iOrd;
    r.dm = iDm;
    r.CpuLimit         = iCpuLimit ;
    r.MemLimit         = iMemLimit ;
    r.AppAlias         = sAppAlias ;
    r.Supervise        = iSupervise;
    r.RunMode          = iRunMode != ProcessConfig::RunModePerformance ? ProcessConfig::RunModeAuto : iRunMode;
    r.AppType          = iAppType  ;

    return r;
}

std::vector<ProcessConfig>
ProcessConfig::prepareProcessers(const std::vector<std::map<std::string, std::string> > &processConfigLines)
{
    std::vector<ProcessConfig> r;

    std::vector<std::string> sLines;
    std::vector<std::string> sCmdFulls;
    std::vector<int> iPortes;

    for (size_t i = 0; i < processConfigLines.size(); ++i)
    {
        std::map<std::string, std::string> processConfigLine = processConfigLines.at(i);
        //pf=aaa/aaa.exe,pt=1,pw=3000,np=5566,nt=6600,nh=a55aa55a

        std::string sExeFilePath;
        int iWaitFirstMillisecond;
        int iProcessType;
        int iLocalPort;
        std::string sHBuf;
        int iTimeOut_receive = 0;
        std::string sPp;
        int iMode;
        std::string sHeartBuffer;
        int nDaemonMode = 0;

        std::string sDm = processConfigLine["dm"];
        if (sDm.length() > 0) nDaemonMode = atoi(sDm.c_str());

        std::string sPf = processConfigLine["pf"];
        std::string sPpf = processConfigLine["ppf"];
        sExeFilePath = getExeFilePath(sPf, sPpf);
        if (! isUrl(sExeFilePath) && ! CxFileSystem::hasRootPath(sExeFilePath))
        {
            std::cout << "config index : " << i << ", param: pf of ppf is invalid!" << std::endl;
            continue;
        }
        sPp = processConfigLine["pp"];
        if (sPp.size() > 255)
        {
            std::cout << "config index : " << i << ", param: pp size too long!" << std::endl;
            continue;
        }
        if (CxString::equalCase(sPp, "null") || CxString::equalCase(sPp, "0"))
        {
            sPp = string();
        }

        std::string sCommandLine = sExeFilePath;
        if (sPp.size() > 0)
        {
            sCommandLine.push_back(' ');
            sCommandLine.append(sPp);
        }
        sCommandLine = CxString::toLower(sCommandLine);
        if (find(sCmdFulls.begin(), sCmdFulls.end(), sCommandLine) != sCmdFulls.end())
        {
            std::cout << "config index : " << i << ", repeat commandLine!" << std::endl;
            continue;
        }
        sCmdFulls.push_back(sCommandLine);

        std::string sPt = processConfigLine["pt"];
        iProcessType = atoi(sPt.c_str());

        std::string sPw = processConfigLine["pw"];
        iWaitFirstMillisecond = atoi(sPw.c_str());
        if (!(iWaitFirstMillisecond >= 0 && iWaitFirstMillisecond < 60 * 1000 * 30))
        {
            std::cout << "config index : " << i << ", param: pw is invalid!" << std::endl;
            continue;
        }

        std::string sNp = processConfigLine["np"];
        iLocalPort = atoi(sNp.c_str());

        if (!(iLocalPort >= 0 && iLocalPort < USHRT_MAX))
        {
            std::cout << "config index : " << i << ", param: np is invalid!" << std::endl;
            continue;
        }
        if (iLocalPort > 0 && find(iPortes.begin(), iPortes.end(), iLocalPort) != iPortes.end())
        {
            std::cout << "config index : " << i << ", param: np is repeat!" << std::endl;
            continue;
        }
        iPortes.push_back(iLocalPort);

        if (iLocalPort > 0)
        {
            std::string sNt = processConfigLine["nt"];
            iTimeOut_receive = atoi(sNt.c_str());
            if (!(iTimeOut_receive >= 0 && iTimeOut_receive < 86400 * 1000))
            {
                std::cout << "config index : " << i << ", param: nt is invalid!" << std::endl;
                continue;
            }
        }

        if (iLocalPort > 0 && iTimeOut_receive > 0)
        {
            std::string sNh = processConfigLine["nh"];
            sHeartBuffer = sNh;
            if (sHeartBuffer.size() > 0)
            {
                if (!(CxString::isValidHexCharater(sHeartBuffer) && (sHeartBuffer.size() % 2 == 0)
                      && (sHeartBuffer.size() < 255)))
                {
                    std::cout << "config index : " << i << ", param: nh is invalid!" << std::endl;
                    continue;
                }
            }
            std::vector<char> hBuf = CxString::hexToData(sHeartBuffer);
            sHBuf = std::string(hBuf.data(), hBuf.size());
        }

        std::string sPm = processConfigLine["pm"];
        iMode = atoi(sPm.data());
        if (!(iMode >= 0 && iMode < 5))
        {
            std::cout << "config index : " << i << ", param: pm is invalid!" << std::endl;
            continue;
        }

        std::string sWorkPath = processConfigLine["pwd"];
        if (! CxFileSystem::hasRootPath(sWorkPath))
        {
            sWorkPath = CxFileSystem::extractPath(sExeFilePath);
        }

        std::string sAid = processConfigLine["aid"];
        int iAid = CxString::toInt32(sAid);
        std::string sOrd = processConfigLine["ord"];
        int iOrd = (sOrd.empty()) ? i : CxString::toInt32(sOrd);

        int iCpuLimit         = CxString::toInt32(processConfigLine["CpuLimit"]);
        int iMemLimit         = CxString::toInt32(processConfigLine["MemLimit"]);
        std::string sAppAlias = processConfigLine["AppAlias"];
        int iSupervise        = CxString::toInt32(processConfigLine["Supervise"]);
        int iRunMode          = CxString::toInt32(processConfigLine["RunMode"]);
        int iAppType          = CxString::toInt32(processConfigLine["AppType"]);

        r.push_back(createProcessConfig(sExeFilePath, sWorkPath, iWaitFirstMillisecond,
                                        iProcessType, iLocalPort, sHBuf, iTimeOut_receive,
                                        sPp, iMode, iAid, iOrd, nDaemonMode,
                                        iCpuLimit, iMemLimit, sAppAlias, iSupervise, iRunMode, iAppType));
    }
    return r;
}

std::string ProcessConfig::getExeFilePath(const std::string &sPf, const std::string &sPpf)
{
    if (sPf.empty())
    {
        return string();
    }

    std::string r = sPf;
    string sPpf2 = CxString::trim(sPpf);

    if (! isUrl(r) && ! CxFileSystem::hasRootPath(r))
    {
        if (sPpf2.size() > 0 && ! CxString::equalCase(sPpf2, "null"))
        {
            r = CxFileSystem::mergeFilePath(sPpf2, sPf);
        }
    }
    if (! isUrl(r) && ! CxFileSystem::hasRootPath(r))
    {
        if ((sPpf2.empty() || CxString::equalCase(sPpf2, "null")) && gs_defaultBinPath.size() > 0)
        {
            r = CxFileSystem::mergeFilePath(gs_defaultBinPath, r);
        }
        else
        {
            r = CxFileSystem::mergeFilePath(CxAppEnv::applicationPath(), r);
        }
    }
//            if (nDaemonMode == 1)
//            {
//                f_okillProcesses.push_back(sPf);
//
//                fn_outInfo(CxString::format("Append kill Process : %s ", sPf.c_str()));
//
//                continue;
//            }
    r = CxString::trim(CxFileSystem::normalize(r));
    return r;
}

bool ProcessConfig::isUrl(const std::string &sPath)
{
    return CxString::beginWithCase(sPath, "http") || CxString::endWithCase(sPath, "html");
}

vector<string> ProcessConfig::report(const ProcessConfig &processConfig)
{
    vector<string> r;
    r.push_back("exeFilePath: " + processConfig.exeFilePath);
    r.push_back("workPath: " + processConfig.workPath);
    r.push_back("exeFileName: " + processConfig.exeFileName);
    r.push_back("processType: " + CxString::toString(processConfig.processType));
    r.push_back("waitFirstMillisecond: " + CxString::toString(processConfig.waitFirstMillisecond));
    r.push_back("localPort: " + CxString::toString(processConfig.localPort));
    r.push_back("heartHexString: " + CxString::toString(processConfig.heartHexString));
    r.push_back("heartBuffer: " + CxString::toString(processConfig.heartBuffer));
    r.push_back("timeOut_receive: " + CxString::toString(processConfig.timeOut_receive));
    r.push_back("runParam: " + CxString::toString(processConfig.runParam));
    r.push_back("commandLine: " + CxString::toString(processConfig.commandLine));
    r.push_back("startMode: " + CxString::toString(processConfig.startMode));
    r.push_back("aid: " + CxString::toString(processConfig.aid));
    r.push_back("ord: " + CxString::toString(processConfig.ord));
    r.push_back("dm: " + CxString::toString(processConfig.dm));
    r.push_back("CpuLimit: " + CxString::toString(processConfig.CpuLimit));
    r.push_back("MemLimit: " + CxString::toString(processConfig.MemLimit));
    r.push_back("AppAlias: " + CxString::toString(processConfig.AppAlias));
    r.push_back("Supervise: " + CxString::toString(processConfig.Supervise));
    r.push_back("RunMode: " + CxString::toString(processConfig.RunMode));
    r.push_back("AppType: " + CxString::toString(processConfig.AppType));
    return r;
}

std::string
ProcessConfig::getTitle(const ProcessConfig& processConfig)
{
    return processConfig.exeFileName + " " + processConfig.runParam;
}

bool
ProcessConfig::isValid(const ProcessConfig& processConfig)
{
    return ! processConfig.exeFileName.empty();
}

std::string ProcessStatus::getLogClass()
{
    return "ProcessStatus";
}

bool ProcessStatus::fromJson(const rapidjson::Value *jValue, ProcessStatus &outProcessStatus)
{
    ProcessStatus & r = outProcessStatus;
    if (jValue && jValue->IsObject())
    {
        r.aid = CxRapidJson::findMemberToInt(*jValue, "aid");
        r.ord = CxRapidJson::findMemberToInt(*jValue, "ord");
        r.process = CxRapidJson::findMemberToString(*jValue, "process");
        r.isRunnig = CxRapidJson::findMemberToInt(*jValue, "isRunnig");
        r.startTime = CxRapidJson::findMemberToInt64(*jValue, "startTime");
        r.startTimes = CxRapidJson::findMemberToInt(*jValue, "startTimes");
        r.receiveBytes = CxRapidJson::findMemberToInt64(*jValue, "receiveBytes");
        r.lastReceiveTime = CxRapidJson::findMemberToInt64(*jValue, "lastReceiveTime");
        r.recordTime = CxRapidJson::findMemberToInt64(*jValue, "recordTime");
        r.syscpu = CxRapidJson::findMemberToInt(*jValue, "syscpu");
        r.sysmem = CxRapidJson::findMemberToInt(*jValue, "sysmem");
        r.cpu = CxRapidJson::findMemberToInt(*jValue, "cpu");
        r.mem = CxRapidJson::findMemberToInt(*jValue, "mem");
        return true;
    }
    return false;
}

std::string ProcessStatus::toJson()
{
    return CxString::format("{\"aid\": %d,\"ord\": %d,\"process\": \"%s\",\"isRunnig\": %d,\"startTime\": %lld,\"startTimes\": %d,\"receiveBytes\": %lld,\"lastReceiveTime\": %lld,\"recordTime\": %lld,\"syscpu\": %d,\"sysmem\": %d,\"cpu\": %d,\"mem\": %d}",
                            aid, ord, process.c_str(), isRunnig, startTime, startTimes, receiveBytes, lastReceiveTime, recordTime, syscpu, sysmem, cpu, mem);
}

void ProcessStatus::assignedFrom(const ProcessStatus &pss)
{
    isRunnig =          pss.isRunnig;
    startTime =         pss.startTime;
    startTimes =        pss.startTimes;
    receiveBytes =      pss.receiveBytes;
    lastReceiveTime =   pss.lastReceiveTime;
    syscpu =            pss.syscpu;
    sysmem =            pss.sysmem;
    cpu =               pss.cpu;
    mem =               pss.mem;
}


std::string ProcessAlarm::getLogClass()
{
    return "ProcessAlarm";
}

bool ProcessAlarm::fromJson(const rapidjson::Value *jValue, ProcessAlarm &outProcessAlarm)
{
    ProcessAlarm & r = outProcessAlarm;
    if (jValue && jValue->IsObject())
    {
        r.aid = CxRapidJson::findMemberToInt(*jValue, "aid");
        r.ord = CxRapidJson::findMemberToInt(*jValue, "ord");
        r.process = CxRapidJson::findMemberToString(*jValue, "process");
        r.alarmType = CxRapidJson::findMemberToInt(*jValue, "alarmType");
        r.alarmMsg = CxRapidJson::findMemberToString(*jValue, "alarmMsg");
        return true;
    }
    return false;
}

std::string ProcessAlarm::toJson()
{
    return CxString::format("{\"aid\": %d,\"ord\": %d,\"process\":\"%s\",\"alarmTime\": %lld,\"alarmType\":%d,\"alarmMsg\":\"%s\"}",
                            aid, ord, process.c_str(), alarmTime, alarmType, alarmMsg.c_str());
}

void ProcessAlarm::assignedFrom(const ProcessAlarm &psa)
{
    alarmTime =          psa.alarmTime;
    alarmType =         psa.alarmType;
    alarmMsg =        psa.alarmMsg;
}



