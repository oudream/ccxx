
include(lua.cmake)

mc_clua_include()
mc_cxlua_base_include()
mc_cxlua_all_include()

add_definitions(-D"GM_BUILD_CCXX_LUA_LIB")

add_library(cxlua SHARED
        ${gsl_clua_header_filepaths} ${gsl_clua_source_filepaths}
        ${gsl_cxlua_base_head_filepaths} ${gsl_cxlua_base_source_filepaths}
        ${gsl_cxlua_all_head_filepaths} ${gsl_cxlua_all_source_filepaths}
        )

add_dependencies(cxlua
        ccxx
        )

target_link_libraries(cxlua
        ccxx
        )

if (MSVC)
    add_definitions(-D"_CRT_SECURE_NO_WARNINGS")
    add_definitions( "/wd4819" )
endif ()

#add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo" )

mc_target_link_libraries(cxlua)

set_target_properties(cxlua PROPERTIES FOLDER ccxx/lib)
