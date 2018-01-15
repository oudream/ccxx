#include "lua_cxstring.h"

using namespace std;

bool fn_bCxLuaString()
{
    CxLuaManager::registLua(new CxLuaString);
    return true;
}

static bool f_bCxLuaString = fn_bCxLuaString();

static int fn_toDouble(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 1) //最少1个参数
    {
        return 0;
    }
    string s = CxLuaCommon::fromTable(L,1);
    int mode =1;
    if(top>1) mode = CxString::toInt32(CxLuaCommon::fromTable(L,2));

    lua_pop(L, top);//清空

    double val = 0;

//    cxDebug()<<mode<<CxString::toHexstring(s.data(),s.length())<<cxEndLine;

    CxBuffer::fromBuf((uint8*)(s.c_str()),sizeof(double),val,mode);
    lua_pushnumber(L, val);
    return 1;
}

static int fn_toInt(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 1) //最少1个参数
    {
        return 0;
    }
    string s = CxLuaCommon::fromTable(L,1);
    int mode =1;
    if(top>1) mode = CxString::toInt32(CxLuaCommon::fromTable(L,2));

    lua_pop(L, top);//清空

    int32 val = 0;

    CxBuffer::fromBuf((uint8*)(s.c_str()),sizeof(int32),val,mode);
    lua_pushnumber(L, val);
    return 1;
}
static int fn_toFloat(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 1) //最少1个参数
    {
        return 0;
    }

    string s = CxLuaCommon::fromTable(L,1);
    int mode =1;
    if(top>1) mode = CxString::toInt32(CxLuaCommon::fromTable(L,2));

    lua_pop(L, top);//清空

    float val = 0;
    CxBuffer::fromBuf((uint8*)(s.c_str()),sizeof(float),val,mode);
    lua_pushnumber(L, val);
    return 1;
}
static int fn_toBit(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 2) //最少1个参数
    {
        return 0;
    }
    string s = CxLuaCommon::fromTable(L,1);
    int bit =  CxString::toInt32(CxLuaCommon::fromTable(L,2));

    lua_pop(L, top);//清空

    uint8* p = (uint8*)(s.c_str());
    int val = -1;
    if(p &&  bit>0 && bit<9 ) val = p[0]&(1<<(bit-1));
    lua_pushnumber(L, val);
    return 1;
}

static int fn_toBit_v2(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 3) //最少1个参数
    {
        return 0;
    }

    string s = CxLuaCommon::fromTable(L,1);
    int pos = CxString::toInt32(CxLuaCommon::fromTable(L,2));
    int bit = CxString::toInt32(CxLuaCommon::fromTable(L,3));

    lua_pop(L, top);//清空

    uint8* p = (uint8*)(s.c_str());
    int val = -1;
    if(p && pos>0 && bit>0 && bit<9 ) val = p[pos-1]&(1<<(bit-1));
    lua_pushnumber(L, val);
    return 1;
}

static int fn_toShort(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 1) //最少1个参数
    {
        return 0;
    }

    string s = CxLuaCommon::fromTable(L,1);
    int mode =1;
    if(top>1) mode = CxString::toInt32(CxLuaCommon::fromTable(L,2));

    lua_pop(L, top);//清空

    int16 val = 0;
    CxBuffer::fromBuf((uint8*)(s.c_str()),sizeof(int16),val,mode);
    lua_pushnumber(L, val);
    return 1;
}

static int fn_toWord(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 1) //最少1个参数
    {
        return 0;
    }

    string s = CxLuaCommon::fromTable(L,1);
    int mode =1;
    if(top>1) mode = CxString::toInt32(CxLuaCommon::fromTable(L,2));

    lua_pop(L, top);//清空

    uint16 val = 0;
    CxBuffer::fromBuf((uint8*)(s.c_str()),sizeof(uint16),val,mode);
    lua_pushnumber(L, val);
    return 1;
}
static int fn_toDword(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 1) //最少1个参数
    {
        return 0;
    }

    string s = CxLuaCommon::fromTable(L,1);
    int mode =1;
    if(top>1) mode = CxString::toInt32(CxLuaCommon::fromTable(L,2));

    lua_pop(L, top);//清空

    uint32 val = 0;
    CxBuffer::fromBuf((uint8*)(s.c_str()),sizeof(uint32),val,mode);
    lua_pushnumber(L, val);
    return 1;
}

