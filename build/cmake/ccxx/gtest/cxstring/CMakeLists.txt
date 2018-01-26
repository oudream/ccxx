#FIND_PACKAGE(extlib REQUIRED)

AUX_SOURCE_DIRECTORY(. DIR_SRCS)
ADD_EXECUTABLE(gtest_cxstring ${DIR_SRCS})
TARGET_LINK_LIBRARIES(gtest_cxstring gtest gtest_main libccxx)

ADD_TEST(gtest_cxstring gtest_cxstring)
