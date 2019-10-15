#include "lua_cxtime.h"

using namespace std;

bool fn_bCxLuaTime()
{
    CxLuaManager::registLua(new CxLuaTime);
    return true;
}

static bool f_bCxLuaTime = fn_bCxLuaTime();

static int fn_getTimeString(lua_State *L)
{
    string sGetTimeString = CxTime::currentSystemTimeString(0,0,0);
    lua_pushstring(L, sGetTimeString.c_str());
    return 1;
}

static int fn_getCurrentTimeMS(lua_State *L)
{
    msepoch_t tmCurr = CxTime::currentSystemTime();
    lua_pushinteger(L, tmCurr);
    return 1;
}
static int fn_getCurrentTimeS(lua_State *L)
{
    sepoch_t tmCurr = CxTime::currentSepoch();
    lua_pushinteger(L, tmCurr);
    return 1;
}

std::string CxLuaTime::tableNameImpl()
{
    return std::string("cx_time");
}

std::map<std::string, fn_int_lua_t> CxLuaTime::tableMethodsImpl()
{
    map<string, fn_int_lua_t> r;
    r[string("getTimeString")] = fn_getTimeString;
    r[string("getCurrentTimeMS")] = fn_getCurrentTimeMS;
    r[string("getCurrentTimeS")] = fn_getCurrentTimeS;
    return r;
}
