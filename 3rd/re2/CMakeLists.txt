
if (${gs_project_enable_uv})
    add_subdirectory(./libuv-2018-04-26)
endif()

if((NOT MSVC_VERSION) OR (MSVC_VERSION GREATER_EQUAL 1900))
    add_subdirectory(./re2-2018-04-01)
endif()

if(NOT WIN32)
    add_subdirectory(htop)
endif()