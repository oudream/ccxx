
#1, ccxx
#1.1, ccxx_env ( string + filesystem + config(profile) )
#1.2, ccxx_interinfo ( [env] + [log](thread) )
#1.3, ccxx_simple ( [env] + [log] + system + container + math + buffer + crc + xml + socket + process + database + encoding + uuuid + procmutex + winreg)
#2, odbc
#3, sqlite
#4, regexp
#5, qt

#1, ccxx
#gs_project_src_path

set(gs_ccxx_source_path ${gs_ccxx_path}/ccxx)
set(gs_ccxx_sample_path ${gs_ccxx_path}/sample)
set(gs_ccxx_test_path ${gs_ccxx_path}/test)
set(gs_ccxx_tools_path ${gs_ccxx_path}/tools)

include_directories(${gs_ccxx_path})
include_directories(${gs_ccxx_source_path})

set(gsl_ccxx_heads
        readme.txt
        cxglobal.h
        cxappenv.h
        cxapplication.h
        cxsystem.h
        cxcontainer.h
        cxstring.h
        cxmath.h
        cxbuffer.h
        cxpacket.h
        cxcrc.h
        cxfile.h
        cxthread.h
        cxtime.h
        cxtimer.h
        cxlog.h
        cxinterinfo.h
        cxcontext.h
        cxdescribe.h
        cxprofile.h
        cxtinyxml.h
        cxefile.h
        cxsharememory.h
        cxsocket.h
        cxbitmap.h
        cxprofile_skv_xml.h
        cxprofile_skv_json.h
        cxchannel_global.h
        cxchannel.h
        cxchannel_none.h
        cxchannel_tcpclient.h
        cxchannel_tcpserver.h
        cxchannel_udp.h
        cxchannel_serial.h
        cxdatabase.h
        cxbusiness.h
        cxencoding.h
        cxresource.h
        cxrapidjson.h
        cxuuid.h
        cxprocess.h
        cxprocmutex.h
        cxwinreg.h
        cxlz4.h
        )

set(gsl_ccxx_sources
        cxglobal.cpp
        cxappenv.cpp
        cxapplication.cpp
        cxsystem.cpp
        cxstring.cpp
        cxmath.cpp
        cxbuffer.cpp
        cxpacket.cpp
        cxcrc.cpp
        cxfile.cpp
        cxthread.cpp
        cxtime.cpp
        cxtimer.cpp
        cxlog.cpp
        cxcontext.cpp
        cxdescribe.cpp
        cxinterinfo.cpp
        cxprofile.cpp
        tinyxml/tinyxml.cpp
        tinyxml/tinyxmlerror.cpp
        tinyxml/tinyxmlparser.cpp
        cxtinyxml.cpp
        cxefile.cpp
        cxsharememory.cpp
        cxsocket.cpp
        cxbitmap.cpp
        cxprofile_skv_xml.cpp
        cxprofile_skv_json.cpp
        cxchannel.cpp
        cxchannel_none.cpp
        cxchannel_tcpclient.cpp
        cxchannel_tcpserver.cpp
        cxchannel_udp.cpp
        cxchannel_serial.cpp
        cxdatabase.cpp
        cxbusiness.cpp
        cxencoding.cpp
        cxchannel_factorymanager.cpp
        cxprofile_factorymanager.cpp
        cxdescribe_factorymanager.cpp
        cxresource.cpp
        cxrapidjson.cpp
        cxuuid.cpp
        cxprocess.cpp
        cxprocmutex.cpp
        cxwinreg.cpp
        cxlz4.cpp
        lz4/lz4.c
        )

mc_merge_file_path(${gs_ccxx_source_path} "${gsl_ccxx_heads}" gsl_ccxx_head_filepaths)
mc_merge_file_path(${gs_ccxx_source_path} "${gsl_ccxx_sources}" gsl_ccxx_source_filepaths)

