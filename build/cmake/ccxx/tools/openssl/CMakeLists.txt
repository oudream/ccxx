
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

include_directories("/usr/local/Cellar/openssl@1.1/1.1.0f/include")
LINK_DIRECTORIES("/usr/local/Cellar/openssl@1.1/1.1.0f/lib")

mc_target_link_libraries(cxtool_openssl1 PROPERTIES FOLDER ccxx/tools)
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


set_target_properties(cxtool_openssl1 PROPERTIES FOLDER ccxx/tools)



