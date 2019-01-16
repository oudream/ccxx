
if (WIN32)
    if (CYGWIN OR MINGW)
        set(gs_openssl_lib_path ${gs_project_cross_compile_path}/lib/win32/mingw)
    else()
        set(gs_openssl_lib_path ${gs_project_cross_compile_path}/lib/win32/vc)
    endif ()
elseif(APPLE)
    # set(gs_openssl_lib_path "/usr/local/Cellar/openssl@1.1/1.1.0f/lib")
    set(gs_openssl_lib_path /usr/local/Cellar/openssl@1.1/1.1.0f/lib)
    set(gs_openssl_include_path /usr/local/Cellar/openssl@1.1/1.1.0f/include)
else()
    set(gs_openssl_lib_path ${gs_project_cross_compile_path}/x86linux/lib)
endif ()


macro(mc_target_define_openssl)
    #    include_directories(${gs_openssl_include_path})
    link_directories(${gs_openssl_lib_path})
endmacro()


macro(mc_target_link_openssl sTargetName)
    if (WIN32)
        if (CYGWIN OR MINGW)
            target_link_libraries(${sTargetName}
                    ${gs_openssl_lib_path}/libssl.a
                    ${gs_openssl_lib_path}/libcrypto.a
                    )
        else()
            target_link_libraries(${sTargetName}
                    ${gs_openssl_lib_path}/libssl32.lib
                    ${gs_openssl_lib_path}/libcrypto32.lib
                    )
        endif ()
    elseif(APPLE)
        target_link_libraries(${sTargetName}
                ${gs_openssl_lib_path}/libssl.dylib
                ${gs_openssl_lib_path}/libcrypto.dylib
                )
    else()
        target_link_libraries(${sTargetName}
                ${gs_openssl_lib_path}/libssl.a
                ${gs_openssl_lib_path}/libcrypto.a
                )
    endif ()
endmacro()
