
find_package (PythonInterp REQUIRED)
find_package (PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

### noddy
set(gsl_noddy_files
        python/noddy1.h
        python/noddy1.cpp
        python/noddy2.h
        python/noddy2.cpp
        python/noddy3.h
        python/noddy3.cpp
        python/noddy4.h
        python/noddy4.cpp
        python/noddy_config.h
        python/noddy_config.cpp
        python/noddy_module.cpp
        )
mc_merge_file_path(${gs_ccxx_test_path} "${gsl_noddy_files}" gsl_noddy_files_filepaths)
source_group("noddy" FILES ${gsl_noddy_files_filepaths})

add_library(noddy SHARED
        ${gsl_noddy_files_filepaths}
        )

if (WIN32)
#    target_link_libraries(noddy wsock32 ws2_32 psapi ${PYTHON_LIBRARIES})
    if (MSVC)
        target_link_libraries(noddy
                ${PYTHON_LIBRARIES}
                )
    else()
        #        target_link_libraries( IcsRt2Stack debug ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}/libgcl_sdk.dll)
        #        target_link_libraries( IcsRt2Stack optimized ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}/libgcl_sdk.dll)
        target_link_libraries(noddy
                C:/Python/Python36-32/libs/libpython36.a
                )
    endif()
else()
    target_link_libraries(noddy dl rt uuid ${PYTHON_LIBRARIES})
endif ()

set_target_properties(noddy PROPERTIES FOLDER ccxx/test)



### shoddy
set(gsl_shoddy_files
        python/shoddy1.h
        python/shoddy1.cpp
        python/shoddy_config.hpp
        python/shoddy_module.cpp
        )
mc_merge_file_path(${gs_ccxx_test_path} "${gsl_shoddy_files}" gsl_shoddy_files_filepaths)
source_group("shoddy" FILES ${gsl_shoddy_files_filepaths})

add_library(shoddy SHARED
        ${gsl_shoddy_files_filepaths}
        )

if (WIN32)
    #    target_link_libraries(shoddy wsock32 ws2_32 psapi ${PYTHON_LIBRARIES})
    if (MSVC)
        target_link_libraries(shoddy
                ${PYTHON_LIBRARIES}
                )
    else()
        #        target_link_libraries( IcsRt2Stack debug ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}/libgcl_sdk.dll)
        #        target_link_libraries( IcsRt2Stack optimized ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}/libgcl_sdk.dll)
        target_link_libraries(shoddy
                C:/Python/Python36-32/libs/libpython36.a
                )
    endif()
else()
    target_link_libraries(shoddy dl rt uuid ${PYTHON_LIBRARIES})
endif ()

set_target_properties(shoddy PROPERTIES FOLDER ccxx/test)

#compile static
mc_build_with_cc_static()


