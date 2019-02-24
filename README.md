# Json Library

Json parser designed to handle different string encoding of C++:
* `char`;
* `char16_t`;
* `char32_t`;
* `wchar_t`;

## Requirements

* C++17 compatible compilers

## Usage

* Download the repository
* Add "./include" to your include path
* Include the umbrella header `Json.hpp` and start parsing json!

````cpp

#include "Json/Json.hpp"
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

* No syntax validation;
* Only string to json, and not the other way around;
* `BasicValue<CharT>` is a little big (64 byte when `CharT` is `char` and compiled with clang++ on macOS)
* `BasicValue<CharT>` does not have iterators
* `BasicValue<CharT>` only have basic data access methods;

## Testing

### Setup

* Navigate to `scripts` folder and run `setup-dev.js` using `node`;
    * You should see a `dependencies` folder afterwards;
* Make a `build` folder and generate a native build file using `cmake`
    * To compile google test as a shared library, add `-DBUILD_SHARED_LIBS=1` to `cmake` when generating the solution.
    
#### Windows

`ParseTest.cpp` may not work as expected on Windows, as the file path used to access the json file follows Linux/Unix format.