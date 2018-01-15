#include "lua_common.h"

//using namespace std;



bool CxLuaCommon::runScript(lua_State *L, const string &sFilePath)
{
    assert(L != NULL);

    int iResult = luaL_dofile(L, sFilePath.c_str());
    if( iResult != 0 )
    {
        string sError = CxString::format(" luaL_dofile[%s] failed : [%s]", sFilePath.c_str(), lua_tostring(L,-1));
        CxLuaOutInfo::outScriptPrompt() << CxTime::currentSepochString() << sError;
        return false;
    }
    else
    {
        string sInfo = CxString::format(" luaL_dofile[%s] success !!!", sFilePath.c_str());
        CxLuaOutInfo::outScriptDebug() << CxTime::currentSepochString() << sInfo;
    }
    return true;
}

void CxLuaCommon::stackDump(lua_State *L)
{
    int i;

    int top = lua_gettop(L);

    printf("\nlua_gettop - count=%d : \n", top);

    for (i = 1; i <= top; i++) {  /* repeat for each level */

        int t = lua_type(L, i);

        printf("%03d : typeid=%d   typename=%s   value=", i, t, lua_typename(L, i));

        switch (t) {

        case LUA_TSTRING:  /* strings */

            printf("`%s'", lua_tostring(L, i));

            break;

        case LUA_TBOOLEAN:  /* booleans */

            printf(lua_toboolean(L, i) ? "true" : "false");

            break;

        case LUA_TNUMBER:  /* numbers */

            printf("%g", lua_tonumber(L, i));

            break;

        default:  /* other values */

            printf("%s", lua_typename(L, t));

            break;

        }

        printf("  \n");  /* put a separator */
    }

    printf("\n");     /* end the listing */
}


void CxLuaCommon::setTable (lua_State *L , const string & sKey, bool value, const string & sTable)
{
    if (sTable.empty())
    {
        lua_pushstring(L, sKey.c_str());
        lua_pushboolean(L, value);
        lua_settable(L, -3);
    }
    else
    {

    }
}

void CxLuaCommon::setTable (lua_State *L , const string & sKey, int value, const string & sTable)
{
    if (sTable.empty())
    {
        lua_pushstring(L, sKey.c_str());
        lua_pushinteger(L, value);
        lua_settable(L, -3);
    }
    else
    {

    }
}

void CxLuaCommon::setTable(lua_State *L, const string &sKey, long long value, const string &sTable)
{
    if (sTable.empty())
    {
        lua_pushstring(L, sKey.c_str());
        lua_pushinteger(L, value);
        lua_settable(L, -3);
    }
    else
    {

    }
}

void CxLuaCommon::setTable (lua_State *L , const string & sKey, double value, const string & sTable)
{
    if (sTable.empty())
    {
        lua_pushstring(L, sKey.c_str());
        lua_pushnumber(L, value);
        lua_settable(L, -3);
    }
    else
    {

    }
}

void CxLuaCommon::setTable (lua_State *L , const string & sKey, const string & value, const string & sTable)
{
    if (sTable.empty())
    {
        lua_pushstring(L, sKey.c_str());
        lua_pushlstring(L, value.data(), value.size());
        lua_settable(L, -3);
    }
    else
    {

    }
}

void CxLuaCommon::setTable (lua_State *L , const string & sKey, lua_CFunction value, const string & sTable)
{
    if (sTable.empty())
    {
        lua_pushstring(L, sKey.c_str());
        lua_pushcfunction(L, value);
        lua_settable(L, -3);
    }
    else
    {

    }
}












bool CxLuaCommon::getTable(lua_State *L, const string &sKey, bool &value, int idxTable, const string &sTable)
{
    int bIs = FALSE;
    if (sTable.empty())
    {
        int idx = idxTable;
        if (idx == 0) idx = -1;
        int t = lua_getfield(L, idx, sKey.c_str());
        if (t == LUA_TBOOLEAN)
        {
            value = lua_toboolean(L, -1);
            bIs = TRUE;
        }
        //保证 idxTable 不变
        if (idx < 0)
        {
            lua_pop(L, 1);
        }
    }
    else
    {

    }
    return bIs;
}

