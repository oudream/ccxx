#include "lua_cxfilesystem.h"

using namespace std;

bool fn_bCxLuaFilesystem()
{
    CxLuaManager::registLua(new CxLuaFileSystem);
    return true;
}

static bool f_bCxLuaFilesystem = fn_bCxLuaFilesystem();

static int fn_getPathInfo(lua_State *L)
{
    lua_newtable(L);     /* creates a table */ 

    int iTop = lua_gettop(L);

    string sFilePath;

    if(iTop >= 1)
    {
        CxLuaCommon::getValue(L, 1, sFilePath);
    }

    CxFilePathInfo pathInfo = CxFileSystem::getPathInfo(sFilePath);

    CxLuaCommon::setTable(L, "pathType",      (int)pathInfo.pathType);
    CxLuaCommon::setTable(L, "path",          pathInfo.path);
    CxLuaCommon::setTable(L, "fileName",      pathInfo.fileName);
    CxLuaCommon::setTable(L, "fileSize",      pathInfo.fileSize);
    CxLuaCommon::setTable(L, "creationTime",  pathInfo.creationTime);
    CxLuaCommon::setTable(L, "lastWriteTime", pathInfo.lastWriteTime);

    return 1;
}

static int fn_createFile(lua_State *L)
{
    lua_newtable(L);     /* creates a table */

    int iTop = lua_gettop(L);

    string sFileName;

    if(iTop >= 1)
    {
        CxLuaCommon::getValue(L, 1, sFileName);
    }

    bool ctFileFlag = CxFileSystem::createFile(sFileName);
    if (!ctFileFlag)
        return -1;
    return 1;
}

static int fn_deleteFile(lua_State *L)
{
    lua_newtable(L);     /* creates a table */

    int iTop = lua_gettop(L);

    string sFileName;

    if(iTop >= 1)
    {
        CxLuaCommon::getValue(L, 1, sFileName);
    }

    bool dlFileFlag = CxFileSystem::deleteFile(sFileName);
    if (!dlFileFlag)
        return -1;
    return 1;
}

static int fn_copyFile(lua_State *L)
{
    lua_newtable(L);     /* creates a table */

    int iTop = lua_gettop(L);

    string sSource="";
    string sTarget="";

    if(iTop >= 2)
    {
        CxLuaCommon::getValue(L, 1, sSource);
        CxLuaCommon::getValue(L, 2, sTarget);
    }
    //清空
    lua_pop(L,lua_gettop(L));

    CxFileSystem::copyFile(sSource, sTarget);
    return 1;
}

static int fn_renameFile(lua_State *L)
{
    lua_newtable(L);     /* creates a table */

    int iTop = lua_gettop(L);

    string sOldPath="";
    string sNewPath="";

    if(iTop >= 2)
    {
        CxLuaCommon::getValue(L, 1, sOldPath);
        CxLuaCommon::getValue(L, 2, sNewPath);
    }
    //清空
    lua_pop(L,lua_gettop(L));

    CxFileSystem::renameFile(sOldPath, sNewPath);
    return 1;
}

static int fn_isExist(lua_State *L)
{
    lua_newtable(L);     /* creates a table */

    int iTop = lua_gettop(L);

    string sFileName;

    if(iTop >= 1)
    {
        CxLuaCommon::getValue(L, 1, sFileName);
    }
    lua_pop(L,lua_gettop(L));

    bool bFlag = CxFileSystem::isExist(sFileName);
    if (!bFlag)
        return 0;
    return 1;
}

static int fn_isFile(lua_State *L)
{
    lua_newtable(L);     /* creates a table */

    int iTop = lua_gettop(L);

    string sFileName;

    if(iTop >= 1)
    {
        CxLuaCommon::getValue(L, 1, sFileName);
    }
    lua_pop(L,lua_gettop(L));

    bool bFlag = CxFileSystem::isFile(sFileName);
    if (!bFlag)
        return 0;
    return 1;
}

static int fn_isDir(lua_State *L)
{
    lua_newtable(L);     /* creates a table */

    int iTop = lua_gettop(L);

    string sFileName;

    if(iTop >= 1)
    {
        CxLuaCommon::getValue(L, 1, sFileName);
    }
    lua_pop(L,lua_gettop(L));

    bool bFlag = CxFileSystem::isDir(sFileName);
    if (!bFlag)
        return 0;
    return 1;
}

static int fn_createDir(lua_State *L)
{
    int iTop = lua_gettop(L);
    string sFileName;
    if(iTop >= 1)
    {
        CxLuaCommon::getValue(L, 1, sFileName);
    }
    lua_pop(L,iTop);
    return CxFileSystem::createDir(sFileName) ? 1 : 0;
}

static int fn_removeDir(lua_State *L)
{
    int iTop = lua_gettop(L);
    string sFileName;

    int nRes = -1;
    if(iTop >= 1)
    {
        CxLuaCommon::getValue(L, 1, sFileName);
        nRes = CxFileSystem::removeDir(sFileName);
    }
    //清空
    lua_pop(L,iTop);
    //返回结果
    lua_pushnumber(L, nRes);
    return 1;
}

static int fn_scanDir(lua_State *L)
{
    int iTop = lua_gettop(L);
    if (iTop < 2)
    {
    	lua_pop(L, iTop);
    	return 0;
    }
    
    string sScanpath;
    int nMode = 0;
    int nRes = -1;    

    CxLuaCommon::getValue(L, 1, sScanpath);
    CxLuaCommon::getValue(L, 2, nMode);

    bool bFlag = false;
    if (nMode > 0) 
    {
    	bFlag = true;
    }

    vector<CxFileSystem::PathInfo> tPathInfos;
    CxFileSystem::scanDir(sScanpath, tPathInfos, bFlag);  

    int nCount = tPathInfos.size();
    //清空
    lua_pop(L, iTop);

    lua_newtable(L);
    CxLuaCommon::setTable(L, CS_Lua_Succed, 1);
    CxLuaCommon::setTable(L, CS_Lua_Count, nCount);        	
    	
    for(size_t i=0; i<tPathInfos.size(); ++i)
    {
        CxFileSystem::PathInfo tInfo = tPathInfos.at(i);
        lua_newtable(L);
        CxLuaCommon::setTable(L, "Path", tInfo.path);
        CxLuaCommon::setTable(L, "PathType", (int)tInfo.pathType);
        CxLuaCommon::setTable(L, "FileName", tInfo.fileName);
        CxLuaCommon::setTable(L, "CreationTime", (long long)tInfo.creationTime);
        CxLuaCommon::setTable(L, "LastWriteTime", (long long)tInfo.lastWriteTime);
        CxLuaCommon::setTable(L, "FullPath", tInfo.filePath());
        lua_rawseti(L, -2, i+1);
    } 
    return 1;
}

std::string CxLuaFileSystem::tableNameImpl()
{
    return std::string("Clfs");
}

std::map<std::string, fn_int_lua_t> CxLuaFileSystem::tableMethodsImpl()
{
    map<string, fn_int_lua_t> r;
    r[string("GetPathInfo")] = fn_getPathInfo;
    r[string("CreateFile")] = fn_createFile;
    r[string("DeleteFile")] = fn_deleteFile;
    r[string("CopyFile")] = fn_copyFile;
    r[string("RenameFile")] = fn_renameFile;
    r[string("IsExist")] = fn_isExist;
    r[string("IsFile")] = fn_isFile;
    r[string("IsDir")] = fn_isDir;
    r[string("CreateDir")] = fn_createDir;
    r[string("RemoveDir")] = fn_removeDir;
    r[string("ScanDir")] = fn_scanDir;
    return r;
}
