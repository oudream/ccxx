
#set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)
#add_executable (curl-demo convert.cpp)
#include_directories(${CURL_INCLUDE_DIR})
#target_link_libraries(curl-demo ${CURL_LIBRARIES})

#############################################

macro(mc_create_test1 sCppName)
    set(gsl_test_${sCppName}_sources
            curl/${sCppName}.c
            )

    mc_merge_file_path(${gs_ccxx_test_path} "${gsl_test_${sCppName}_sources}"
            gsl_test_${sCppName}_source_filepaths)

    add_executable(cxtest_curl_${sCppName}
            ${gsl_test_${sCppName}_head_filepaths} ${gsl_test_${sCppName}_source_filepaths}
            )

    add_dependencies(cxtest_curl_${sCppName}
            ccxx
            )

    target_link_libraries(cxtest_curl_${sCppName}
            ccxx
            )

    if (WIN32)
        if (CYGWIN OR MINGW)
            target_link_libraries(cxtest_curl_${sCppName}
                    ${gs_project_lib_path}/mingw/libcurl.a
                    )
        else()
            target_link_libraries(cxtest_curl_${sCppName}
                    ${gs_project_lib_path}/mingw/libcurl.dll
                    )
        endif ()
#    elseif(APPLE)
#        target_link_libraries(cxtest_curl_${sCppName}
#                ${gs_project_lib_path}/libcurl.4.dylib
#                )
    else()
#        target_link_libraries(cxtest_curl_${sCppName}
#                ${gs_project_lib_path}/libcurl.a
#                )
        target_link_libraries(cxtest_curl_${sCppName}
                ${CURL_LIBRARIES}
                )
    endif ()

    set_target_properties(cxtest_curl_${sCppName} PROPERTIES FOLDER ccxx/test)
endmacro()

#set(v_name1 "ou")
#set(v_name2 "dream")
#set(${v_name1}${v_name2} "ouyangjinhong")
#message("oudream is " ${oudream})
#message("oudream is " ${${v_name1}${v_name2}})

#############################################

mc_create_test1(ftpget)
mc_create_test1(post-callback)
