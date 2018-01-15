#include "cxsystem.h"


#include "cxappenv.h"


#ifndef GM_OS_WIN

#include <sys/utsname.h>  // For uname.
#include <sys/stat.h>

#endif


using namespace std;


bool CxSystem::sendEmailInternal(const char*dest, const char *subject, const char*body)
{
    if (dest && *dest)
    {
        fprintf(stderr, "Trying to send TITLE: %s BODY: %s to %s\n", subject, body, dest);
        string cmd = string("/bin/mail -s\"") + subject + "\" " + dest;
        FILE* pipe = fopen(cmd.c_str(), "w");
        if (pipe != NULL)
        {
            // Add the body if we have one
            if (body)
            {
                fwrite(body, sizeof(char), strlen(body), pipe);
            }
            bool ok = fclose(pipe) != -1;
            if ( !ok )
            {
                fprintf(stderr, "Problems sending mail to %s: %s\n", dest, strerror(errno));
            }
            return ok;
        }
    }
    fprintf(stderr, "Unable to send mail to %s\n", dest);
    return false;
}

std::vector<string> CxSystem::getTempDirectories()
{
    std::vector<string> sTempDirectories;
#ifdef GM_OS_WIN
    // On windows we'll try to find a directory in this order:
    //   C:/Documents & Settings/whomever/TEMP (or whatever GetTempPath() is)
    //   C:/TMP/
    //   C:/TEMP/
    //   C:/WINDOWS/ or C:/WINNT/
    //   .
    char tmp[MAX_PATH];
    if (GetTempPathA(MAX_PATH, tmp))
        sTempDirectories.push_back( string(tmp) );
    sTempDirectories.push_back("C:\\tmp\\");
    sTempDirectories.push_back("C:\\temp\\");
#else
    // Directories, in order of preference. If we find a dir that
    // exists, we stop adding other less-preferred dirs
    const char * candidates[] = {
        // Non-null only during unittest/regtest
        getenv("TEST_TMPDIR"),

        // Explicitly-supplied temp dirs
        getenv("TMPDIR"), getenv("TMP"),

        // If all else fails
        "/tmp",
    };

    for (size_t i = 0; i < (sizeof(candidates) / sizeof(*(candidates))); i++) {
        const char *d = candidates[i];
        if (!d) continue;  // Empty env var

        // Make sure we don't surprise anyone who's expecting a '/'
        string dstr = d;
        if (dstr[dstr.size() - 1] != '/') {
            dstr += "/";
        }
        sTempDirectories.push_back(dstr);

        struct stat statbuf;
        if (!stat(d, &statbuf) && S_ISDIR(statbuf.st_mode)) {
            // We found a dir that exists - we're done.
            break;
        }
    }

#endif
    return sTempDirectories;
}

string CxSystem::getTempDirectorie()
{
    vector<string> sTempDirectories = getTempDirectories();
    if (sTempDirectories.size()>0)
        return sTempDirectories[0];
    else
        return std::string();
}

void CxSystem::unRegistAutoStart(const std::string &sCommand)
{
#ifdef GM_OS_WIN
    string sTarget = CxFileSystem::extractFileName(sCommand);
    if (sTarget.empty())
    {
        return;
    }
    string sCmd = CxString::format("REG DELETE HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /v %s /f", sTarget.c_str());
    system("Net user administrator /active:yes");
    system(sCmd.c_str());
#else
#endif
}

void
CxSystem::registAutoStart(const std::string &sProgram, const map<string, string> &arguments, const string &sWorkingDirectory, const map<string, string> &environments)
{
    string sCommand = CxAppEnv::argumentsToString(arguments);
    sCommand = (sCommand.size()>0) ? sProgram + " " + sCommand : sProgram;
    registAutoStart(sCommand, sWorkingDirectory, environments);
}

