#include "cxsystem.h"
#include "cxappenv.h"

#ifndef GM_OS_WIN
#include <sys/utsname.h>  // For uname.
#include <sys/stat.h>
#else
#include <tchar.h>
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

static unsigned long long system_time_delta; // time increment

#ifdef GM_OS_WIN

class _DiskInfo {
public:
    std::string volume;             // \\?\Volume{eedc1316-5245-11e6-a9ff-f44d301b437a}
    std::string device;             // \Device\HarddiskVolume5
    std::string type;               // Fixed
    std::vector<std::string> paths; // F:\       //
    std::string label;              // �¼Ӿ�
    std::string serial;             // 2674BF26
    std::string format;             // NTFS
    std::string flags;              // 3E700FF
    int64       totalSize;          // 143588847616
    int64       freeSize;           // 49181224960

    _DiskInfo():totalSize(-1),freeSize(-1){}
    ~_DiskInfo(){}
    inline _DiskInfo(const _DiskInfo &o) :
        volume(o.volume),
        device(o.device),
        type(o.type),
        label(o.label),
        serial(o.serial),
        format(o.format),
        flags(o.flags),
        totalSize(o.totalSize),
        freeSize(o.freeSize)
    {
        for (int i = 0; i < o.paths.size(); ++i)
        {
            this->paths.push_back(o.paths[i]);
        }
    }

    inline _DiskInfo &operator=(const _DiskInfo &o)
    {
        if (this != &o)
        {
            volume = o.volume;
            device = o.device;
            type = o.type;
            label = o.label;
            serial = o.serial;
            format = o.format;
            flags = o.flags;
            totalSize = o.totalSize;
            freeSize = o.freeSize;
            for (int i = 0; i < o.paths.size(); ++i)
            {
                this->paths.push_back(o.paths[i]);
            }
        }
        return *this;
    }

    inline char extractRoot(const std::string & sPath) const
    {
        size_t iIndex = sPath.find(':');
        if (iIndex != std::string::npos && iIndex > 0)
        {
            char c = sPath[iIndex - 1];
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
            {
                return c;
            }
        }
        return 0;
    }

    inline bool hasRoot(const std::string & sPath) const
    {
        char cDrive = extractRoot(sPath);
        if (cDrive != 0)
        {
            for (int i = 0; i < paths.size(); ++i)
            {
                char cDrive2 = extractRoot(paths[i]);
                if (tolower(cDrive) == tolower(cDrive2))
                {
                    return true;
                }
            }
        }
        return false;
    }

};

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
    // get system memory Usage
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    if (::GlobalMemoryStatusEx(&statex))
    {
        r.sysmem = statex.dwMemoryLoad;
    }
    return r;
}

bool CxSystem::winInstallService(const std::string &sServiceName, const string &sCommand)
{
    if (sServiceName.size())
    if (winIsExistService(sServiceName))
        return true;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        MessageBox(NULL, ("Couldn't open service manager"), sServiceName.c_str(), MB_OK);
        return false;
    }

    DWORD dwStartupType = SERVICE_AUTO_START;

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

    BOOL bDelete = ::DeleteService(hService);
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    return bDelete != 0;
}

