#ifndef LUA_BASE_H
#define LUA_BASE_H

#include "lua_common.h"



class GM_CCXX_LUA_API CxLuaBase
{
public:
    CxLuaBase();

    ~CxLuaBase();

    std::string tableName() {
        return tableNameImpl();
    }

    std::map<std::string, fn_int_lua_t> tableMethods() {
        return tableMethodsImpl();
    };

protected:
    virtual std::string tableNameImpl() = 0;

    virtual std::map<std::string, fn_int_lua_t> tableMethodsImpl() = 0;

};

class CxLuaManager
{
public:
    static const std::vector<CxLuaBase *> & cxLuas();

    static void registLua(CxLuaBase *);

    static void loadCxLuas(lua_State * L);

};

#endif // LUA_BASE_H
