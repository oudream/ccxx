#ifndef LUA_CXFILESYSTEM_H
#define LUA_CXFILESYSTEM_H

#include "lua_base.h"

class CxLuaFileSystem : public CxLuaBase
{
protected:
    std::string tableNameImpl();

    std::map<std::string, fn_int_lua_t> tableMethodsImpl();

};

#endif // LUA_CXFILESYSTEM_H
