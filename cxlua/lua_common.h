#ifndef LUA_COMMON_H
#define LUA_COMMON_H


//#ifndef LUA_BUILD_AS_DLL
//#define LUA_BUILD_AS_DLL
//#endif

// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++
#include <lua-5.3.4/lua.hpp>

#include <map>
#include <string>
#include <iostream>
#include <ccxx/ccxx.h>

using namespace std;

const std::string CS_Lua_Succed = "succed";
const std::string CS_Lua_Count  = "n";
const std::string CS_Lua_Type   = "tp";
const std::string CS_Lua_Value  = "v";
const std::string CS_Lua_Empty  = "empty";
	

typedef int (*fn_int_lua_t)(lua_State *);

 
#if defined(GM_BUILD_CCXX_LUA_LIB)
#define GM_CCXX_LUA_API  GM_DECL_EXPORT
#elif defined(GM_BUILD_CCXX_LUA_CODE)
#define GM_CCXX_LUA_API
#else
#define GM_CCXX_LUA_API  GM_DECL_IMPORT
// GM_BUILD_CCXX_LUA_INCLUDE
#endif


class GM_CCXX_LUA_API CxLuaOutInfo
{
public:
    static inline CxInterinfoOutStream outScriptPrompt() {
		CxInterinfoOutStream r; r.noend(); r.type(CxInterinfo::Type_Script); r << CxGlobal::promptString << " Script : "; return r;
    }

#if defined(GM_DEBUG)
    static inline CxInterinfoOutStream outScriptDebug() {
        CxInterinfoOutStream r; r.noend(); r.type(CxInterinfo::Type_Script); r << CxGlobal::debugString << " Script : "; return r;
    }
#else
#undef outScriptDebug
    static inline CxInterinfoNoOutStream outScriptDebug() { return CxInterinfoNoOutStream(); }
#define CX_NO_outScriptDebug_MACRO while (false) outScriptDebug
#define outScriptDebug CX_NO_outScriptDebug_MACRO
#endif

};


class GM_CCXX_LUA_API CxLuaCommon
{
public:
    //*dofile : run lua file
    static bool runScript(lua_State *L , const std::string & sFilePath);

public:
    static void stackDump (lua_State *L);

    //*set value by table[idxTable=-1]
    static void setTable (lua_State *L , const std::string & sKey, bool value, const std::string & sTable = std::string());

    static void setTable (lua_State *L , const std::string & sKey, int value, const std::string & sTable = std::string());

    static void setTable (lua_State *L , const std::string & sKey, long long value, const std::string & sTable = std::string());

    static void setTable (lua_State *L , const std::string & sKey, double value, const std::string & sTable = std::string());

    static void setTable (lua_State *L , const std::string & sKey, const std::string & value, const std::string & sTable = std::string());

    static void setTable (lua_State *L , const std::string & sKey, lua_CFunction value, const std::string & sTable = std::string());

    //*get value by table[idxTable]
    static bool getTable (lua_State *L , const std::string & sKey, bool & value, int idxTable = 0, const std::string & sTable = std::string());

    static bool getTable (lua_State *L , const std::string & sKey, int & value, int idxTable = 0, const std::string & sTable = std::string());

    static bool getTable (lua_State *L , const std::string & sKey, long long & value, int idxTable = 0, const std::string & sTable = std::string());

    static bool getTable (lua_State *L , const std::string & sKey, double & value, int idxTable = 0, const std::string & sTable = std::string());

    static bool getTable (lua_State *L , const std::string & sKey, std::string & value, int idxTable = 0, const std::string & sTable = std::string());

    static bool getTable (lua_State *L , const std::string & sKey, lua_CFunction & value, int idxTable = 0, const std::string & sTable = std::string());

    //*get value by table[idxTable]
    static bool getTable (lua_State *L , int idxKey, bool & value, int idxTable = 0, const std::string & sTable = std::string());

    static bool getTable (lua_State *L , int idxKey, int & value, int idxTable = 0, const std::string & sTable = std::string());

    static bool getTable (lua_State *L , int idxKey, long long & value, int idxTable = 0, const std::string & sTable = std::string());

    static bool getTable (lua_State *L , int idxKey, double & value, int idxTable = 0, const std::string & sTable = std::string());

    static bool getTable (lua_State *L , int idxKey, std::string & value, int idxTable = 0, const std::string & sTable = std::string());

    static bool getTable (lua_State *L , int idxKey, lua_CFunction & value, int idxTable = 0, const std::string & sTable = std::string());

    //*get value by top
    static bool getValue(lua_State *L , int idx, bool& b);

    static bool getValue(lua_State *L , int idx, int& i);

    static bool getValue(lua_State *L , int idx, uint32& n);

    static bool getValue(lua_State *L , int idx, int64& n);

    static bool getValue(lua_State *L , int idx, uint64& n);

    static bool getValue(lua_State *L , int idx, double& d);

    static bool getValue(lua_State *L , int idx, float& f);

    static bool getValue(lua_State *L , int idx, std::string& s, bool bCString = false);

    static bool getValue(lua_State *L , int idx, std::vector<std::string>& ss, bool bCString = false);

    static bool getValue(lua_State *L , int idx, std::vector<std::vector<std::string> >& sRows, bool bCString = false);

    //*call lua function
    static bool callLuaFunction(lua_State *L , int iParamCount, int iResultCount);

    static bool callLuaFunction(lua_State *L , int iParamCount, int iResultCount, int & iResult);

    static bool callLuaFunction(lua_State *L , int iParamCount, int iResultCount, std::string & sResult);

    static void stringToTable(lua_State * L,const std::string &data);

    static void mapToTable(lua_State * L, const std::map< std::string, std::string> &data);

    static std::string fromTable(lua_State * L,int idx);

};


#endif // LUA_COMMON_H
