#include "lua_cxappenv.h"

using namespace std;

bool fn_bCxLuaApplication()
{
    CxLuaManager::registLua(new CxLuaAppenv);
    return true;
}

static bool f_bCxLuaApplication = fn_bCxLuaApplication();

static int fn_getApplicationPath(lua_State *L)
{
    string sApplicationPath = CxApplication::applicationPath();
    lua_pushstring(L, sApplicationPath.c_str());
    return 1;
}

static int fn_getApplicationFileName(lua_State *L)
{
    string sApplicationFileName = CxApplication::applicationFileName();
    lua_pushstring(L, sApplicationFileName.c_str());
    return 1;
}

static int fn_getApplicationTargetName(lua_State *L)
{
    string sApplicationTargetName = CxApplication::applicationTargetName();
    lua_pushstring(L, sApplicationTargetName.c_str());
    return 1;
}

static int fn_getConfigPath(lua_State *L)
{
	string sConfigPath = CxApplication::configPath();
    lua_pushstring(L, sConfigPath.c_str());
    return 1;		
}

static int fn_getConfigFilePath(lua_State *L)
{
	string sConfigFilePath = CxApplication::configFilePath();
    lua_pushstring(L, sConfigFilePath.c_str());
    return 1;
}

static int fn_getLogPath(lua_State *L)
{
	string sLogPath = CxApplication::logPath();
    lua_pushstring(L, sLogPath.c_str());
    return 1;
}
	
static int fn_getTempPath(lua_State *L)
{
	string sTempPath = CxApplication::tempPath();
    lua_pushstring(L, sTempPath.c_str());
    return 1;
}

static int fn_getDataPath(lua_State *L)
{
	string sDataPath = CxApplication::dataPath();
    lua_pushstring(L, sDataPath.c_str());
    return 1;
}

static int fn_getApplicationConfig(lua_State *L)
{
    int n = lua_gettop(L);
    string sSection;
    string sKey;
    string tDefault;

    bool bOk = true;
    if (n>=1)
    {
        bOk = CxLuaCommon::getValue(L,1,sSection);
    }
    if (n>=2)
    {
        bOk = CxLuaCommon::getValue(L,2,sKey);
    }
    if (n>=3)
    {
        bOk = CxLuaCommon::getValue(L,3,tDefault);
    }

    if (!bOk) return false;
    string sApplicationConfig = CxApplication::findConfig(sSection, sKey, tDefault);
    lua_pushstring(L, sApplicationConfig.c_str());
    return 1;
}

std::string CxLuaAppenv::tableNameImpl()
{
    return std::string("ClApp");
}

std::map<std::string, fn_int_lua_t> CxLuaAppenv::tableMethodsImpl()
{
    std::map<std::string, fn_int_lua_t> r;
    r[string("GetAppPath")] = fn_getApplicationPath;
    r[string("GetAppFileName")] = fn_getApplicationFileName;
    r[string("GetAppTargetName")] = fn_getApplicationTargetName;        
    r[string("GetConfigPath")] = fn_getConfigPath;
    r[string("GetConfigFilePath")] = fn_getConfigFilePath;
    r[string("GetLogPath")] = fn_getLogPath;
    r[string("GetTempPath")] = fn_getTempPath;
    r[string("GetDataPath")] = fn_getDataPath;        
    r[string("GetAppConfig")] = fn_getApplicationConfig;        
    return r;
}