bool CxLuaCommon::getTable(lua_State *L, const string &sKey, int &value, int idxTable, const string &sTable)
{
    int bIs = FALSE;
    if (sTable.empty())
    {
        int idx = idxTable;
        if (idx == 0) idx = -1;
        int t = lua_getfield(L, idx, sKey.c_str());
        if (t != LUA_TNIL)
        {
            lua_Integer v = lua_tointegerx(L, -1, &bIs);
            if (bIs)
            {
                value = v;
            }
        }
        //保证 idxTable 不变
        if (idx < 0)
        {
            lua_pop(L, 1);
        }
    }
    else
    {
    }
    return TRUE;
}

bool CxLuaCommon::getTable(lua_State *L, const string &sKey, long long &value, int idxTable, const string &sTable)
{
    int bIs = FALSE;
    if (sTable.empty())
    {
        int idx = idxTable;
        if (idx == 0) idx = -1;
        int t = lua_getfield(L, idx, sKey.c_str());
        if (t != LUA_TNIL)
        {
            lua_Integer v = lua_tointegerx(L, -1, &bIs);
            if (bIs)
            {
                value = v;
            }
        }
        //保证 idxTable 不变
        if (idx < 0)
        {
            lua_pop(L, 1);
        }
    }
    else
    {
    }
    return bIs;
}

bool CxLuaCommon::getTable(lua_State *L, const string &sKey, double &value, int idxTable, const string &sTable)
{
    int bIs = FALSE;
    if (sTable.empty())
    {
        int idx = idxTable;
        if (idx == 0) idx = -1;
        int t = lua_getfield(L, idx, sKey.c_str());
        if (t != LUA_TNIL)
        {
            lua_Number v = lua_tonumberx(L, -1, &bIs);
            if (bIs)
            {
                value = v;
            }
        }
        //保证 idxTable 不变
        if (idx < 0)
        {
            lua_pop(L, 1);
        }
    }
    else
    {
    }
    return bIs;
}

bool CxLuaCommon::getTable(lua_State *L, const string &sKey, string &value, int idxTable, const string &sTable)
{
    int bIs = FALSE;
    if (sTable.empty())
    {
        int idx = idxTable;
        if (idx == 0) idx = -1;
        int t = lua_getfield(L, idx, sKey.c_str());
        if (t != LUA_TNIL)
        {
            size_t iParam = 0;
            const char * pParam = lua_tolstring(L, -1, & iParam);
            if(pParam)
            {
                value = string(pParam, iParam);
                bIs = TRUE;
            }
        }
        //保证 idxTable 不变
        if (idx < 0)
        {
            lua_pop(L, 1);
        }
    }
    else
    {
    }
    return bIs;
}

bool CxLuaCommon::getTable(lua_State *L, const string &sKey, lua_CFunction & value, int idxTable, const string &sTable)
{
    int bIs = FALSE;
    if (sTable.empty())
    {
        int idx = idxTable;
        if (idx == 0) idx = -1;
        int t = lua_getfield(L, idx, sKey.c_str());
        if (t == LUA_TFUNCTION)
        {
            value = lua_tocfunction(L, -1);
            bIs = (value != NULL);
        }
        //保证 idxTable 不变
        if (idx < 0)
        {
            lua_pop(L, 1);
        }
    }
    else
    {
    }
    return bIs;
}




















bool CxLuaCommon::getTable(lua_State *L, int idxField, bool &value, int idxTable, const string &sTable)
{
    int bIs = FALSE;
    if (sTable.empty())
    {
        int idx = idxTable;
        if (idx == 0) idx = -1;
        int t = lua_rawgeti(L, idx, idxField);
        if (t == LUA_TBOOLEAN)
        {
            value = lua_toboolean(L, -1);
            bIs = TRUE;
        }
        //保证 idxTable 不变
        if (idx < 0)
        {
            lua_pop(L, 1);
        }
    }
    else
    {

    }
    return bIs;
}

