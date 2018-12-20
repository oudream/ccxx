###has ccxx
set(fslSourceCcxxs
        openssl/main.cpp
        )

mc_merge_file_path(${gs_ccxx_tools_path} "${fslSourceCcxxs}" fslSourceCcxxPaths)

add_executable(cxtool_openssl1
        ${fslSourceCcxxPaths})

add_dependencies(cxtool_openssl1
        ccxx
        )

target_link_libraries(cxtool_openssl1
        ccxx
        )

mc_target_link_libraries(cxtool_openssl1 PROPERTIES FOLDER ccxx/tools)

if (WIN32)
    set(gs_openssl_lib_path ${gs_project_cross_compile_path}/lib/win32)
    if (CYGWIN OR MINGW)
        target_link_libraries(cxtool_openssl1
                ${gs_openssl_lib_path}/mingw/libssl.a
                ${gs_openssl_lib_path}/mingw/libcrypto.a
                )
    else()
        target_link_libraries(cxtool_openssl1
                ${gs_openssl_lib_path}/vc/libssl32.lib
                ${gs_openssl_lib_path}/vc/libcrypto32.lib
                )
    endif ()
elseif(APPLE)
    include_directories("/usr/local/Cellar/openssl@1.1/1.1.0f/include")
    LINK_DIRECTORIES("/usr/local/Cellar/openssl@1.1/1.1.0f/lib")

    #target_link_libraries(cxtool_openssl1
    #        libssl.dylib
    #        libcrypto.dylib
    #        )
    target_link_libraries(cxtool_openssl1
            /usr/local/Cellar/openssl@1.1/1.1.0f/lib/libssl.dylib
            )
    target_link_libraries(cxtool_openssl1
            /usr/local/Cellar/openssl@1.1/1.1.0f/lib/libcrypto.dylib
            )
else()
    set(gs_openssl_lib_path ${gs_project_cross_compile_path}/x86linux)
    target_link_libraries(cxtool_openssl1
            ${gs_openssl_lib_path}/lib/libssl.a
            ${gs_openssl_lib_path}/lib/libcrypto.a
            )
endif ()

set_target_properties(cxtool_openssl1 PROPERTIES FOLDER ccxx/tools)
