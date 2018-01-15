
mc_ccxx_simple_include()

###ccxx_sample_simple
set(gsl_ccxx_sample_simple_files
        simple/main.cpp
        )
mc_merge_file_path(${gs_ccxx_sample_path} "${gsl_ccxx_sample_simple_files}" gsl_ccxx_sample_simple_files_filepaths)
source_group("ccxx_sample_simple" FILES ${gsl_ccxx_sample_simple_files_filepaths})

add_executable(ccxx_sample_simple
        ${gsl_ccxx_sample_simple_files_filepaths}
        ${gsl_ccxx_simple_head_filepaths} ${gsl_ccxx_simple_source_filepaths}
        )

if (WIN32)
    target_link_libraries(ccxx_sample_simple wsock32 ws2_32 psapi)
endif ()

if (UNIX)
    target_link_libraries(ccxx_sample_simple dl rt uuid)
endif ()

set_target_properties(ccxx_sample_simple PROPERTIES FOLDER ccxx/sample)

#compile static
mc_build_with_cc_static()