bool CxLuaCommon::getTable(lua_State *L, int idxField, int &value, int idxTable, const string &sTable)
{
    int bIs = FALSE;
    if (sTable.empty())
    {
        int idx = idxTable;
        if (idx == 0) idx = -1;
        int t = lua_rawgeti(L, idx, idxField);
        if (t != LUA_TNIL)
        {
            lua_Integer v = lua_tointegerx(L, -1, &bIs);
            if (bIs)
            {
                value = v;
            }
        }
        //保证 idxTable 不变
        if (idx < 0)
        {
            lua_pop(L, 1);
        }
    }
    else
    {
    }
    return bIs;
}

bool CxLuaCommon::getTable(lua_State *L, int idxField, long long &value, int idxTable, const string &sTable)
{
    int bIs = FALSE;
    if (sTable.empty())
    {
        int idx = idxTable;
        if (idx == 0) idx = -1;
        int t = lua_rawgeti(L, idx, idxField);
        if (t != LUA_TNIL)
        {
            lua_Integer v = lua_tointegerx(L, -1, &bIs);
            if (bIs)
            {
                value = v;
            }
        }
        //保证 idxTable 不变
        if (idx < 0)
        {
            lua_pop(L, 1);
        }
    }
    else
    {
    }
    return bIs;
}

bool CxLuaCommon::getTable(lua_State *L, int idxField, double &value, int idxTable, const string &sTable)
{
    int bIs = FALSE;
    if (sTable.empty())
    {
        int idx = idxTable;
        if (idx == 0) idx = -1;
        int t = lua_rawgeti(L, idx, idxField);
        if (t != LUA_TNIL)
        {
            int bIs;
            lua_Number v = lua_tonumberx(L, -1, &bIs);
            if (bIs)
            {
                value = v;
            }
        }
        //保证 idxTable 不变
        if (idx < 0)
        {
            lua_pop(L, 1);
        }
    }
    else
    {
    }
    return bIs;
}

bool CxLuaCommon::getTable(lua_State *L, int idxField, string &value, int idxTable, const string &sTable)
{
    int bIs = FALSE;
    if (sTable.empty())
    {
        int idx = idxTable;
        if (idx == 0) idx = -1;
        int t = lua_rawgeti(L, idx, idxField);
        if (t != LUA_TNIL)
        {
            size_t iParam = 0;
            const char * pParam = lua_tolstring(L, -1, & iParam);
            if(pParam)
            {
                value = string(pParam, iParam);
                bIs = TRUE;
            }
        }
        //保证 idxTable 不变
        if (idx < 0)
        {
            lua_pop(L, 1);
        }
    }
    else
    {
    }
    return bIs;
}

bool CxLuaCommon::getTable(lua_State *L, int idxField, lua_CFunction & value, int idxTable, const string &sTable)
{
    int bIs = FALSE;
    if (sTable.empty())
    {
        int idx = idxTable;
        if (idx == 0) idx = -1;
        int t = lua_rawgeti(L, idx, idxField);
        if (t == LUA_TFUNCTION)
        {
            value = lua_tocfunction(L, -1);
            bIs = (value != NULL);
        }
        //保证 idxTable 不变
        if (idx < 0)
        {
            lua_pop(L, 1);
        }
    }
    else
    {
    }
    return bIs;
}











bool CxLuaCommon::getValue(lua_State *L , int idx, bool &b)
{
    if (lua_isboolean(L, idx))
    {
        b = lua_toboolean(L, idx);
        return true;
    }
    return false;
}

bool CxLuaCommon::getValue(lua_State *L , int idx, int& i)
{
    int bIs;
    lua_Integer value = lua_tointegerx(L, idx, &bIs);
    if (bIs)
    {
        i = value;
        return true;
    }
    return false;
}

bool CxLuaCommon::getValue(lua_State *L , int idx, uint32& n)
{
    int bIs;
    lua_Integer value = lua_tointegerx(L, idx, &bIs);
    if (bIs)
    {
        n = value;
        return true;
    }
    return false;
}

