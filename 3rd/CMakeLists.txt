
if (gs_project_enable_uv)
    add_subdirectory(./libuv)
    include_directories(${gs_libuv_include_path})
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
    add_subdirectory(./yaml-cpp/yaml-cpp)
    include_directories(${gs_yamlcpp_include_path})
endif ()

if (gs_project_enable_hiredis)
    add_subdirectory(hiredis)
    include_directories(${gs_hiredis_include_path})
endif ()
