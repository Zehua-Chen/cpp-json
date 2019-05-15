add_executable(
    test_value
    testmain.cpp
    ArrayTest.cpp
    ObjectTest.cpp
    PrimitiveTest.cpp
    ValueTest.cpp
    KeyTest.cpp)
    
target_link_libraries(
    test_value
    PRIVATE
        gtest
        jsoncpp)
        
set_target_properties(
    test_value
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})