#ifndef LUA_CXTIME_H
#define LUA_CXTIME_H

#include "lua_base.h"

class CxLuaTime : public CxLuaBase
{
protected:
    std::string tableNameImpl();

    std::map<std::string, fn_int_lua_t> tableMethodsImpl();

};

#endif // LUA_CXTIME_H
