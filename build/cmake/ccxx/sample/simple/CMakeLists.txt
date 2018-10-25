
mc_ccxx_simple_include()

###cxsample_simple
set(gsl_cxsample_simple_files
        simple/main.cpp
        )
mc_merge_file_path(${gs_ccxx_sample_path} "${gsl_cxsample_simple_files}" gsl_cxsample_simple_files_filepaths)
source_group("cxsample_simple" FILES ${gsl_cxsample_simple_files_filepaths})

add_executable(cxsample_simple
        ${gsl_cxsample_simple_files_filepaths}
        ${gsl_ccxx_simple_head_filepaths} ${gsl_ccxx_simple_source_filepaths}
        )

mc_target_link_libraries(cxsample_simple)

set_target_properties(cxsample_simple PROPERTIES FOLDER ccxx/sample)

#compile static
mc_build_with_cc_static()

