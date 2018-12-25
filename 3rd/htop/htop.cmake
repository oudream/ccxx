

#1, htop
set(gs_ccxx_htop_path ${gs_project_3rd_path}/htop)
set(gs_chtop_include_path ${gs_project_include_path}/htop-5.3.4)

set(gsl_htop_headers
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
        src/htop.h
        src/htopconf.h
        src/htoplib.h
        src/lundump.h
        src/lvm.h
        src/lzio.h
        )

set(gsl_htop_sources
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

mc_merge_file_path(${gs_ccxx_htop_path} "${gsl_htop_headers}" gsl_chtop_header_filepaths)
mc_merge_file_path(${gs_ccxx_htop_path} "${gsl_htop_sources}" gsl_chtop_source_filepaths)

macro(mc_chtop_include)
    include_directories(${gs_chtop_include_path})
    source_group("chtop" FILES ${gsl_chtop_header_filepaths} ${gsl_chtop_source_filepaths})
endmacro()


#2, htop_base
set(gsl_htop_base_heads
        cxhtop/htop_common.h
        cxhtop/htop_base.h
        cxhtop/htop_run.h
        )

set(gsl_htop_base_sources
        cxhtop/htop_common.cpp
        cxhtop/htop_base.cpp
        cxhtop/htop_run.cpp
        )

mc_merge_file_path(${gs_ccxx_path} "${gsl_htop_base_heads}" gsl_cxhtop_base_head_filepaths)
mc_merge_file_path(${gs_ccxx_path} "${gsl_htop_base_sources}" gsl_cxhtop_base_source_filepaths)

macro(mc_cxhtop_base_include)
    source_group("htop_base" FILES ${gsl_cxhtop_base_head_filepaths} ${gsl_cxhtop_base_source_filepaths})
endmacro()


#3, htop_ccxx
set(gsl_htop_ccxx_heads
        cxhtop/htop_cxtime.h
        cxhtop/htop_cxstring.h
        cxhtop/htop_cxfilesystem.h
        cxhtop/htop_cxappenv.h
        cxhtop/htop_cxinterinfo.h
        cxhtop/htop_cxlog.h
        cxhtop/htop_cxencoding.h
        cxhtop/htop_cxapplication.h
        )

set(gsl_htop_ccxx_sources
        cxhtop/htop_cxtime.cpp
        cxhtop/htop_cxstring.cpp
        cxhtop/htop_cxfilesystem.cpp
        cxhtop/htop_cxappenv.cpp
        cxhtop/htop_cxinterinfo.cpp
        cxhtop/htop_cxlog.cpp
        cxhtop/htop_cxencoding.cpp
        cxhtop/htop_cxapplication.cpp
        )

mc_merge_file_path(${gs_ccxx_path} "${gsl_htop_ccxx_heads}" gsl_cxhtop_all_head_filepaths)
mc_merge_file_path(${gs_ccxx_path} "${gsl_htop_ccxx_sources}" gsl_cxhtop_all_source_filepaths)

macro(mc_cxhtop_all_include)
    source_group("htop_ccxx" FILES ${gsl_cxhtop_all_head_filepaths} ${gsl_cxhtop_all_source_filepaths})
endmacro()
