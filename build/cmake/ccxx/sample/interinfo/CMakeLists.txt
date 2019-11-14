
mc_ccxx_interinfo_include()

###cxsample_interinfo
set(gsl_cxsample_interinfo_files
        interinfo/main.cpp
        )
mc_merge_file_path(${gs_ccxx_sample_path} "${gsl_cxsample_interinfo_files}" gsl_cxsample_interinfo_files_filepaths)
source_group("cxsample_interinfo" FILES ${gsl_cxsample_interinfo_files_filepaths})

add_executable(cxsample_interinfo
        ${gsl_cxsample_interinfo_files_filepaths}
        ${gsl_ccxx_interinfo_head_filepaths} ${gsl_ccxx_interinfo_source_filepaths}
        )

mc_target_link_libraries(cxsample_interinfo)

set_target_properties(cxsample_interinfo PROPERTIES FOLDER ccxx/sample)

#compile static
mc_build_with_cc_static()

