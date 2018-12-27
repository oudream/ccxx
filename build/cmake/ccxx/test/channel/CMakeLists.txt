###environment

macro(mc_create_test1 sCppName)
    set(gsl_test_${sCppName}_sources
            channel/main_${sCppName}.cpp
            )

    mc_merge_file_path(${gs_ccxx_test_path} "${gsl_test_${sCppName}_sources}" gsl_test_${sCppName}_source_filepaths)

    add_executable(cxtest_channel_${sCppName}
            ${gsl_test_${sCppName}_head_filepaths} ${gsl_test_${sCppName}_source_filepaths}
            )

    add_dependencies(cxtest_channel_${sCppName}
            ccxx
            )

    target_link_libraries(cxtest_channel_${sCppName}
            ccxx
            )

    set_target_properties(cxtest_channel_${sCppName} PROPERTIES FOLDER ccxx/test)
endmacro()

mc_create_test1(channel_serial1)
mc_create_test1(channel_tcpclient1)
mc_create_test1(channel_tcpclient2)
mc_create_test1(channel_tcpserver1)
mc_create_test1(channel_tcpserver2)
mc_create_test1(channel_udp1)
mc_create_test1(channel_udp_client1)
mc_create_test1(channel_udp_server1)
