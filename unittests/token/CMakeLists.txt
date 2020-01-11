add_executable(
    test_token
    testmain.cc
    test_token.cc
    test_tokenizer.cc)

target_link_libraries(
    test_token
    PRIVATE
        gtest
        json)

set_target_properties(
    test_token
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})