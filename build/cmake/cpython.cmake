
### cpython2
if (DEFINED gs_project_cpython_version AND (gs_project_cpython_version LESS_EQUAL 3))
else()
    ### cpython3
    find_package (PythonInterp REQUIRED)
    find_package (PythonLibs REQUIRED)
    include_directories(${PYTHON_INCLUDE_DIRS})
endif ()


