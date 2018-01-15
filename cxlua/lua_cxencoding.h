#ifndef LUA_CXENCODING_H
#define LUA_CXENCODING_H

#include "lua_base.h"

class CxLuaEncode: public CxLuaBase
{
protected:
    std::string tableNameImpl();

    std::map<std::string, fn_int_lua_t> tableMethodsImpl();

};

#endif // LUA_CXENCODING_H
