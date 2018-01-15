#include "lua_cxencoding.h"

using namespace std;

bool fn_bCxLuaEncode()
{
    CxLuaManager::registLua(new CxLuaEncode);
    return true;
}

static bool f_bCxLuaEncode = fn_bCxLuaEncode();

static int fn_gb2312ToUtf8(lua_State *L)
{
	int nTop = lua_gettop(L);
	if (nTop < 1) //最少1个参数
	{
		return 0;
	}
	
	string sOrigin = CxLuaCommon::fromTable(L, 1);    	
	CxEncoding tEncode;
	string sCode = tEncode.gb2312ToUtf8(sOrigin);   
	
	lua_pop(L, nTop); //清空 
	lua_pushstring(L, sCode.c_str());	
	return 1;
}

static int fn_utf8ToGb2312(lua_State *L)
{
	int nTop = lua_gettop(L);
	if (nTop < 1) //最少1个参数
	{
		return 0;
	}
	
	string sOrigin = CxLuaCommon::fromTable(L, 1);    	
	CxEncoding tEncode;
	string sCode = tEncode.utf8ToGb2312(sOrigin);   
	
	lua_pop(L, nTop); //清空 
	lua_pushstring(L, sCode.c_str());	
	return 1;
}

static int fn_base64Encode(lua_State * L)
{
	int nTop = lua_gettop(L);
	if (nTop < 1)
	{
		return 0;
	}
	
	string sOrigin = CxLuaCommon::fromTable(L, 1);
	CxEncoding tEncode;
	string sCode = tEncode.base64Encode(sOrigin); 
	
	lua_pop(L, nTop); //清空 
	lua_pushstring(L, sCode.c_str());	
	return 1;	
}

static int fn_base64Decode(lua_State * L)
{
	int nTop = lua_gettop(L);
	if (nTop < 1)
	{
		return 0;
	}
	
	string sOrigin = CxLuaCommon::fromTable(L, 1);
	CxEncoding tEncode;
	string sCode = tEncode.base64Decode(sOrigin); 
	
	lua_pop(L, nTop); //清空 
	lua_pushstring(L, sCode.c_str());	
	return 1;	
}

static int fn_hasChinese(lua_State * L)
{
	int nTop = lua_gettop(L);
	if (nTop < 1)
	{
		return 0;
	}
	
	string sOrigin = CxLuaCommon::fromTable(L, 1);
	CxEncoding tEncode;
	bool nRst = tEncode.hasChinese(sOrigin); 

	lua_pop(L, nTop); //清空 
	if (nRst)
	{
		lua_pushinteger(L, 1);  
	}
	else
	{
		lua_pushinteger(L, 0);
	}		
	return 1;	
}

std::string CxLuaEncode::tableNameImpl()
{
	return std::string("ClEncode");   
}

std::map<std::string, fn_int_lua_t> CxLuaEncode::tableMethodsImpl()
{
	map<string, fn_int_lua_t> r;
	r[string("Gb2312ToUtf8")] = fn_gb2312ToUtf8;
	r[string("Utf8ToGb2312")] = fn_utf8ToGb2312;	
	r[string("Base64Encode")] = fn_base64Encode;
	r[string("Base64Decode")] = fn_base64Decode;	
	r[string("HasChinese")] = fn_hasChinese;
    return r;
}


