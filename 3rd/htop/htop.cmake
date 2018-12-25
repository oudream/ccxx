

#1, htop
set(gs_ccxx_htop_path ${CMAKE_CURRENT_SOURCE_DIR})
set(gs_chtop_include_path ${gs_ccxx_htop_path})


set(gsl_htop_headers
        htop/RichString.h
        htop/XAlloc.h
        htop/Object.h
        htop/Hashtable.h
        htop/FunctionBar.h
        htop/Process.h
        htop/Vector.h
        htop/Hashtable.h
        htop/UsersTable.h
        htop/Panel.h
        htop/Process.h
        htop/Settings.h
        htop/CRT.h
        htop/ListItem.h
        htop/StringUtils.h
        )

set(gsl_htop_sources
        htop/RichString.c
        htop/XAlloc.c
        htop/Object.c
        htop/Hashtable.c
        htop/FunctionBar.c
        htop/Process.c
        htop/Vector.c
        htop/Hashtable.c
        htop/UsersTable.c
        htop/Panel.c
        htop/Process.c
        htop/Settings.c
        htop/CRT.c
        htop/ListItem.c
        htop/StringUtils.c
        )

mc_merge_file_path(${gs_ccxx_htop_path} "${gsl_htop_headers}" gsl_chtop_header_filepaths)
mc_merge_file_path(${gs_ccxx_htop_path} "${gsl_htop_sources}" gsl_chtop_source_filepaths)

macro(mc_chtop_include)
    include_directories(${gs_chtop_include_path})
    source_group("chtop" FILES ${gsl_chtop_header_filepaths} ${gsl_chtop_source_filepaths})
endmacro()



#2, htop_os
set(gsl_htop_darwin_heads
        htop/darwin/DarwinCRT.h
        htop/darwin/DarwinProcess.h
        htop/darwin/DarwinProcessList.h
        htop/darwin/Platform.h
        )

set(gsl_htop_darwin_sources
        htop/darwin/DarwinCRT.c
        htop/darwin/DarwinProcess.c
        htop/darwin/DarwinProcessList.c
        htop/darwin/Platform.c
        )

mc_merge_file_path(${gs_ccxx_htop_path} "${gsl_htop_darwin_heads}" gsl_cxhtop_darwin_head_filepaths)
mc_merge_file_path(${gs_ccxx_htop_path} "${gsl_htop_darwin_sources}" gsl_cxhtop_darwin_source_filepaths)

macro(mc_cxhtop_darwin_include)
    source_group("htop_darwin" FILES ${gsl_cxhtop_darwin_head_filepaths} ${gsl_cxhtop_darwin_source_filepaths})
endmacro()
