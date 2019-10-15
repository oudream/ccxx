#include "lua_cxlog.h"

using namespace std;

bool fn_bCxLuaLog()
{
    CxLuaManager::registLua(new CxLuaLog);
    return true;
}

static bool f_bCxLuaLog = fn_bCxLuaLog();

static int fn_printLog(lua_State *L)
{
	int nTop = lua_gettop(L);
	if (nTop < 2) //最少2个参数
	{
		return 0;
	}
	
	//level of log's info
	int nLevel = 1;
	CxLuaCommon::getValue(L, 1, nLevel);
	if ((nLevel < 1) || (nLevel > 5))	
	{
		return 0;
	}
	
	string sInfo;
	for (int i=1; i<nTop; i++)
	{
		if (lua_isboolean(L, i+1)) 
		{
			bool b;
			CxLuaCommon::getValue(L, i+1, b);
			if (b)
			{
				sInfo = sInfo + "true";
			}
			else
			{
				sInfo = sInfo + "false";
			}
		}
		
		else if (lua_isinteger(L, i+1))  
		{
			int nValue;
			CxLuaCommon::getValue(L, i+1, nValue);	
				
			char sValue[64];
			sprintf(sValue, "%d", nValue);
			sInfo = sInfo + sValue;			
		}		
		
		else if (lua_isnumber(L, i+1))  
		{
			double nValue;
			CxLuaCommon::getValue(L, i+1, nValue);	
				
			char sValue[64];
			sprintf(sValue, "%f", nValue);
			sInfo = sInfo + sValue;			
		}
		
		else if (lua_isstring(L, i+1))  
		{
			string sValue;
			CxLuaCommon::getValue(L, i+1, sValue);
			sInfo = sInfo + sValue;	
		}	
				
		//nothing to do 			
	}
	cxLog().level(nLevel) << sInfo;	

	lua_pop(L, nTop); //清空 
	lua_pushnumber(L, 1);	 //succeed in call
	return 1;		
}

std::string CxLuaLog::tableNameImpl()
{
	return std::string("ClLog");   
}

std::map<std::string, fn_int_lua_t> CxLuaLog::tableMethodsImpl()
{
	map<string, fn_int_lua_t> r;
	r[string("printLog")] = fn_printLog;
    return r;
}

