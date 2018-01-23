
#cmake . -G "Visual Studio 12 2013" --build "C:\ddd\ccpp\googletest\googletest-master" -B"C:\ddd\ccpp\googletest\googletest-master\cmake-vs2013"
#cmake . -G "Visual Studio 14 2015" --build "C:\ddd\ccpp\googletest\googletest-master" -B"C:\ddd\ccpp\googletest\googletest-master\build\cmake-vs2015" -Dgtest_build_tests=ON -Dgtest_build_samples=ON -Dgmock_build_tests=ON
#-DCMAKE_BUILD_TYPE=Debug -Dgtest_build_tests=ON -Dgtest_build_samples=ON -Dgmock_build_tests=ON
#-DCMAKE_BUILD_TYPE=Release -Dgtest_build_tests=ON -Dgtest_build_samples=ON -Dgmock_build_tests=ON
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-narrowing -Wa,-mbig-obj")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-narrowing -std=c++11 -Wa,-mbig-obj")

set(gs_gtest_path ${gs_project_path}/gtest)

