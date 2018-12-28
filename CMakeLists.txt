add_subdirectory(dependencies/googletest/googletest)

add_executable(
    test
    "test/BasicObjectTest.cpp"
    "test/BasicArrayTest.cpp"
    "test/BasicPrimitiveTest.cpp")
    
target_link_libraries(
    test
    PRIVATE 
      gtest gtest_main)
    
target_include_directories(
    test
    PRIVATE
      "include/")

set_target_properties(
    test
    PROPERTIES
      CXX_STANDARD 17)