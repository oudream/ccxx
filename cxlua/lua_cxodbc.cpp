#include "lua_cxodbc.h"

using namespace std;

bool fn_bCxLuaOdbc()
{
    CxLuaManager::registLua(new CxLuaOdbc);
    return true;
}

static bool f_bCxLuaOdbc = fn_bCxLuaOdbc();

static int fn_openDatabase(lua_State * L)
{
	int nTop = lua_gettop(L);
	if (nTop < 2) //最少1个参数
	{
		lua_pop(L, nTop); //清空 
		return 0;
	}
	
	string sDBSource = CxLuaCommon::fromTable(L, 1);   
	string sDBType = CxLuaCommon::fromTable(L, 2);   
		
	//connect to db
	CxDatabase * pDb = CxDatabaseManager::createDatabase(sDBSource, sDBType);	
	if (pDb == NULL)
	{
		lua_pop(L, nTop); //清空 
		return 0;
	}

	map<string, string> sConnectParams = CxString::splitToMap("", CxGlobal::middleChar, CxGlobal::splitChar);
	bool nRet = pDb->openDatabase(sDBSource, sDBType, true, &sConnectParams);	
	if (nRet != true)
	{
		lua_pop(L, nTop); //清空 
		return 0;
	} 		

	lua_pop(L, nTop); //清空 
	lua_pushinteger(L, 1);  //成功		
	return 1;
}

static int fn_execSql(lua_State * L)
{
	int nTop = lua_gettop(L);
	if (nTop < 2) //最少1个参数
	{
		lua_pop(L, nTop); //清空 
		return 0;
	}
	
	string sDBSource = CxLuaCommon::fromTable(L, 1); 	
	CxDatabase * pDb = CxDatabaseManager::findDb(sDBSource);
	if (pDb == NULL)
	{
		lua_pop(L, nTop); //清空 
		return 0;
	}
	
	string sSql = CxLuaCommon::fromTable(L, 2);		
	int nRet = pDb->execSql(sSql);
	if (nRet < 0)
	{
		lua_pop(L, nTop); //清空 
		return 0;
	}		
		 		
	lua_pop(L, nTop); //清空 
	lua_pushinteger(L, 1);  //成功		
	return 1;
}

static int fn_loadSql(lua_State * L)
{
	int nTop = lua_gettop(L);
	if (nTop < 2) //最少1个参数
	{
		lua_pop(L, nTop); //清空 
		return 0;
	}
	
	string sDBSource = CxLuaCommon::fromTable(L, 1); 	
	CxDatabase * pDb = CxDatabaseManager::findDb(sDBSource);
	if (pDb == NULL)
	{
		lua_pop(L, nTop); //清空 
		return 0;
	}
	
	string sSql = CxLuaCommon::fromTable(L, 2);	
	vector<std::vector<std::string> > tRows;
	vector<std::string> tFields;	
	int nRst = pDb->loadSql(sSql, tRows, &tFields);	
	if (nRst < 0)
	{
		lua_pop(L, nTop); //清空 
		return 0;
	}
	
	int nField = tFields.size();
	if (nField == 0)
	{
		lua_pop(L, nTop); //清空 
		return 0;		
	}	

	int nRow = tRows.size();
	if (nRow == 0)
	{
		lua_pop(L, nTop); //清空 
		lua_newtable(L); 
		return 1;	
	}
		
	lua_pop(L, nTop); //清空

	lua_newtable(L);		 	
	for (size_t i=0; i<nRow; ++i)
	{
		lua_newtable(L);				
		std::vector<std::string> tRec = tRows.at(i);				
		for (int j=0; j<nField; j++)
		{			
			string sCntItem = tRec.at(j);
			lua_pushstring(L, sCntItem.c_str());
			lua_rawseti(L, -2, j+1);
		}					
		lua_rawseti(L, -2, i+1);			
	}
	
	return 1;
}

std::string CxLuaOdbc::tableNameImpl()
{
	return std::string("ClOdbc");   
}

std::map<std::string, fn_int_lua_t> CxLuaOdbc::tableMethodsImpl()
{
	map<string, fn_int_lua_t> r;
	r[string("OpenDatabase")] = fn_openDatabase;  
	r[string("ExecSql")] = fn_execSql;  
	r[string("LoadSql")] = fn_loadSql;	
    return r;
}






  