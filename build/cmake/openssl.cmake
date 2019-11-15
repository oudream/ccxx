
if (WIN32)
    set(OPENSSL_INCLUDE_DIR "${gs_project_cross_compile_path}/include/openssl")
    if (CYGWIN OR MINGW)
        set(OPENSSL_SSL_LIBRARY "${gs_project_cross_compile_path}/lib/win32/mingw/libssl.a")
        set(OPENSSL_CRYPTO_LIBRARY "${gs_project_cross_compile_path}/lib/win32/mingw/libcrypto.a")
    else()
        set(OPENSSL_SSL_LIBRARY "${gs_project_cross_compile_path}/lib/win32/vc/libssl.a")
        set(OPENSSL_CRYPTO_LIBRARY "${gs_project_cross_compile_path}/lib/win32/vc/libcrypto.a")
    endif ()
elseif(APPLE)
    set(OPENSSL_INCLUDE_DIR "${gs_project_cross_compile_path}/include/openssl")
    set(OPENSSL_SSL_LIBRARY "/usr/local/Cellar/openssl@1.1/1.1.0f/lib/libssl.dylib")
    set(OPENSSL_CRYPTO_LIBRARY "/usr/local/Cellar/openssl@1.1/1.1.0f/lib/libcrypto.dylib")
else()
    find_package (OpenSSL REQUIRED)
    # OPENSSL_INCLUDE_DIR
    # OPENSSL_CRYPTO_LIBRARY
    # OPENSSL_SSL_LIBRARY
    # OPENSSL_LIBRARIES
    # apt | apk | yam libssl-dev openssl-dev
endif ()


macro(mc_target_define_openssl)
    include_directories(${OPENSSL_INCLUDE_DIR})
endmacro()


macro(mc_target_link_openssl sTargetName)
    target_link_libraries(${sTargetName}
            ${OPENSSL_SSL_LIBRARY}
            ${OPENSSL_CRYPTO_LIBRARY}
            )
endmacro()
