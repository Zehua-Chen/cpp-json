add_executable(
    test_parser
    testmain.cpp
    ComposerTest.cpp)
    
target_link_libraries(
    test_parser
    PRIVATE
        gtest
        jsoncpp)
        
set_target_properties(
    test_parser
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})