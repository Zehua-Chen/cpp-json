cmake_minimum_required(VERSION 3.0)

project("json-cpp")

add_subdirectory(dependencies/googletest/googletest)

set(
    HEADERS
    "include/Json/Assembler.hpp"
    "include/Json/BasicValue.hpp"
    "include/Json/Json.hpp"
    "include/Json/Keywords.hpp"
    "include/Json/Parse.hpp"
    "include/Json/Token.hpp"
    "include/Json/Lexer.hpp")

add_executable(
    test
    # "test/AssemblerTest.cpp"
    "test/testmain.cpp"
    "test/BasicObjectTest.cpp"
    "test/BasicArrayTest.cpp"
    "test/BasicPrimitiveTest.cpp"
    "test/BasicValueTest.cpp"
    # "test/EncodingTest.cpp"
    # "test/ParseTest.cpp"
    "test/TokenTest.cpp"
    "test/LexerTest.cpp"
    ${HEADERS})
    
target_link_libraries(
    test
    PRIVATE 
      gtest)
    
# Gtest's include already added through linking with gtest
target_include_directories(
    test
    PRIVATE
      "include/")

set_target_properties(
    test
    PROPERTIES
      CXX_STANDARD 17)
      
# If build test as shared library
if (BUILD_SHARED_LIBS)
    target_compile_definitions(
        test
        PRIVATE
          GTEST_LINKED_AS_SHARED_LIBRARY=1)
endif()