macro(mc_ccxx_include)
    source_group("ccxx" FILES ${gsl_ccxx_head_filepaths} ${gsl_ccxx_source_filepaths})
endmacro()


#1.1, ccxx_env ( string + filesystem + config(profile) )
set(gsl_ccxx_env_heads
        cxglobal.h
        cxstring.h
        cxtime.h
        cxfile.h
        cxappenv.h
        cxprofile.h
        cxrapidjson.h
        cxprofile_skv_json.h
        )

set(gsl_ccxx_env_sources
        cxglobal.cpp
        cxstring.cpp
        cxtime.cpp
        cxfile.cpp
        cxappenv.cpp
        cxprofile.cpp
        cxrapidjson.cpp
        cxprofile_skv_json.cpp
        cxprofile_factorymanager.cpp
        )

mc_merge_file_path(${gs_ccxx_source_path} "${gsl_ccxx_env_heads}" gsl_ccxx_env_head_filepaths)
mc_merge_file_path(${gs_ccxx_source_path} "${gsl_ccxx_env_sources}" gsl_ccxx_env_source_filepaths)

macro(mc_ccxx_env_include)
    source_group("ccxx" FILES ${gsl_ccxx_env_head_filepaths} ${gsl_ccxx_env_source_filepaths})
    add_definitions(-D"GM_BUILD_CCXX_CORE_CODE")
    add_definitions(-D"GM_PROJECT_XML_NULL")
endmacro()


#1.2, ccxx_interinfo ( [env] + [log](thread) )
set(gsl_ccxx_interinfo_heads
        cxglobal.h
        cxstring.h
        cxtime.h
        cxfile.h
        cxappenv.h
        cxprofile.h
        cxrapidjson.h
        cxprofile_skv_json.h
        cxthread.h
        cxinterinfo.h
        cxlog.h
        )

set(gsl_ccxx_interinfo_sources
        cxglobal.cpp
        cxstring.cpp
        cxtime.cpp
        cxfile.cpp
        cxappenv.cpp
        cxprofile.cpp
        cxrapidjson.cpp
        cxprofile_skv_json.cpp
        cxprofile_factorymanager.cpp
        cxthread.cpp
        cxinterinfo.cpp
        cxlog.cpp
        )

mc_merge_file_path(${gs_ccxx_source_path} "${gsl_ccxx_interinfo_heads}" gsl_ccxx_interinfo_head_filepaths)
mc_merge_file_path(${gs_ccxx_source_path} "${gsl_ccxx_interinfo_sources}" gsl_ccxx_interinfo_source_filepaths)

macro(mc_ccxx_interinfo_include)
    source_group("ccxx" FILES ${gsl_ccxx_interinfo_head_filepaths} ${gsl_ccxx_interinfo_source_filepaths})
    add_definitions(-D"GM_BUILD_CCXX_CORE_CODE")
    add_definitions(-D"GM_PROJECT_XML_NULL")
endmacro()


#1.3, ccxx_simple ( [env] + [log] + system + container + math + buffer + crc + xml + socket + process + database + encoding + uuuid + procmutex + winreg)
set(gsl_ccxx_simple_heads
        cxglobal.h
        cxstring.h
        cxtime.h
        cxfile.h
        cxappenv.h
        cxprofile.h
        cxrapidjson.h
        cxprofile_skv_json.h
        cxthread.h
        cxinterinfo.h
        cxlog.h
        cxsystem.h
        cxcontainer.h
        cxmath.h
        cxbuffer.h
        cxcrc.h
        cxtinyxml.h
        cxprofile_skv_xml.h
        cxsocket.h
        cxprocess.h
        cxdatabase.h
        cxencoding.h
        cxuuid.h
        cxprocmutex.h
        cxwinreg.h
        cxlz4.h
        )

