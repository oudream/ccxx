
set(gs_gtest_path ${gs_project_path}/gtest)

include(${gs_project_3rd_path}/googletest/googletest/cmake/internal_utils.cmake)
config_compiler_and_linker()  # Defined in internal_utils.cmake.

##FIND_PACKAGE(extlib REQUIRED)
#
#AUX_SOURCE_DIRECTORY(${gs_gtest_path}/cxstring DIR_SRCS)
#ADD_EXECUTABLE(gtest_cxstring ${DIR_SRCS})
#TARGET_LINK_LIBRARIES(gtest_cxstring gtest gtest_main ccxx)
#
#ADD_TEST(gtest_cxstring gtest_cxstring)
macro(mc_create_test1 sCxxName)
    aux_source_directory(${gs_gtest_path}/${sCxxName} gsl_${sCxxName}_DIR_SRCS)

    add_executable(gtest_${sCxxName} ${gsl_${sCxxName}_DIR_SRCS})

    add_dependencies(gtest_${sCxxName} gtest gtest_main ccxx)

    target_link_libraries(gtest_${sCxxName} gtest gtest_main ccxx)

    set_target_properties(gtest_${sCxxName} PROPERTIES FOLDER ccxx/gtest)

    add_test(gtest_${sCxxName} gtest_${sCxxName})
endmacro()


mc_create_test1(cxstring)

mc_create_test1(cxfile)
