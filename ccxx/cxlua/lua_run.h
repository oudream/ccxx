#ifndef LUA_RUN
#define LUA_RUN

#include "lua_common.h"

class GM_CCXX_LUA_API CxLuaRun
{
public:
    static void registInitFunction(fn_void_t fn);

    static void init();

    static lua_State * luaState();

    static void start();

    static void stop();

    static std::string scriptPath();

//sFilePath is xml style :
/*
<?xml version="1.0" encoding="utf-8"?>
<xxx>
    <script>
        <a>
            <b>
                <prop ssid="0" port=5562/>
            </b>
        </a>
    </script>
</xxx>
*/
    static void update(const std::string & sFilePath);

    //pData is xml style
    static void update(const char * pData, int iLength);

    static void update(const std::vector<std::map<std::string, std::string> > & rows);

    static void update(const std::map<std::string, std::string> & row);

    static void update( int ssid, int iPort );

    static std::vector<std::string> reportSelf();

public:
    static int interinfoIn_line( const std::string & sInfo, const msepoch_t & dtIn, int iSource, int eInType, int iTag);

    static int interinfoIn_cmd( const std::string & sCommand, const std::map<std::string, std::string> & sParams, const msepoch_t & dtIn, int iSource, int eInType, int iTag);

    //timer load deal function
    static void timer_timeout(int iInterval);

    static void loadLuaFiles(bool bLocalCommon, bool bLocadTask);

    static void registLuaFileLoaded(fn_void_pchar_t fn);

private:
    static std::vector<fn_void_t> * getFnInits();

    static std::vector<fn_void_pchar_t> * getFnLuaFileLoadeds();

    static int beforeLoadLuaFile(const std::string & sFilePath);

    static int afterLoadLuaFile(const std::string & sFilePath, int iResultLoad, const std::string & sResultString);

};

#endif // LUA_RUN
