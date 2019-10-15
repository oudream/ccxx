#include "lua_cxinterinfo.h"


using namespace std;

std::string CxLuaInterInfo::tableNameImpl()
{
    return std::string();
}

std::map<std::string, fn_int_lua_t> CxLuaInterInfo::tableMethodsImpl()
{
    return std::map<std::string, fn_int_lua_t>();
}
