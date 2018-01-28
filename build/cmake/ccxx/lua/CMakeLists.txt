
include(lua.cmake)

mc_clua_include()
mc_cxlua_base_include()
mc_cxlua_all_include()

add_library(libcxlua SHARED
        ${gsl_clua_header_filepaths} ${gsl_clua_source_filepaths}
        ${gsl_cxlua_base_head_filepaths} ${gsl_cxlua_base_source_filepaths}
        ${gsl_cxlua_all_head_filepaths} ${gsl_cxlua_all_source_filepaths}
        )

if (MSVC)
    add_definitions(-D"_CRT_SECURE_NO_WARNINGS")
    add_definitions( "/wd4819" )
endif ()

#add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo" )

if (WIN32)
    target_link_libraries(libcxlua wsock32 ws2_32 psapi)
elseif(APPLE)
    find_library(CFLIB CoreFoundation)
    target_link_libraries(libcxlua ${CFLIB})
    add_definitions(-DGUID_CFUUID)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
elseif(ANDROID)
    target_compile_definitions(xg PRIVATE GUID_ANDROID)
else()
    find_package(Libuuid REQUIRED)
    if (NOT LIBUUID_FOUND)
        message(FATAL_ERROR
                "You might need to run 'sudo apt-get install uuid-dev' or similar")
    endif()
    include_directories(${LIBUUID_INCLUDE_DIR})
    target_link_libraries(Libuuid ${LIBUUID_LIBRARY})
    add_definitions(-DGUID_LIBUUID)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
    target_link_libraries(libcxlua dl rt)
#    target_link_libraries(libcxlua dl rt uuid)
endif ()

set_target_properties(libcxlua PROPERTIES FOLDER ccxx/lua)
