
mc_ccxx_include()

#use cc4000 's path construction
add_definitions(-D"PROJECT_PLATFORM_CC4000")
add_definitions(-D"GM_BUILD_CCXX_CORE_LIB")

add_library(ccxx SHARED ${gsl_ccxx_head_filepaths} ${gsl_ccxx_source_filepaths})

if (MSVC)
    add_definitions(-D"_CRT_SECURE_NO_WARNINGS")
    add_definitions( "/wd4819" )
endif ()

#add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo" )

mc_target_link_libraries(ccxx)

set_target_properties(ccxx PROPERTIES FOLDER ccxx/lib)
