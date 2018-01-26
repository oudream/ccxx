#FIND_PACKAGE(extlib REQUIRED)

AUX_SOURCE_DIRECTORY(../src TEST_SRCS)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
ADD_EXECUTABLE(test_account ${TEST_SRCS} ${DIR_SRCS})
TARGET_LINK_LIBRARIES(test_account gmock gmock_main)

ADD_TEST(test_account test_account)
