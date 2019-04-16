add_executable(
    test_token
    testmain.cpp
    TokenTest.cpp)
    
target_link_libraries(
    test_token
    PRIVATE
        gtest
        jsoncpp)
        
set_target_properties(
    test_token
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})