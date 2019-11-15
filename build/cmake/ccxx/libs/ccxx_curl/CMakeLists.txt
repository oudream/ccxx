
mc_ccxx_curl_include()

add_definitions(-D"GM_BUILD_CCXX_CURL_LIB")

add_library(ccxx_curl SHARED
        ${gs_ccxx_curl_filepaths}
        )

mc_target_define_curl()

mc_target_link_curl(ccxx_curl)

add_dependencies(ccxx_curl
        ccxx
        )

target_link_libraries(ccxx_curl
        ccxx
        )

set_target_properties(ccxx_curl PROPERTIES FOLDER ccxx/lib)