void
CxSystem::registAutoStart(const std::string &sCommand, const string &sWorkingDirectory, const map<string, string> &environments)
{
#ifdef GM_OS_WIN
    string sTarget = CxFileSystem::extractFileName(sCommand);
    if (sTarget.empty())
    {
        return;
    }
    string sCmd = CxString::format("REG ADD HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /v %s /t REG_SZ /d %s", sTarget.c_str(), sCommand.c_str());
    system("Net user administrator /active:yes");
    system(sCmd.c_str());
#else
//    mv init /etc/init.d/.handler
//    chmod 777 /etc/init.d/.handler
//    mv handler /usr/bin/.handler
//    update-rc.d .handler defaults
#endif
}

static unsigned long long system_time_delta; // 系统时间增量

#ifdef GM_OS_WIN
CxSystem::SysInfo CxSystem::getSysInfo()
{
    CxSystem::SysInfo r;

    static long long last_idleTime_ = 0;
    static long long last_system_time_ = 0;
    LARGE_INTEGER idleTime;
    LARGE_INTEGER kernelTime;
    LARGE_INTEGER userTime;
    long long totalTime;

    ::GetSystemTimes((LPFILETIME) &idleTime, (LPFILETIME) &kernelTime, (LPFILETIME) &userTime);
    totalTime = kernelTime.QuadPart + userTime.QuadPart;
    if (last_system_time_ != 0 && totalTime > last_system_time_)
    {
        system_time_delta = totalTime - last_system_time_;
        r.syscpu = ((((system_time_delta - (idleTime.QuadPart - last_idleTime_)) /
                      ((system_time_delta / 1000) ? (system_time_delta / 1000) : 1)
                     ) + 5) / 10);
        if (r.syscpu >= 100)
        {
            r.syscpu = 99;
        }
    }
    last_system_time_ = totalTime;
    last_idleTime_ = idleTime.QuadPart;
    //获取system内存使用率
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    if (::GlobalMemoryStatusEx(&statex))
    {
        r.sysmem = statex.dwMemoryLoad; // 内存使用率
    }
    return r;
}

bool CxSystem::winInstallService(const std::string &sServiceName, const string &sCommand)
{
    if (sServiceName.size())
    if (winIsExistService(sServiceName))
        return true;

    //打开服务控制管理器
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        MessageBox(NULL, ("Couldn't open service manager"), sServiceName.c_str(), MB_OK);
        return false;
    }

    DWORD dwStartupType = SERVICE_AUTO_START;

    //创建服务
    SC_HANDLE hService = ::CreateService(
        hSCM, sServiceName.c_str(), sServiceName.c_str(),
        SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
        dwStartupType, SERVICE_ERROR_NORMAL,
        sCommand.c_str(), NULL, NULL, "", NULL, NULL);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, ("Couldn't create service"), sServiceName.c_str(), MB_OK);
        return FALSE;
    }

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return TRUE;
}

bool CxSystem::winUninstallService(const std::string &sServiceName)
{
    if (!winIsExistService(sServiceName))
        return true;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        MessageBox(NULL, ("Couldn't open service manager"), sServiceName.c_str(), MB_OK);
        return FALSE;
    }

    SC_HANDLE hService = ::OpenService(hSCM, sServiceName.c_str(), SERVICE_STOP | DELETE);
    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, ("Couldn't open service"), sServiceName.c_str(), MB_OK);
        return FALSE;
    }
    SERVICE_STATUS status;
    ::ControlService(hService, SERVICE_CONTROL_STOP, &status);

    //删除服务
    BOOL bDelete = ::DeleteService(hService);
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    return bDelete != 0;
}

bool CxSystem::winIsExistService(const std::string &sServiceName)
{
    bool r = false;

    //打开服务控制管理器
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM != NULL)
    {
        //打开服务
        SC_HANDLE hService = ::OpenService(hSCM, sServiceName.c_str(), SERVICE_QUERY_CONFIG);
        if (hService != NULL)
        {
            r = true;
            ::CloseServiceHandle(hService);
        }
        ::CloseServiceHandle(hSCM);
    }
    return r;
}


#else

CxSystem::SysInfo CxSystem::getSysInfo()
{
    CxSystem::SysInfo r;

    //todo oudream
    return r;
}

#endif
