
mc_ccxx_env_include()

###ccxx_sample_env
set(gsl_ccxx_sample_env_files
        env/main.cpp
        )
mc_merge_file_path(${gs_ccxx_sample_path} "${gsl_ccxx_sample_env_files}" gsl_ccxx_sample_env_files_filepaths)
source_group("ccxx_sample_env" FILES ${gsl_ccxx_sample_env_files_filepaths})

add_executable(ccxx_sample_env
        ${gsl_ccxx_sample_env_files_filepaths}
        ${gsl_ccxx_env_head_filepaths} ${gsl_ccxx_env_source_filepaths}
        )

set_target_properties(ccxx_sample_env PROPERTIES FOLDER ccxx/sample)

#compile static
mc_build_with_cc_static()

