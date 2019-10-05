add_executable(
    test_value
    testmain.cc
    test_array.cc
    test_object.cc
    test_primitive.cc
    test_value.cc
    test_key.cc)

target_link_libraries(
    test_value
    PRIVATE
        gtest
        jsoncpp)

set_target_properties(
    test_value
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})