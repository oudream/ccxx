
### cpython2
if (DEFINED gs_project_cpython_version AND (gs_project_cpython_version LESS_EQUAL 3))
else()
    ### cpython3
    find_package (PythonInterp REQUIRED)
    find_package (PythonLibs REQUIRED)
endif ()

if (WIN32 AND NOT MSVC AND gs_prefix_path_python)
    #        target_link_libraries( IcsRt2Stack debug ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}/libgcl_sdk.dll)
    #        target_link_libraries( IcsRt2Stack optimized ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}/libgcl_sdk.dll)
    set(PYTHON_LIBRARIES ${gs_prefix_path_python}/libs/libpython36.a)
endif()


macro(mc_target_define_cpython)
    ### cpython2
    if (DEFINED gs_project_cpython_version AND (gs_project_cpython_version LESS_EQUAL 3))
    else()
        ### cpython3
        include_directories(${PYTHON_INCLUDE_DIRS})
    endif ()
endmacro()