bool CxLuaCommon::getValue(lua_State *L , int idx, int64& n)
{
    int bIs;
    lua_Integer value = lua_tointegerx(L, idx, &bIs);
    if (bIs)
    {
        n = value;
        return true;
    }
    return false;
}

bool CxLuaCommon::getValue(lua_State *L , int idx, uint64& n)
{
    int bIs;
    lua_Integer value = lua_tointegerx(L, idx, &bIs);
    if (bIs)
    {
        n = value;
        return true;
    }
    return false;
}

bool CxLuaCommon::getValue(lua_State *L , int idx, double& d)
{
    int bIs;
    lua_Number value = lua_tonumberx(L, idx, &bIs);
    if (bIs)
    {
        d = value;
        return true;
    }
    return false;
}

bool CxLuaCommon::getValue(lua_State *L , int idx, float& f)
{
    int bIs;
    lua_Number value = lua_tonumberx(L, idx, &bIs);
    if (bIs)
    {
        f = value;
        return true;
    }
    return false;
}

bool CxLuaCommon::getValue(lua_State *L , int idx, std::string& s, bool bCString)
{
    size_t iParam = 0;
    const char * pParam = lua_tolstring(L, idx, & iParam);
    if(pParam)
    {
        if (bCString)
            s = pParam;
        else
            s = string(pParam, iParam);
        return true;
    }
    return false;
}

bool CxLuaCommon::getValue(lua_State *L, int idx, std::vector<string> &ss, bool bCString)
{
    int iTop = lua_gettop(L);
    if (iTop < 1) return false;
    int t = lua_type(L, idx);
    if (t == LUA_TTABLE)
    {
        size_t iCount = lua_rawlen(L, idx);
        for (size_t i = 1; i <= iCount; ++i)
        {
            t = lua_rawgeti(L, idx, i);
            if (t != LUA_TNIL)
            {
                size_t iParam = 0;
                const char * pParam = lua_tolstring(L, -1, & iParam);
                if(pParam)
                {
                    if (bCString)
                        ss.push_back(string(pParam));
                    else
                        ss.push_back(string(pParam, iParam));
                }
            }
            //保证 idx 不变
            lua_pop(L, 1);
        }
    }
    lua_settop(L, iTop);
    return true;
}

bool CxLuaCommon::getValue(lua_State *L, int idx, std::vector<std::vector<string> > &sRows, bool bCString)
{
    int iTop = lua_gettop(L);
    if (iTop < 1) return false;
    int t = lua_type(L, idx);
    if (t == LUA_TTABLE)
    {
        size_t iParam = 0;
        const char * pParam = NULL;
        size_t iRowCount = lua_rawlen(L, idx);
        for (size_t i = 1; i <= iRowCount; ++i)
        {
            t = lua_rawgeti(L, idx, i);
            if (t == LUA_TTABLE)
            {
                std::vector<string> sRow;
                size_t iColumnCount = lua_rawlen(L, -1);
                for (size_t j = 0; j <= iColumnCount; ++j)
                {
                    t = lua_rawgeti(L, -1, j);
                    if (t != LUA_TNIL)
                    {
                        pParam = lua_tolstring(L, -1, & iParam);
                        if(pParam)
                        {
                            if (bCString)
                                sRow.push_back(string(pParam));
                            else
                                sRow.push_back(string(pParam, iParam));
                        }
                    }
                    //保证 idx 不变
                    lua_pop(L, 1);
                }
                sRows.push_back(sRow);
            }
            //保证 idx 不变
            lua_pop(L, 1);
        }
    }
    lua_settop(L, iTop);
    return true;
}

 

bool CxLuaCommon::callLuaFunction(lua_State *L , int iParamCount, int iResultCount)
{
    bool r = false;
    if (lua_pcall(L,iParamCount,iResultCount,0) == 0)
    {
        r = true;
    }
    else
    {
        string sError = lua_tostring(L, -1);
        CxLuaOutInfo::outScriptPrompt() << "error running function `f': " << sError;
        r = false;
    }
    //清空
    lua_settop(L, 0);
    return r;
}

