#ifndef LUA_CXSTRING_H
#define LUA_CXSTRING_H

#include "lua_base.h"

class GM_CCXX_LUA_API CxLuaString : public CxLuaBase
{
protected:
    std::string tableNameImpl();

    std::map<std::string, fn_int_lua_t> tableMethodsImpl();

};

#endif // LUA_CXSTRING_H
