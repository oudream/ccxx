
if (NOT WIN32)
    find_package(CURL REQUIRED)

    # If the package has been found, several variables will
    # be set, you can find the full list with descriptions
    # Print some message showing some of them
    message(STATUS "curl library status:")
    message(STATUS "    libraries: ${CURL_LIBRARIES}")
    message(STATUS "    include path: ${CURL_INCLUDE_DIRS}")

    # Link your application with curl libraries
    #target_link_libraries(curl_example ${CURL_LIBRARIES})
endif ()

macro(mc_target_link_curl sTargetName)
    if (WIN32)
        if (CYGWIN OR MINGW)
            target_link_libraries(${sTargetName}
                    ${gs_project_lib_path}/mingw/libcurl.a
                    ${gs_project_lib_path}/mingw/libcurl.dll.a
                    )
        else()
            target_link_libraries(${sTargetName}
                    ${gs_project_lib_path}/vc/libcurl.lib
                    )
        endif ()
        #    elseif(APPLE)
        #        target_link_libraries(${sTargetName}
        #                ${gs_project_lib_path}/libcurl.4.dylib
        #                )
    else()
        #        target_link_libraries(${sTargetName}
        #                ${gs_project_lib_path}/libcurl.a
        #                )
        # find_package(CURL REQUIRED)
        target_link_libraries(${sTargetName}
                ${CURL_LIBRARIES}
                )
    endif ()
endmacro()
