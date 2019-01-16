
if (NOT WIN32)
    # Find OpenCV, you may need to set OpenCV_DIR variable
    # to the absolute path to the directory containing OpenCVConfig.cmake file
    # via the command line or GUI
    find_package(OpenCV REQUIRED)

    # If the package has been found, several variables will
    # be set, you can find the full list with descriptions
    # in the OpenCVConfig.cmake file.
    # Print some message showing some of them
    message(STATUS "OpenCV library status:")
    message(STATUS "    config: ${OpenCV_DIR}")
    message(STATUS "    version: ${OpenCV_VERSION}")
    message(STATUS "    libraries: ${OpenCV_LIBS}")
    message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

    # Link your application with OpenCV libraries
    #target_link_libraries(opencv_example ${OpenCV_LIBS})
endif ()

macro(mc_target_link_opencv sTargetName)
    if (WIN32)
        if (CYGWIN OR MINGW)
#            target_link_libraries(${sTargetName}
#                    ${gs_project_lib_path}/mingw/libcurl.a
#                    ${gs_project_lib_path}/mingw/libcurl.dll.a
#                    )
        else()
#            target_link_libraries(${sTargetName}
#                    ${gs_project_lib_path}/vc/libcurl.lib
#                    )
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
                ${OpenCV_LIBS}
                )
    endif ()
endmacro()