bool CxLuaCommon::callLuaFunction(lua_State *L , int iParamCount, int iResultCount, int &iResult)
{
    bool r = false;
    if (callLuaFunction(L, iParamCount, iResultCount))
    {
        CxLuaCommon::getValue(L, -1, iResult);
        r = true;
    }
    //清空
    lua_settop(L, 0);
    return r;
}

bool CxLuaCommon::callLuaFunction(lua_State *L , int iParamCount, int iResultCount, string &sResult)
{
    bool r = false;
    if (callLuaFunction(L, iParamCount, iResultCount))
    {
        CxLuaCommon::getValue(L, -1, sResult);
        r = true;
    }
    //清空
    lua_settop(L, 0);
    return r;
}


string CxLuaCommon::fromTable(lua_State *L, int idx)
{
    if(L==NULL || idx<1) return std::string();

    string sRet = std::string();
    int type = lua_type(L, idx);

    if (type == LUA_TTABLE)
    {
        lua_pushnil(L);
        string sKey;
        string sValue;
        map<string,string> map1;
        while( lua_next(L,idx) )
        {
            CxLuaCommon::getValue(L, -2, sKey);
            CxLuaCommon::getValue(L, -1, sValue);
            map1[sKey] = sValue;
            lua_pop(L,1); // 弹出 value，让 key 留在栈顶
        }
        sRet = CxString::join(map1,cs_middle_string,cs_split_string);
		return sRet;
    }
    else if (type == LUA_TSTRING) {
        CxLuaCommon::getValue(L,idx, sRet);
		return sRet;
    }
    else if (type == LUA_TNUMBER) {
    	int bIs;
    	lua_Integer nIntVal = lua_tointegerx(L, idx, &bIs);
    	if (bIs)
    	{
	        int nNum = 0;
	        CxLuaCommon::getValue(L, idx, nNum);
	        sRet = CxString::format("%d", nNum);  
	        return sRet;  		
    	}

    	lua_Number nFloatVal = lua_tonumberx(L, idx, &bIs);
    	if (bIs)
    	{
	        double nNum = 0;
	        CxLuaCommon::getValue(L, idx, nNum);
	        sRet = CxString::format("%f", nNum);  
	        return sRet;      		
    	}
    }    
}

void CxLuaCommon::mapToTable(lua_State *L, const std::map<string, string> &data)
{
    lua_newtable(L);
    int type = 0;
    int n=data.size();
    CxLuaCommon::setTable(L, CS_Lua_Type, type);
    CxLuaCommon::setTable(L, CS_Lua_Count, n);
    for (map<string, string>::const_iterator it = data.begin(); it != data.end(); ++it)
    {
        CxLuaCommon::setTable(L, it->first, it->second);
    }
}

void CxLuaCommon::stringToTable(lua_State * L,const string &data)
{
    vector<string> v = CxString::split(data,"\r\n");

    if(v.size()==1) v = CxString::split(data,"\n");

    vector<map<string,string> > vv = CxString::splitToLines(v,'=',';');

    int type = 0;

    int n=vv.size();
    if (n>1)
    {
        const map<string,string> & map1= vv.at(0);
        if(map1.size()>1)type=1;
    }

    lua_newtable(L);
    CxLuaCommon::setTable(L, CS_Lua_Type, type);
    CxLuaCommon::setTable(L, CS_Lua_Count, n);
    if(type == 1) //多记录
    {
        for(size_t i=0;i<vv.size();++i)
        {
            const map<string,string> & map2 = vv.at(i);
            mapToTable(L,map2);          
            lua_rawseti(L,-2,i+1);
        }
    }
    else if(type == 0) //单记录
    {
        for(size_t i=0;i<vv.size();++i)
        {
            const map<string,string> & map2 = vv.at(i);
            for (map<string, string>::const_iterator it = map2.begin(); it != map2.end(); ++it)
            {
                CxLuaCommon::setTable(L, it->first, it->second);
            }
        }
    }
}



