# Json Library

Json parser designed to handle different string encoding of C++:
* `char`;
* `char16_t`;
* `char32_t`;
* `wchar_t`;

## Testing

### Setup

* Navigate to `scripts` folder and run `setup-dev.js` using `node`;
    * You should see a `dependencies` folder afterwards;
* Make a `build` folder and generate a native build file using `cmake`
    * To compile google test as a shared library, add `-DBUILD_SHARED_LIBS=1` to `cmake` when generating the solution.