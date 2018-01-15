
set(gs_ccxx_test_env_path ${gs_ccxx_path}/test/env)

if (${gs_project_enable_qt})
    add_subdirectory(qtui1)
endif()

if (WIN32)
    add_subdirectory(windows)
endif ()

