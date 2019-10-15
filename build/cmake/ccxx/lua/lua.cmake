
#1, lua
#2, lua.base
#3, lua.ccxx


#1, lua
set(gs_ccxx_lua_path ${gs_project_3rd_path}/lua/lua)
set(gs_clua_include_path ${gs_project_include_path}/lua/lua)

set(gsl_lua_headers
        src/lapi.h
        src/lauxlib.h
        src/lcode.h
        src/lctype.h
        src/ldebug.h
        src/ldo.h
        src/lfunc.h
        src/lgc.h
        src/llex.h
        src/llimits.h
        src/lmem.h
        src/lobject.h
        src/lopcodes.h
        src/lparser.h
        src/lstate.h
        src/lstring.h
        src/ltable.h
        src/ltm.h
        src/lua.h
        src/luaconf.h
        src/lualib.h
        src/lundump.h
        src/lvm.h
        src/lzio.h
        )

set(gsl_lua_sources
        src/lapi.c
        src/lauxlib.c
        src/lbaselib.c
        src/lbitlib.c
        src/lcode.c
        src/lcorolib.c
        src/lctype.c
        src/ldblib.c
        src/ldebug.c
        src/ldo.c
        src/ldump.c
        src/lfunc.c
        src/lgc.c
        src/linit.c
        src/liolib.c
        src/llex.c
        src/lmathlib.c
        src/lmem.c
        src/loadlib.c
        src/lobject.c
        src/lopcodes.c
        src/loslib.c
        src/lparser.c
        src/lstate.c
        src/lstring.c
        src/lstrlib.c
        src/ltable.c
        src/ltablib.c
        src/ltm.c
        src/lundump.c
        src/lutf8lib.c
        src/lvm.c
        src/lzio.c
        )

mc_merge_file_path(${gs_ccxx_lua_path} "${gsl_lua_headers}" gsl_clua_header_filepaths)
mc_merge_file_path(${gs_ccxx_lua_path} "${gsl_lua_sources}" gsl_clua_source_filepaths)

macro(mc_clua_include)
    include_directories(${gs_clua_include_path})
    source_group("clua" FILES ${gsl_clua_header_filepaths} ${gsl_clua_source_filepaths})
endmacro()


#2, lua_base
set(gsl_lua_base_heads
        lua_common.h
        lua_base.h
        lua_run.h
        )

set(gsl_lua_base_sources
        lua_common.cpp
        lua_base.cpp
        lua_run.cpp
        )

set(gs_ccxx_cxlua_path ${gs_ccxx_source_path}/cxlua)

mc_merge_file_path(${gs_ccxx_cxlua_path} "${gsl_lua_base_heads}" gsl_cxlua_base_head_filepaths)
mc_merge_file_path(${gs_ccxx_cxlua_path} "${gsl_lua_base_sources}" gsl_cxlua_base_source_filepaths)

macro(mc_cxlua_base_include)
    source_group("lua_base" FILES ${gsl_cxlua_base_head_filepaths} ${gsl_cxlua_base_source_filepaths})
endmacro()


#3, lua_ccxx
set(gsl_lua_ccxx_heads
        lua_cxtime.h
        lua_cxstring.h
        lua_cxfilesystem.h
        lua_cxappenv.h
        lua_cxinterinfo.h
        lua_cxlog.h
        lua_cxencoding.h
        lua_cxapplication.h
        )

set(gsl_lua_ccxx_sources
        lua_cxtime.cpp
        lua_cxstring.cpp
        lua_cxfilesystem.cpp
        lua_cxappenv.cpp
        lua_cxinterinfo.cpp
        lua_cxlog.cpp
        lua_cxencoding.cpp
        lua_cxapplication.cpp
        )

mc_merge_file_path(${gs_ccxx_cxlua_path} "${gsl_lua_ccxx_heads}" gsl_cxlua_all_head_filepaths)
mc_merge_file_path(${gs_ccxx_cxlua_path} "${gsl_lua_ccxx_sources}" gsl_cxlua_all_source_filepaths)

macro(mc_cxlua_all_include)
    source_group("lua_ccxx" FILES ${gsl_cxlua_all_head_filepaths} ${gsl_cxlua_all_source_filepaths})
endmacro()
