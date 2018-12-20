#ifndef LUA_CXENCODING_H
#define LUA_CXENCODING_H

#include "lua_base.h"

class GM_CCXX_LUA_API CxLuaEncode : public CxLuaBase
{
protected:
    std::string tableNameImpl();

    std::map<std::string, fn_int_lua_t> tableMethodsImpl();

};

#endif // LUA_CXENCODING_H
