#include "lua_cxrapidjson.h"

using namespace std;

bool fn_bCxLuaJson()
{
    CxLuaManager::registLua(new CxLuaJson);
    return true;
}

static bool f_bCxLuaJson = fn_bCxLuaJson();

static int fn_getValue(lua_State * L)
{
	int nTop = lua_gettop(L);
	if (nTop < 2) //最少1个参数
	{
		lua_pop(L, nTop); //清空 
		return 0;
	}
	
	string sFile = CxLuaCommon::fromTable(L, 1); 
	CxSkverJson tJson(sFile);
	if (tJson.empty())
	{
		lua_pop(L, nTop); //清空 
		return 0;
	}

	map<string, string> tConfig;
	map<string, map<string, string> > tMapConfig = tJson.getSectionEntryValues();
	map<string, map<string, string> >::iterator tIter = tMapConfig.begin();
	for (; tIter != tMapConfig.end(); ++tIter)
	{
		tConfig = tIter->second;
	} 
			 	
	std::vector<std::vector<string> > tRows;
	bool nRst = CxLuaCommon::getValue(L, 2, tRows, false);
	int nRow = tRows.size();
	if (nRow == 0)
	{
		lua_pop(L, nTop); //清空 
		return 0;
	}	
	
	lua_pop(L, nTop); //清空 	
	lua_newtable(L);	
	
	int i = 0;
	while (i < nRow)
	{
		std::vector<string> tCntRow = tRows.at(i);
		if (tCntRow.size() != 2)
		{
			i++;
			continue;
		}
		string sSection = tCntRow.at(0);
		string sKey = tCntRow.at(1);		
		string sValue = tConfig[sKey];
		printf("Section=%s, Key=%s, Value=%s\n", 
			sSection.c_str(), sKey.c_str(), sValue.c_str());
		
		lua_pushstring(L, sValue.c_str());
		lua_rawseti(L, -2, i+1); 		
		
		i++;
	}
  
	return 1;		
}

static int fn_setValue(lua_State * L)
{
	int nTop = lua_gettop(L);
	if (nTop < 2) //最少1个参数
	{
		lua_pop(L, nTop); //清空 
		return 0;
	}
	
	string sFile = CxLuaCommon::fromTable(L, 1); 
	CxSkverJson tJson(sFile);
	if (tJson.empty())
	{
		lua_pop(L, nTop); //清空 
		return 0;
	}
		 	
	std::vector<std::vector<string> > tRows;
	bool nRst = CxLuaCommon::getValue(L, 2, tRows, false);
		
	int nRow = tRows.size();
	int i = 0;
	while (i < nRow)
	{
		std::vector<string> tCntRow = tRows.at(i);
		if (tCntRow.size() != 3)
		{
			i++;
			continue;
		}
		string sSection = tCntRow.at(0);
		string sKey = tCntRow.at(1);
		string sValue = tCntRow.at(2);
		printf("Section=%s, Key=%s, Value=%s \n", 
			sSection.c_str(), sKey.c_str(), sValue.c_str());
		tJson.setValue(sSection, sKey, sValue);
		
		i++;
	}
	tJson.save();

	lua_pop(L, nTop); //清空 
	lua_pushnumber(L, 1);	 //succeed in call
	return 1;		
}


/*
//获得数据库配置 
int LoadDbCfg()
{
	g_sMirrorPath = CxApplication::dataPath();
	if (g_sMirrorPath.length() == 0)
	{
		return RTDB_ERROR;
	}	
		
	//load config	
	string sConfigFilePath = CxApplication::configPath() + "/SvrGlobal.json";
	CxSkverJson tJson(sConfigFilePath);
	map<string, string> tConfig;
	map<string, map<string, string> > tMapConfig = tJson.getSectionEntryValues();
	map<string, map<string, string> >::iterator tIter = tMapConfig.begin();
	for (; tIter != tMapConfig.end(); ++tIter)
	{
		tConfig = tIter->second;
	} 

	//connect to db
	string sNodeID = tConfig["ServerId"]; 
	g_sDBSource = tConfig["DBSource"];   

*/

std::string CxLuaJson::tableNameImpl()
{
	return std::string("ClJson");   
}

std::map<std::string, fn_int_lua_t> CxLuaJson::tableMethodsImpl()
{
	map<string, fn_int_lua_t> r;
	r[string("GetValue")] = fn_getValue;
	r[string("SetValue")] = fn_setValue;
    return r;
}










