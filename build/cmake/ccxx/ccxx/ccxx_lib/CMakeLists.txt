
mc_ccxx_include()

#use cc4000 's path construction
add_definitions(-D"PROJECT_PLATFORM_CC4000")
add_definitions(-D"GM_BUILD_CCXX_CORE_LIB")

add_library(libccxx SHARED ${gsl_ccxx_head_filepaths} ${gsl_ccxx_source_filepaths})

if (MSVC)
    add_definitions(-D"_CRT_SECURE_NO_WARNINGS")
    add_definitions( "/wd4819" )
endif ()

#add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo" )

if (WIN32)
    target_link_libraries(libccxx wsock32 ws2_32 psapi)
endif ()

if (UNIX)
    target_link_libraries(libccxx dl rt uuid)
endif ()

set_target_properties(libccxx PROPERTIES FOLDER ccxx/lib)
