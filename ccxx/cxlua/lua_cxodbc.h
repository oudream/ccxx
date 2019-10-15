#ifndef LUA_CXODBC_H
#define LUA_CXODBC_H

#include "lua_base.h"

class CxLuaOdbc: public CxLuaBase
{
protected:
    std::string tableNameImpl();

    std::map<std::string, fn_int_lua_t> tableMethodsImpl();

};

#endif // LUA_CXODBC_H
