#ifndef LUA_CXLUALOG_H
#define LUA_CXLUALOG_H

#include "lua_base.h"

class GM_CCXX_LUA_API CxLuaLog : public CxLuaBase
{
protected:
    std::string tableNameImpl();

    std::map<std::string, fn_int_lua_t> tableMethodsImpl();
};


#endif // LUA_CXLUALOG_H



