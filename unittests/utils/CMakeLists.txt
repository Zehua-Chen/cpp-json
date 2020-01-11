add_executable(
    test_utils
    testmain.cc
    test_convert.cc)

target_link_libraries(
    test_utils
    PRIVATE
        gtest
        json)

set_target_properties(
    test_utils
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})