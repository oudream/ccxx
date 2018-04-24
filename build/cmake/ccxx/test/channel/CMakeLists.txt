###environment

set(fslSources
        channel/test_channel_udp1.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(cxtest_channel
        ${fslSourcePaths}
        )

add_dependencies(cxtest_channel
        ccxx
        )

target_link_libraries(cxtest_channel
        ccxx
        )

set_target_properties(cxtest_channel PROPERTIES FOLDER ccxx/test)



### test channel udp client
set(fslUdpClient
        channel/test_channel_udp_client1.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslUdpClient}" fslUdpClientPath)

add_executable(cxtest_udp_client
        ${fslUdpClientPath}
        )

add_dependencies(cxtest_udp_client
        ccxx
        )

target_link_libraries(cxtest_udp_client
        ccxx
        )

set_target_properties(cxtest_udp_client PROPERTIES FOLDER ccxx/test)



### test channel udp server
set(fslUdpServer
        channel/test_channel_udp_server1.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslUdpServer}" fslUdpServerPath)

add_executable(cxtest_udp_server
        ${fslUdpServerPath}
        )

add_dependencies(cxtest_udp_server
        ccxx
        )

target_link_libraries(cxtest_udp_server
        ccxx
        )

set_target_properties(cxtest_udp_server PROPERTIES FOLDER ccxx/test)




### test channel serial
set(fslSerial
        channel/test_channel_serial1.cpp
#        channel/test_serial1.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSerial}" fslSerialPath)

add_executable(cxtest_serial
        ${fslSerialPath}
        )

add_dependencies(cxtest_serial
        ccxx
        )

target_link_libraries(cxtest_serial
        ccxx
        )

set_target_properties(cxtest_serial PROPERTIES FOLDER ccxx/test)

