
include_directories(${PYTHON_INCLUDE_DIRS})

### noddy
set(gsl_noddy_files
        noddy1.h
        noddy1.cpp
        noddy2.h
        noddy2.cpp
        noddy3.h
        noddy3.cpp
        noddy4.h
        noddy4.cpp
        noddy_config.h
        noddy_config.cpp
        noddy_module.cpp
        )
mc_merge_file_path(${gs_ccxx_test_path}/python/noddy "${gsl_noddy_files}" gsl_noddy_files_filepaths)
source_group("noddy" FILES ${gsl_noddy_files_filepaths})

add_library(noddy SHARED
        ${gsl_noddy_files_filepaths}
        )

target_link_libraries(noddy
        ${PYTHON_LIBRARIES}
        )

mc_target_link_libraries(noddy)

set_target_properties(noddy PROPERTIES FOLDER ccxx/test)



### shoddy
set(gsl_shoddy_files
        shoddy1.h
        shoddy1.cpp
        shoddy_config.hpp
        shoddy_module.cpp
        )
mc_merge_file_path(${gs_ccxx_test_path}/python/shoddy "${gsl_shoddy_files}" gsl_shoddy_files_filepaths)
source_group("shoddy" FILES ${gsl_shoddy_files_filepaths})

add_library(shoddy SHARED
        ${gsl_shoddy_files_filepaths}
        )

target_link_libraries(shoddy
        ${PYTHON_LIBRARIES}
        )

mc_target_link_libraries(shoddy)


set_target_properties(shoddy PROPERTIES FOLDER ccxx/test)

#compile static
mc_build_with_cc_static()