bool CxSystem::winIsExistService(const std::string &sServiceName)
{
    bool r = false;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM != NULL)
    {
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

/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * *       _getDiskInfos _getDiskInfos _getDiskInfos begin:       * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

// Descriptions of the flags given by GetVolumeInformation
const TCHAR *flag_desc[] = {
    TEXT("Supports Case Sensitive Search"), TEXT("Supports Case Preserved Names"), TEXT("Supports File Compression"), TEXT("Supports Named Streams"),
    TEXT("Preserves ACLs"), TEXT("Read-Only"), TEXT("Supports Sequential Write Once"), TEXT("Supports Encryption"),
    TEXT("Supports Extended Attributes"), TEXT("Supports Hard Links"), TEXT("Supports Object IDs"), TEXT("Supports Open by File ID"),
    TEXT("Supports Reparse Points"), TEXT("Supports Sparse Files"), TEXT("Supports Transactions"), TEXT("Supports USN Journal"),
    TEXT("Supports Unicode on Disk"), TEXT("Is Compressed"), TEXT("Supports Disk Quotas")
};

// The values of the flags given by GetVolumeInformation
const DWORD flag_values[] = {
    0x00000002, 0x00000001, 0x00000010, 0x00040000, 0x00000008, 0x00080000, 0x00100000, 0x00020000, 0x00800000, 0x00400000,
    0x00010000, 0x01000000, 0x00000080, 0x00000040, 0x00200000, 0x02000000, 0x00000004, 0x00008000, 0x00000020
};

// The types of drives given by GetDriveType
const TCHAR *drive_types[] = {
    TEXT("Unknown"), TEXT("Invalid"), TEXT("Removable"), TEXT("Fixed"), TEXT("Remote"), TEXT("CD Drive"), TEXT("RAM Disk"),
};

// The units used by byte_units
const TCHAR *units[] = {
    TEXT("bytes"), TEXT("KB"), TEXT("MB"), TEXT("GB"), TEXT("TB"),
};

// Prints the descriptions of the flags given by GetVolumeInformation
static void print_flags(DWORD flags) {
    DWORD i;
    for (i = 0; i < ARRAYSIZE(flag_values); ++i) {
        if (flags & flag_values[i]) {
            _tprintf(TEXT("  %s\n"), flag_desc[i]);
        }
    }
}

// Returns the size reduced to one of the units bytes, KB, MB, GB, TB
static double byte_size(ULONGLONG bytes) {
    DWORD i;
    ULONGLONG x;
    for (i = 0, x = 1; i < 4; ++i, x *= 1024)
        if (bytes < x*1024) break;
    return ((double)bytes) / x;
}

// Gets the unit for the reduced size given by byte_size
static const TCHAR *byte_unit(ULONGLONG bytes) {
    DWORD i;
    ULONGLONG x;
    for (i = 0, x = 1024; i < 4; ++i, x *= 1024)
        if (bytes < x) break;
    return units[i];
}

std::vector<_DiskInfo> _getDiskInfos()
{
    std::vector<_DiskInfo> diskInfos;

    HANDLE sh;
    TCHAR vname[MAX_PATH+1], dname[MAX_PATH+1], paths[1024], *path, label[MAX_PATH+1], fsname[MAX_PATH+1];
    BOOL first = TRUE;
    DWORD len, count, serial, flags, err;
    ULARGE_INTEGER free, total;

//    _tprintf(TEXT("VolumeInfo Copyright (C) 2010  Jeffrey Bush <jeff@coderforlife.com>\n"));
//    _tprintf(TEXT("This program comes with ABSOLUTELY NO WARRANTY;\n"));
//    _tprintf(TEXT("This is free software, and you are welcome to redistribute it\n"));
//    _tprintf(TEXT("under certain conditions;\n"));
//    _tprintf(TEXT("See http://www.gnu.org/licenses/gpl.html for more details.\n\n"));

    // Get the first volume (handle is used to iterate)
    sh = FindFirstVolume(vname, ARRAYSIZE(vname));
    if (sh == INVALID_HANDLE_VALUE) {
//        _tprintf(TEXT("FindFirstVolume failed: %d\n"), GetLastError());
        return diskInfos;
    }

    // Iterate through all volumes
    while (first || FindNextVolume(sh, vname, ARRAYSIZE(vname))) {
        first = FALSE;

        _DiskInfo diskInfo;

        // Make sure the volume path is valid
        len = _tcslen(vname);
        if (len < 5 || _tcsncmp(vname, TEXT("\\\\?\\"), 4) != 0 || vname[len-1] != '\\') {
//            _tprintf(TEXT("FindFirstVolume/FindNextVolume returned a bad path: %s\n"), vname);
            continue;
        }
//        _tprintf(TEXT("Volume: %s\n"), vname);
        diskInfo.volume = std::string(vname);

        // The device name (which doesn't work with a trailing '\')
        vname[len-1] = 0;
        if (QueryDosDevice(&vname[4], dname, ARRAYSIZE(dname)))
//            _tprintf(TEXT("Device: %s\n"), dname);
            diskInfo.device = std::string(dname);
        else
//            _tprintf(TEXT("Failed to get device name: %d\n"), GetLastError());
            ;
        vname[len-1] = '\\';

        // The drive type
//        _tprintf(TEXT("Type:   %s\n"), drive_types[GetDriveType(vname)]);
        diskInfo.type = std::string(drive_types[GetDriveType(vname)]);

        // Get all the mount paths for a volume (typically C:\, D:\, ...)
        if (GetVolumePathNamesForVolumeName(vname, paths, ARRAYSIZE(paths), &count)) {
//            _tprintf(TEXT("Paths:"));
            for (path = paths; path[0] != 0; path += _tcslen(path) + 1)
                diskInfo.paths.push_back(std::string(path));
//                _tprintf(TEXT("  %s"), path);
//            _tprintf(TEXT("\n"));
        } else
//            _tprintf(TEXT("Failed to get volume paths: %d\n"), GetLastError());
            ;

        // Get lots of information about the volume: label, serial, abilities (flags), and filesystem name
        if (GetVolumeInformation(vname, label, ARRAYSIZE(label), &serial, NULL, &flags, fsname, ARRAYSIZE(fsname))) {
//            _tprintf(TEXT("Label:  %s\n"), label);
//            _tprintf(TEXT("Serial: %X\n"), serial);
//            _tprintf(TEXT("Format: %s\n"), fsname);
//            _tprintf(TEXT("Flags:  %X\n"), flags);
            diskInfo.label = std::string(label);
            diskInfo.serial = CxString::toString(serial);
            diskInfo.format = std::string(fsname);
            diskInfo.flags = CxString::toString(flags);
//            print_flags(flags);
        } else {
//            _tprintf(TEXT("Failed to get volume information: %d\n"), GetLastError());
        }

        // Size information (note: this is influenced by quotas, if they are enabled for the current user)
        if (GetDiskFreeSpaceEx(vname, &free, &total, NULL)) {
//            _tprintf(TEXT("Size:   Total: %13llu [%6.1f %s]\n"), total.QuadPart, byte_size(total.QuadPart), byte_unit(total.QuadPart));
//            _tprintf(TEXT("        Free:  %13llu [%6.1f %s]\n"), free.QuadPart,  byte_size(free.QuadPart),  byte_unit(free.QuadPart));
            diskInfo.totalSize = total.QuadPart;
            diskInfo.freeSize = free.QuadPart;
        } else {
            diskInfo.totalSize = -1;
            diskInfo.freeSize = -1;
//            _tprintf(TEXT("Failed to get volume size information: %d\n"), GetLastError());
        }

        diskInfos.push_back(diskInfo);
//        _tprintf(TEXT("\n"));
    }

    // Report and errors and close the handle
    err = GetLastError();
    FindVolumeClose(sh);
    if (err != ERROR_NO_MORE_FILES) {
//        _tprintf(TEXT("FindNextVolume failed: %d\n"), err);
        return diskInfos;
    }

    return diskInfos;
}

/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * *       _getDiskInfos _getDiskInfos _getDiskInfos end.       * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

int64
CxSystem::getDiskFreeSpace(const std::string& sDisk)
{
    std::vector<_DiskInfo> diskInfos = _getDiskInfos();
    for (int i = 0; i < diskInfos.size(); ++i)
    {
        const _DiskInfo& diskInfo = diskInfos[i];
        if (diskInfo.hasRoot(sDisk))
        {
            return diskInfo.freeSize;
        }
    }
    return -1;
}

int64
CxSystem::getDiskSpace(const std::string& sDisk)
{
    std::vector<_DiskInfo> diskInfos = _getDiskInfos();
    for (int i = 0; i < diskInfos.size(); ++i)
    {
        const _DiskInfo& diskInfo = diskInfos[i];
        if (diskInfo.hasRoot(sDisk))
        {
            return diskInfo.totalSize;
        }
    }
    return -1;
}

vector<CxSystem::DiskInfo>
CxSystem::getDiskInfos()
{
    std::vector<_DiskInfo> diskInfos = _getDiskInfos();
    vector<CxSystem::DiskInfo> r;

    for (int i = 0; i < diskInfos.size(); ++i)
    {
        const _DiskInfo& diskInfo = diskInfos[i];
        CxSystem::DiskInfo info;
        info.name = CxString::join(diskInfo.paths, 0);
        info.totalSize = diskInfo.totalSize;
        info.freeSize = diskInfo.freeSize;
        r.push_back(info);
    }

    return r;
}

#else

CxSystem::SysInfo CxSystem::getSysInfo()
{
    CxSystem::SysInfo r;

    return r;
}

vector<CxSystem::DiskInfo> CxSystem::getDiskInfos()
{
    return vector<CxSystem::DiskInfo>();
}

int64 CxSystem::getDiskFreeSpace(const std::string &sDisk)
{
    return 0;
}

int64 CxSystem::getDiskSpace(const std::string &sDisk)
{
    return 0;
}

#endif
