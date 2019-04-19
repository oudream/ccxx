
if (${gs_project_enable_uv})
    include(${gs_project_path}/3rd/uv.cmake)
endif ()


if (${gs_project_enable_yamlcpp})
    include(${gs_project_path}/3rd/yamlcpp.cmake)
endif ()


if (${gs_project_enable_hiredis})
    include(${gs_project_path}/3rd/hiredis.cmake)
endif ()

