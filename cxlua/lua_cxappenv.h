#ifndef LUA_CXAPPENV_H
#define LUA_CXAPPENV_H

#include "lua_base.h"

class GM_CCXX_LUA_API CxLuaAppenv : public CxLuaBase
{
protected:
    std::string tableNameImpl();

    std::map<std::string, fn_int_lua_t> tableMethodsImpl();

};

#endif // LUA_CXAPPENV_H
