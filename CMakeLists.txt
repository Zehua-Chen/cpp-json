cmake_minimum_required(VERSION 3.11)
project("json")

option(JSON_DEV "set to 1 to also setup testing" 0)

if(JSON_DEV EQUAL 1)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0)

    FetchContent_GetProperties(googletest)

    if(NOT googletest_POPULATED)
        # get google test if goole test not added
        FetchContent_Populate(googletest)

        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()

    # add test targets
    add_subdirectory(unittests)
endif()

add_library(json INTERFACE)

target_include_directories(
    json
    INTERFACE
        "./include/")

target_compile_features(
    json
    INTERFACE
        cxx_std_17)