set(gsl_ccxx_simple_sources
        cxglobal.cpp
        cxstring.cpp
        cxtime.cpp
        cxfile.cpp
        cxappenv.cpp
        cxprofile.cpp
        cxrapidjson.cpp
        cxprofile_skv_json.cpp
        cxprofile_factorymanager.cpp
        cxthread.cpp
        cxinterinfo.cpp
        cxlog.cpp
        cxsystem.cpp
        cxmath.cpp
        cxbuffer.cpp
        cxcrc.cpp
        cxtinyxml.cpp
        tinyxml/tinyxml.cpp
        tinyxml/tinyxmlerror.cpp
        tinyxml/tinyxmlparser.cpp
        cxprofile_skv_xml.cpp
        cxsocket.cpp
        cxprocess.cpp
        cxdatabase.cpp
        cxencoding.cpp
        cxuuid.cpp
        cxprocmutex.cpp
        cxwinreg.cpp
        cxlz4.cpp
        lz4/lz4.c
        )

mc_merge_file_path(${gs_ccxx_source_path} "${gsl_ccxx_simple_heads}" gsl_ccxx_simple_head_filepaths)
mc_merge_file_path(${gs_ccxx_source_path} "${gsl_ccxx_simple_sources}" gsl_ccxx_simple_source_filepaths)

macro(mc_ccxx_simple_include)
    source_group("ccxx" FILES ${gsl_ccxx_simple_head_filepaths} ${gsl_ccxx_simple_source_filepaths})
    add_definitions(-D"GM_BUILD_CCXX_CORE_CODE")
    add_definitions(-D"GM_PROJECT_CXAPPLICATION_NULL")
#    win32 : target_link_libraries(xxxx wsock32 ws2_32 psapi)
endmacro()


#2, odbc
set(gs_ccxx_odbc_files
        otl/otlv4.h
        cxdatabase_odbc.h
        cxdatabase_odbc.cpp
        )

mc_merge_file_path(${gs_ccxx_source_path} "${gs_ccxx_odbc_files}" gs_ccxx_odbc_filepaths)

macro(mc_ccxx_odbc_include)
    source_group("ccxx_odbc" FILES ${gs_ccxx_odbc_filepaths})
endmacro()


#3, sqlite
set(gs_ccxx_sqlite_files
        cxdatabase_sqlite.h
        sqlite3/sqlite3.h
        sqlite3/sqlite3ext.h
        cxdatabase_sqlite.cpp
        sqlite3/sqlite3.c
        )

mc_merge_file_path(${gs_ccxx_source_path} "${gs_ccxx_sqlite_files}" gs_ccxx_sqlite_filepaths)

macro(mc_ccxx_sqlite_include)
    source_group("ccxx_sqlite" FILES ${gs_ccxx_sqlite_filepaths})
endmacro()


#5, qt
set(gsl_ccxx_qt_files
        cxqtutil.h
        cxqtutil.cpp
        cxapplication_qt.h
        cxapplication_qt.cpp
        )

mc_merge_file_path(${gs_ccxx_source_path} "${gsl_ccxx_qt_files}" gsl_ccxx_qt_filepaths)

macro(mc_ccxx_qt_include)
    source_group("ccxx_qt" FILES ${gsl_ccxx_qt_filepaths})
endmacro()


#, curl
set(gs_ccxx_curl_files
        cxcurl.h
        cxcurl.cpp
        )

mc_merge_file_path(${gs_ccxx_source_path} "${gs_ccxx_curl_files}" gs_ccxx_curl_filepaths)

macro(mc_ccxx_curl_include)
    source_group("ccxx_curl" FILES ${gs_ccxx_curl_filepaths})
endmacro()

#, hiredis
set(gsl_ccxx_hiredis_files
        cxhiredis.h
        cxhiredis.cpp
        )

mc_merge_file_path(${gs_ccxx_source_path} "${gsl_ccxx_hiredis_files}" gsl_ccxx_hiredis_filepaths)

macro(mc_ccxx_hiredis_include)
    source_group("ccxx_hiredis" FILES ${gsl_ccxx_hiredis_filepaths})
endmacro()
