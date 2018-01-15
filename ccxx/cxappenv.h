#ifndef PROJECT_CXAPPENV_H
#define PROJECT_CXAPPENV_H

#include "cxstring.h"
#include "cxfile.h"

static const std::string CS_SectionProgramConfig = "ProgramConfigs";
static const std::string CS_EntryProcessEventsSleep = "ProcessEventsSleep";

#ifndef GM_PROJECT_TYPE
#define GM_PROJECT_TYPE (1)
#endif

#define GM_PROJECT_TYPE_APP_CONSOLE   (1)
#define GM_PROJECT_TYPE_APP_GUI       (2)
#define GM_PROJECT_TYPE_LIB_DLL       (3)
#define GM_PROJECT_TYPE_LIB_STATIC    (4)

class GM_CCXX_CORE_API CxAppEnv {
public:
    class GM_CCXX_CORE_API CommandLineArguments {
    public:
        CommandLineArguments(int argc, const char * argv[]);
        ~CommandLineArguments();

        void push(const std::string & sKey, const std::string & sValue);

        int argc;

        const char * * argv;

    private:
        std::vector<std::string> _args;
        std::vector<const char *> _argPchars;

    private:
        void updateArgs();

    };

public:
    static void init(int argc, const char * argv[]);

    static const std::string& applicationFilePath();

    static const std::string& applicationFileName();

    static const std::string& applicationTargetName();

    static const std::string& applicationPath();

    static const std::string& applicationDeployPath();

    static std::string applicationDeployPathMerge(const std::string & sMerge);

    static void setApplicationDefaultDeployPath(const std::string& value);

    static const std::string& configPath();

    static const std::string& configDefaultFilePath();

    static const std::string& configFilePath();

    static const std::string& configExtendFilePath();

    static const std::string& logPath();

    static const std::string& tempPath();

    static const std::string& dataPath();

    static const std::vector<std::string> & argumentList();

    static const std::map<std::string, std::string>& arguments();

    static std::string argumentsString();

    //sArgumentName: std::string("name1")
    static std::string findArgument(const std::string& sArgumentName);

    //sArgumentName: std::string("name1")
    static bool containArgument(const std::string& sArgumentName);

    //sArguments : std::map<std::string, std::string> sArguments; sArguments["-name1"] = "value1"; sArguments["-name2"] = "value2";
    static std::string argumentsToString(const std::map<std::string, std::string>& sArguments);

    //sArg : "-name1 value1 value2 -name2 value3"
    static std::map<std::string, std::string> argumentsFromString(const std::string & sArg);

    static std::map<std::string, std::string> argumentsFromArgcv(int argc, const char *argv[]);

    //*part : application config
    static const std::map<std::string, std::map<std::string, std::string> > & getConfigsData();

    //*part : application config
    static const std::vector<std::string> & getConfigSectionNames();

    static const std::map<std::string, std::string> & findConfigs(const std::string & sSection);

    template<typename TValue>
    static TValue findConfig(const std::string & sSection, const std::string & sKey, const TValue & tDefault, bool bSaveNew = false)
    {
        std::string sDefault = CxString::toString(tDefault);
        std::string sValue = doFindConfig(sSection, sKey, sDefault, bSaveNew);
        return CxString::fromString(sValue, tDefault);
    }

    /**
     *
     * @param sFilePath
     * @return
     */
    static int saveConfig(const std::string & sFilePath = std::string());

    static void saveConfig(const std::string &sSection, const std::string &sKey, const std::string &sValue);

    /**
     * check app single instance
     * @param argc
     * @param argv
     * @return
     */
    static bool singleInstance(int argc, const char *argv[]);

private:
    static std::map<std::string, std::map<std::string, std::string> > toRelativePath(const std::map<std::string, std::map<std::string, std::string> > & sections);

    static const std::string & doFindConfig(const std::string & sSection, const std::string & sKey, const std::string & sDefault = std::string(), bool bSaveNew = true);

    static void updateConfigSectionNames();

    static void argumentSaveToConfig();

};

#endif //PROJECT_CXAPPENV_H
