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

public:
    static bool sendEmailInternal(const char *dest, const char *subject, const char *body);

    static std::vector<std::string> getTempDirectories();

    static std::string getTempDirectorie();

    static void registAutoStart(const std::string & sProgram,
                                const std::map<std::string, std::string> & arguments = std::map<std::string, std::string>(),
                                const std::string & sWorkingDirectory = std::string(),
                                const std::map<std::string, std::string> & environments = std::map<std::string, std::string>());

    static void registAutoStart(const std::string & sCommand,
                                const std::string & sWorkingDirectory = std::string(),
                                const std::map<std::string, std::string> & environments = std::map<std::string, std::string>());

    static void unRegistAutoStart(const std::string & sCommand);

    static SysInfo getSysInfo();

#ifdef GM_OS_WIN
    static bool winInstallService(const std::string &sServiceName, const std::string &sCommand = std::string());

    static bool winUninstallService(const std::string &sServiceName);

    static bool winIsExistService(const std::string & sServiceName);

#endif

};

#endif // CXSYSTEM_H
