#include "lua_base.h"

using namespace std;

std::vector<CxLuaBase *> & fn_getCxLuas()
{
    static std::vector<CxLuaBase *> m;
    return m;
}


CxLuaBase::CxLuaBase()
{
}

CxLuaBase::~CxLuaBase()
{
}

const std::vector<CxLuaBase *> & CxLuaManager::cxLuas()
{
    return fn_getCxLuas();
}

void CxLuaManager::registLua(CxLuaBase * oLua)
{
    vector<CxLuaBase *> & cxLuas2 = fn_getCxLuas();
    if (std::find(cxLuas2.begin(), cxLuas2.end(), oLua) == cxLuas2.end())
        cxLuas2.push_back(oLua);
}
void CxLuaManager::loadCxLuas(lua_State *L)
{
    cxLuaOutScriptPrompt() << "loadCxLuas begin:";

    vector<CxLuaBase *> & cxluas2 = fn_getCxLuas();
    for (size_t i = 0; i < cxluas2.size(); ++i)
    {
        CxLuaBase *oCxLua = cxluas2.at(i);
        string sTableName = oCxLua->tableName();
        if (sTableName.empty())
        {
            continue;
        }
        cxLuaOutScriptPrompt() << "  tableName : " << sTableName << ", count: " << oCxLua->tableMethods().size() << " : ...";
        lua_newtable(L);
        const std::map<std::string, fn_int_lua_t> &tableMethods = oCxLua->tableMethods();
        for (std::map<std::string, fn_int_lua_t>::const_iterator it = tableMethods.begin(); it != tableMethods.end(); ++it)
        {
            std::string sName = it->first;
            fn_int_lua_t fn = it->second;
            if (sName.empty() || fn == 0)
            {
                continue;
            }
            CxLuaCommon::setTable(L, sName, fn);
            cxLuaOutScriptPrompt() << "    functionName : " << sName;
        }
        lua_setglobal(L, sTableName.c_str()); 
    }

    cxLuaOutScriptPrompt() << "loadCxLuas end.";
}
