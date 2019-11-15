
if (WIN32)
    set(CURL_INCLUDE_DIRS "${gs_project_cross_compile_path}/include/curl")
    if (CYGWIN OR MINGW)
        set(CURL_LIBRARIES
                "${gs_project_lib_path}/mingw/libcurl.a"
                "${gs_project_lib_path}/mingw/libcurl.dll.a"
                )
    else ()
        set(CURL_LIBRARIES "${gs_project_lib_path}/vc/libcurl.lib")
    endif ()
else ()
    find_package(CURL REQUIRED)
    #    message(STATUS "curl library status:")
    #    message(STATUS "    libraries: ${CURL_LIBRARIES}")
    #    message(STATUS "    include path: ${CURL_INCLUDE_DIRS}")
endif ()


macro(mc_target_define_curl)
    include_directories(${CURL_INCLUDE_DIRS})
endmacro()


macro(mc_target_link_curl sTargetName)
    target_link_libraries(${sTargetName}
            ${CURL_LIBRARIES}
            )
endmacro()
