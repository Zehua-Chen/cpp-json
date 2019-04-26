# Json Library

Json parser designed to handle different string encoding of C++:
* `char`;
* `char16_t`;
* `char32_t`;
* `wchar_t`;

## Requirements

* C++17 compatible compilers

## Usage

You are welcome to add the build system you use! Just open a pull request!

### CMake

* Download the repository (ex. `FetchContent`)
* Link the `jsoncpp` library target to your targets

````cmake
include(FetchContent)

# declare jsoncpp
FetchContent_Declare(
    jsoncpp
    GIT_REPOSITORY https://github.com/Zehua-Chen/json-cpp
    GIT_TAG version)

# see if jsoncpp has been downloaded
FetchContent_GetProperties(jsoncpp)

if(NOT googletest_POPULATED)
    # download json cpp if not
    FetchContent_Populate(
        googletest)
    # add jsoncpp's CMakeLists as a sub directory
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

target_link_libraries(
    your_target
    PRIVATE
        jsoncpp)
````

### Other Build Systems

* Download the repository
* Add "./include" to your include path
* Include the umbrella header `json.hpp` and start parsing json!

## Quick Start

````cpp

#include "json/json.hpp"
#include <string>
#include <iostream>

using namespace json;

int main()
{
    string json = "{ 'key': 'value' }";
    Value value = parse(json.begin(), json.end());
    
    std::cout << value["key"].string() << std::endl;
    
    return 0;
}

````

## Known Limitations

* Only string to json, and not the other way around;
* `BasicValue<CharT>` does not have iterators
* `BasicValue<CharT>` only have basic data access methods;

## Development

### Setup

* When generating build files using cmake, define `JSONCPP_DEV` to be 1

````
cmake .. -G Ninja -DJSONCPP_DEV=1
````
    
#### Windows

`ParseTest.cpp` may not work as expected on Windows, as the file path used to access the json file follows Linux/Unix format.