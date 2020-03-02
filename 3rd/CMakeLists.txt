
if (gs_project_enable_uv)
    include_directories(${gs_libuv_include_path})
    add_subdirectory(./libuv)
endif ()

add_subdirectory(./re2)

if (gs_project_enable_htop)
    if (NOT WIN32)
        add_subdirectory(./htop)
    endif ()
endif ()

add_subdirectory(./googletest)

add_subdirectory(./benchmark)

if (gs_project_enable_yamlcpp)
    include_directories(${gs_yamlcpp_include_path})
    add_subdirectory(./yaml-cpp/yaml-cpp)
endif ()

if (gs_project_enable_hiredis)
    include_directories(${gs_hiredis_include_path})
    add_subdirectory(hiredis)
endif ()
