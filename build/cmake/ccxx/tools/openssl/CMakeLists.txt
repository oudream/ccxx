###has ccxx

set(fslSourceCcxxs
        openssl/main.cpp
        )

mc_merge_file_path(${gs_ccxx_tools_path} "${fslSourceCcxxs}" fslSourceCcxxPaths)

add_executable(cxtool_openssl1
        ${fslSourceCcxxPaths})

mc_target_define_openssl()

mc_target_link_openssl(cxtool_openssl1)

target_link_libraries(cxtool_openssl1
        ccxx
        )

add_dependencies(cxtool_openssl1
        ccxx
        )

set_target_properties(cxtool_openssl1 PROPERTIES FOLDER ccxx/tools)
