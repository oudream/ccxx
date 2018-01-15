
mc_ccxx_interinfo_include()

###ccxx_sample_interinfo
set(gsl_ccxx_sample_interinfo_files
        interinfo/main.cpp
        )
mc_merge_file_path(${gs_ccxx_sample_path} "${gsl_ccxx_sample_interinfo_files}" gsl_ccxx_sample_interinfo_files_filepaths)
source_group("ccxx_sample_interinfo" FILES ${gsl_ccxx_sample_interinfo_files_filepaths})

add_executable(ccxx_sample_interinfo
        ${gsl_ccxx_sample_interinfo_files_filepaths}
        ${gsl_ccxx_interinfo_head_filepaths} ${gsl_ccxx_interinfo_source_filepaths}
        )

set_target_properties(ccxx_sample_interinfo PROPERTIES FOLDER ccxx/sample)

#compile static
mc_build_with_cc_static()

