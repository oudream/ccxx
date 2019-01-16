
mc_target_define_cpython()

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

target_link_libraries(noddy
        ${PYTHON_LIBRARIES}
        )

mc_target_link_libraries(noddy)

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

target_link_libraries(shoddy
        ${PYTHON_LIBRARIES}
        )

mc_target_link_libraries(shoddy)


set_target_properties(shoddy PROPERTIES FOLDER ccxx/test)

#compile static
mc_build_with_cc_static()


