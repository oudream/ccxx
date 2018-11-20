#ifndef CXSYSTEM_H
#define CXSYSTEM_H

#include "cxglobal.h"

class GM_CCXX_CORE_API CxSystem
{
public:
    struct SysInfo {
        unsigned char syscpu; // 0 - 100
        unsigned char sysmem; // 0 - 100
    };

    struct DiskInfo {
        std::string name;               // win32[c:\ d:\] unix[/dev/disk1s1]
        int64       totalSize;          // 143588847616
        int64       freeSize;           // 49181224960
    };

public:
    /**
     * sendEmailInternal
     * @param dest
     * @param subject
     * @param body
     * @return
     */
    static bool sendEmailInternal(const char *dest, const char *subject, const char *body);

    /**
     * getTempDirectories
     * @return
     */
    static std::vector<std::string> getTempDirectories();

    /**
     * getTempDirectorie
     * @return
     */
    static std::string getTempDirectorie();

    /**
     *
     * @param sProgram
     * @param arguments
     * @param sWorkingDirectory
     * @param environments
     */
    static void registAutoStart(const std::string & sProgram,
                                const std::map<std::string, std::string> & arguments = std::map<std::string, std::string>(),
                                const std::string & sWorkingDirectory = std::string(),
                                const std::map<std::string, std::string> & environments = std::map<std::string, std::string>());

    /**
     *
     * @param sCommand
     * @param sWorkingDirectory
     * @param environments
     */
    static void registAutoStart(const std::string & sCommand,
                                const std::string & sWorkingDirectory = std::string(),
                                const std::map<std::string, std::string> & environments = std::map<std::string, std::string>());

    /**
     *
     * @param sCommand
     */
    static void unRegistAutoStart(const std::string & sCommand);

    /**
     * getSysInfo
     * @return syscpu; // 0 - 100 and sysmem; // 0 - 100 , it is person
     */
    static SysInfo getSysInfo();

    /**
     * getDiskInfos : Space. -1 : invalid
     * @return DiskInfos : struct DiskInfo List
     */
    static std::vector<DiskInfo> getDiskInfos();

    /**
     * function getDiskFreeSpace
     * @param sDisk : eg : "c:"
     * @return Disk Free Space. -1 : invalid
     */
    static int64 getDiskFreeSpace(const std::string& sDisk);

    /**
    * function getDiskFreeSpace
    * @param sDisk : eg : "c:"
    * @return Disk total Space. -1 : invalid
    */
   static int64 getDiskSpace(const std::string& sDisk);

#ifdef GM_OS_WIN
    /**
     *
     * @param sServiceName
     * @param sCommand
     * @return
     */
    static bool winInstallService(const std::string &sServiceName, const std::string &sCommand = std::string());

    /**
     *
     * @param sServiceName
     * @return
     */
    static bool winUninstallService(const std::string &sServiceName);

    /**
     *
     * @param sServiceName
     * @return
     */
    static bool winIsExistService(const std::string & sServiceName);

#endif

};

#endif // CXSYSTEM_H
