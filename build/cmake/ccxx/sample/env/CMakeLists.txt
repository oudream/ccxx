
mc_ccxx_env_include()

###cxsample_env
set(gsl_cxsample_env_files
        env/main.cpp
        )
mc_merge_file_path(${gs_ccxx_sample_path} "${gsl_cxsample_env_files}" gsl_cxsample_env_files_filepaths)
source_group("cxsample_env" FILES ${gsl_cxsample_env_files_filepaths})

add_executable(cxsample_env
        ${gsl_cxsample_env_files_filepaths}
        ${gsl_ccxx_env_head_filepaths} ${gsl_ccxx_env_source_filepaths}
        )

mc_target_link_libraries(cxsample_env)

set_target_properties(cxsample_env PROPERTIES FOLDER ccxx/sample)

#compile static
mc_build_with_cc_static()

