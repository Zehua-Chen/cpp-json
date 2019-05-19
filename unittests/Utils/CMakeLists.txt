add_executable(
    test_utils
    testmain.cpp
    ConvertTest.cpp)
    
target_link_libraries(
    test_utils
    PRIVATE
        gtest
        jsoncpp)
        
set_target_properties(
    test_utils
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})