
mc_ccxx_curl_include()

add_definitions(-D"GM_BUILD_CCXX_CURL_LIB")

add_library(ccxx_curl SHARED
        ${gs_ccxx_curl_filepaths}
        )

add_dependencies(ccxx_curl
        ccxx
        )

target_link_libraries(ccxx_curl
        ccxx
        )

mc_target_link_curl(ccxx_curl)

set_target_properties(ccxx_curl PROPERTIES FOLDER ccxx/lib)

