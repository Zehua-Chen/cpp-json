add_subdirectory(dependencies/googletest/googletest)

add_executable(
    test
    test/SampleTest.cpp)
    
target_link_libraries(
    test
    PRIVATE gtest gtest_main)

set_target_properties(
    test
    PROPERTIES
      CXX_STANDARD 17)