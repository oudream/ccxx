
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
        cxxml.h
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
        cxsafequeue.h
        cxsafethread.h
        cxsafebuffer.h
        cxdatabase.h
        cxbusiness.h
        cxencoding.h
        cxresource.h
        cxjson.h
        cxuuid.h
        cxprocess.h
        cxprocmutex.h
        cxwinreg.h
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
        cxxml.cpp
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
        cxsafequeue.cpp
        cxsafethread.cpp
        cxsafebuffer.cpp
        cxdatabase.cpp
        cxbusiness.cpp
        cxencoding.cpp
        cxchannel_factorymanager.cpp
        cxprofile_factorymanager.cpp
        cxdescribe_factorymanager.cpp
        cxresource.cpp
        cxjson.cpp
        cxuuid.cpp
        cxprocess.cpp
        cxprocmutex.cpp
        cxwinreg.cpp
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
        cxjson.h
        cxprofile_skv_json.h
        )

set(gsl_ccxx_env_sources
        cxglobal.cpp
        cxstring.cpp
        cxtime.cpp
        cxfile.cpp
        cxappenv.cpp
        cxprofile.cpp
        cxjson.cpp
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
        cxjson.h
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
        cxjson.cpp
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
        cxjson.h
        cxprofile_skv_json.h
        cxthread.h
        cxinterinfo.h
        cxlog.h
        cxsystem.h
        cxcontainer.h
        cxmath.h
        cxbuffer.h
        cxcrc.h
        cxxml.h
        cxprofile_skv_xml.h
        cxsocket.h
        cxprocess.h
        cxdatabase.h
        cxencoding.h
        cxuuid.h
        cxprocmutex.h
        cxwinreg.h
        )

set(gsl_ccxx_simple_sources
        cxglobal.cpp
        cxstring.cpp
        cxtime.cpp
        cxfile.cpp
        cxappenv.cpp
        cxprofile.cpp
        cxjson.cpp
        cxprofile_skv_json.cpp
        cxprofile_factorymanager.cpp
        cxthread.cpp
        cxinterinfo.cpp
        cxlog.cpp
        cxsystem.cpp
        cxmath.cpp
        cxbuffer.cpp
        cxcrc.cpp
        cxxml.cpp
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


#4, regexp
set(gs_ccxx_regexp_path ${gs_ccxx_source_path}/regexp)

set(gsl_regexp_headers
        re2/filtered_re2.h
        re2/prefilter.h
        re2/prefilter_tree.h
        re2/prog.h
        re2/re2.h
        re2/regexp.h
        re2/set.h
        re2/stringpiece.h
        re2/unicode_casefold.h
        re2/unicode_groups.h
        re2/variadic_function.h
        re2/walker-inl.h
        util/atomicops.h
        util/flags.h
        util/logging.h
        util/mutex.h
        util/sparse_array.h
        util/sparse_set.h
        util/utf.h
        util/util.h
        util/pcre.h
        )

set(gsl_regexp_sources
        re2/bitstate.cc
        re2/compile.cc
        re2/dfa.cc
        re2/filtered_re2.cc
        re2/mimics_pcre.cc
        re2/nfa.cc
        re2/onepass.cc
        re2/parse.cc
        re2/perl_groups.cc
        re2/prefilter.cc
        re2/prefilter_tree.cc
        re2/prog.cc
        re2/re2.cc
        re2/regexp.cc
        re2/set.cc
        re2/simplify.cc
        re2/stringpiece.cc
        re2/tostring.cc
        re2/unicode_casefold.cc
        re2/unicode_groups.cc
        util/rune.cc
        util/strutil.cc
        util/pcre.cc
        )

mc_merge_file_path(${gs_ccxx_regexp_path} "${gsl_regexp_headers}" gsl_regexp_header_filepaths)
mc_merge_file_path(${gs_ccxx_regexp_path} "${gsl_regexp_sources}" gsl_regexp_source_filepaths)

macro(mc_ccxx_regexp_include)
    include_directories(${gs_ccxx_regexp_path})
    source_group("ccxx_regexp" FILES ${gsl_regexp_header_filepaths} ${gsl_regexp_source_filepaths})
endmacro()


#5, qt
set(gsl_ccxx_qt_files
        qtcommon.h
        qtcommon.cpp
        cxapplication_qt.h
        cxapplication_qt.cpp
        )

mc_merge_file_path(${gs_ccxx_source_path} "${gsl_ccxx_qt_files}" gsl_ccxx_qt_filepaths)

macro(mc_ccxx_qt_include)
    source_group("ccxx_qt" FILES ${gsl_ccxx_qt_filepaths})
endmacro()
