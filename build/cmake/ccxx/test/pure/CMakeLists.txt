
###ccxx files
include(./../../ccxx.cmake)

set(gsl_ccxx_pure_heads
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
#        cxdescribe.h
        cxprofile.h
#        cxxml.h
#        cxefile.h
#        cxsharememory.h
#        cxsocket.h
#        cxbitmap.h
        cxprocess.h
#        cxprofile_skv_xml.h
#        cxprofile_skv_json.h
#        cxchannel_global.h
#        cxchannel.h
#        cxchannel_tcpclient.h
#        cxchannel_tcpserver.h
#        cxchannel_udp.h
#        cxchannel_serial.h
#        cxsafequeue.h
#        cxsafethread.h
#        cxsafebuffer.h
#        cxdatabase.h
#        cxbusiness.h
#        cxencoding.h
#        cxresource.h
#        cxjson.h
#        cxuuid.h
#        cxprocmutex.h
#        cxwinreg.h
        )

set(gsl_ccxx_pure_sources
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
#        cxdescribe.cpp
        cxinterinfo.cpp
        cxprofile.cpp
#        tinyxml/tinyxml.cpp
#        tinyxml/tinyxmlerror.cpp
#        tinyxml/tinyxmlparser.cpp
#        cxxml.cpp
#        cxefile.cpp
#        cxsharememory.cpp
#        cxsocket.cpp
#        cxbitmap.cpp
        cxprocess.cpp
#        cxprofile_skv_xml.cpp
#        cxprofile_skv_json.cpp
#        cxchannel.cpp
#        cxchannel_tcpclient.cpp
#        cxchannel_tcpserver.cpp
#        cxchannel_udp.cpp
#        cxchannel_serial.cpp
#        cxsafequeue.cpp
#        cxsafethread.cpp
#        cxsafebuffer.cpp
#        cxdatabase.cpp
#        cxbusiness.cpp
#        cxencoding.cpp
#        cxchannel_factorymanager.cpp
        cxprofile_factorymanager.cpp
#        cxdescribe_factorymanager.cpp
#        cxresource.cpp
#        cxjson.cpp
#        cxuuid.cpp
#        cxprocmutex.cpp
#        cxwinreg.cpp
        )

mc_merge_file_path(${gs_ccxx_source_path} "${gsl_ccxx_pure_heads}" gsl_ccxx_pure_head_filepaths)
mc_merge_file_path(${gs_ccxx_source_path} "${gsl_ccxx_pure_sources}" gsl_ccxx_pure_source_filepaths)

macro(mc_ccxx_pure_include)
    source_group("ccxx" FILES ${gsl_ccxx_pure_head_filepaths} ${gsl_ccxx_pure_source_filepaths})
    add_definitions(-D"GM_BUILD_CCXX_CORE_CODE")
    add_definitions(-D"GM_PROJECT_CXAPPLICATION_NULL")
    add_definitions(-D"GM_PROJECT_JSON_NULL")
    add_definitions(-D"GM_PROJECT_XML_NULL")
    #    win32 : target_link_libraries(xxxx wsock32 ws2_32 psapi)
endmacro()

mc_ccxx_pure_include()

set(fslSources
        pure/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)
source_group("pure" FILES ${fslSourcePaths})

add_executable(cxtest_pure
        ${fslSourcePaths}
        ${gsl_ccxx_pure_head_filepaths} ${gsl_ccxx_pure_source_filepaths}
        )

if (WIN32)
    target_link_libraries(cxtest_pure wsock32 ws2_32 psapi ntdll)
endif ()

if (UNIX)
    target_link_libraries(cxtest_pure dl rt)
endif ()

set_target_properties(cxtest_pure PROPERTIES FOLDER ccxx/test)

mc_build_with_cc_static()