static int fn_toShort_v2(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 2) //最少2个参数
    {
        return 0;
    }

    string s = CxLuaCommon::fromTable(L,1);
    int pos = CxString::toInt32(CxLuaCommon::fromTable(L,2));
    int mode =1;
    if (top>2) mode = CxString::toInt32(CxLuaCommon::fromTable(L,3));

    lua_pop(L, top);//清空

    int16 val = 0;
    CxBuffer::fromBuf((uint8*)(s.c_str())+pos,sizeof(int16),val,mode);
    lua_pushnumber(L, val);
    return 1;
}

static int fn_toWord_v2(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 2) //最少2个参数
    {
        return 0;
    }

    string s = CxLuaCommon::fromTable(L,1);
    int pos = CxString::toInt32(CxLuaCommon::fromTable(L,2));
    int mode =1;
    if(top>2) mode = CxString::toInt32(CxLuaCommon::fromTable(L,3));

    lua_pop(L, top);//清空

    uint16 val = 0;
    CxBuffer::fromBuf((uint8*)(s.c_str())+pos,sizeof(uint16),val,mode);
    lua_pushnumber(L, val);
    return 1;
}
static int fn_toDword_v2(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 2) //最少2个参数
    {
        return 0;
    }

    string s = CxLuaCommon::fromTable(L,1);
    int pos = CxString::toInt32(CxLuaCommon::fromTable(L,2));
    int mode =1;
    if(top>2) mode = CxString::toInt32(CxLuaCommon::fromTable(L,3));

    lua_pop(L, top);//清空

    uint32 val = 0;
    CxBuffer::fromBuf((uint8*)(s.c_str())+pos,sizeof(uint32),val,mode);
    lua_pushnumber(L, val);
    return 1;
}


static int fn_toDouble_v2(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 2) //最少2个参数
    {
        return 0;
    }
    string s = CxLuaCommon::fromTable(L,1);
    int pos = CxString::toInt32(CxLuaCommon::fromTable(L,2));
    int mode =1;
    if(top>2) mode = CxString::toInt32(CxLuaCommon::fromTable(L,3));

    lua_pop(L, top);//清空

    double val = 0;
    CxBuffer::fromBuf((uint8*)(s.c_str())+pos,sizeof(double),val,mode);
    lua_pushnumber(L, val);
    return 1;
}

static int fn_toInt_v2(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 2) //最少2个参数
    {
        return 0;
    }
    string s = CxLuaCommon::fromTable(L,1);
    int pos = CxString::toInt32(CxLuaCommon::fromTable(L,2));
    int mode =1;
    if(top>2) mode = CxString::toInt32(CxLuaCommon::fromTable(L,3));

    lua_pop(L, top);//清空

    int32 val = 0;

    CxBuffer::fromBuf((uint8*)(s.c_str())+pos,sizeof(int32),val,mode);
    lua_pushnumber(L, val);
    return 1;
}

static int fn_toFloat_v2(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 2) //最少2个参数
    {
        return 0;
    }

    string s = CxLuaCommon::fromTable(L,1);
    int pos = CxString::toInt32(CxLuaCommon::fromTable(L,2));
    int mode =1;
    if(top>2) mode = CxString::toInt32(CxLuaCommon::fromTable(L,3));

    lua_pop(L, top);//清空

    float val = 0;
    CxBuffer::fromBuf((uint8*)(s.c_str())+pos,sizeof(float),val,mode);
    lua_pushnumber(L, val);
    return 1;
}

std::string CxLuaString::tableNameImpl()
{
    return std::string("ClString");
}

std::map<std::string, fn_int_lua_t> CxLuaString::tableMethodsImpl()
{
    map<string, fn_int_lua_t> r;
    r[string("ToBit")] = fn_toBit;
    r[string("ToShort")] = fn_toShort;
    r[string("ToUshort")] = fn_toWord;
    r[string("ToInt")] = fn_toInt;
    r[string("ToUint")] = fn_toDword;
    r[string("ToFloat")] = fn_toFloat;
    r[string("ToDouble")] = fn_toDouble;
    r[string("ToBit_v2")] = fn_toBit_v2;
    r[string("ToShort_v2")] = fn_toShort_v2;
    r[string("ToUshort_v2")] = fn_toWord_v2;
    r[string("ToInt_v2")] = fn_toInt_v2;
    r[string("ToUint_v2")] = fn_toDword_v2;
    r[string("ToFloat_v2")] = fn_toFloat_v2;
    r[string("ToDouble_v2")] = fn_toDouble_v2;
    return r;
}

