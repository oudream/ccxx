
include(htop.cmake)

mc_chtop_include()
mc_cxhtop_darwin_include()

add_definitions(-D"GM_BUILD_CCXX_HTOP_LIB")

add_library(cxhtop SHARED
        ${gsl_chtop_header_filepaths} ${gsl_chtop_source_filepaths}
        ${gsl_cxhtop_darwin_head_filepaths} ${gsl_cxhtop_darwin_source_filepaths}
        )

add_dependencies(cxhtop
        ccxx
        )

target_link_libraries(cxhtop
        ccxx
        )

if (MSVC)
    add_definitions(-D"_CRT_SECURE_NO_WARNINGS")
    add_definitions( "/wd4819" )
endif ()

#add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo" )

mc_target_link_libraries(cxhtop)

set_target_properties(cxhtop PROPERTIES FOLDER ccxx/lib)
