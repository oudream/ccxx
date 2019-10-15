#ifndef LUA_CXLUAJSON_H
#define LUA_CXLUAJSON_H

#include "lua_base.h"

class CxLuaJson: public CxLuaBase
{
protected:
    std::string tableNameImpl();

    std::map<std::string, fn_int_lua_t> tableMethodsImpl();
};


#endif // LUA_CXLUAJSON_H



