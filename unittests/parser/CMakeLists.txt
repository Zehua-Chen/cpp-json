add_executable(
    test_parser
    testmain.cc
    test_parser.cc)

target_link_libraries(
    test_parser
    PRIVATE
        gtest
        jsoncpp)

target_compile_definitions(
    test_parser
    PRIVATE
        PATH_TESTFILE="${CMAKE_SOURCE_DIR}/unittests/resources/1.jsonc")

set_target_properties(
    test_parser
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})