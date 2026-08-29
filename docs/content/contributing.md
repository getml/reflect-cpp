# How to contribute

## Setup

Make sure includes are relative.

We need internal includes to be relative and not depend on any externally set include directory.

That is, for example, if you are within any file in `rfl/internal`, prefer
```cpp
#include "to_ptr_named_tuple.hpp"
```
over
```cpp
#include "rfl/internal/to_ptr_named_tuple.hpp"
```
For further details and reasoning, please refer to [#30](https://github.com/getml/reflect-cpp/issues/30).

Headers should be self-contained. This will be checked by the CI.

## Compiling and running the tests

reflect-cpp uses vcpkg for dependency management, including
gtest, which is required for the tests.

```bash
# bootstrap vcpkg if you haven't done so already
git submodule update --init
./vcpkg/bootstrap-vcpkg.sh # Linux, macOS
./vcpkg/bootstrap-vcpkg.bat # Windows
# You may be prompted to install additional dependencies.
```

## JSON only

To compile the tests, do the following:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DREFLECTCPP_BUILD_TESTS=ON
cmake --build build -j 4 # gcc, clang
cmake --build build --config Release -j 4 # MSVC
```

To run the tests, do the following:

```
./build/tests/json/reflect-cpp-json-tests
```

## All serialization formats

To compile the tests with serialization formats other than JSON, do the following:

```bash
cmake -S . -B build -DREFLECTCPP_BUILD_TESTS=ON -DREFLECTCPP_ALL_FORMATS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 4 # gcc, clang
cmake --build build --config Release -j 4 # MSVC
```

To run the tests, do the following:

```
./build/tests/bson/reflect-cpp-bson-tests
./build/tests/cbor/reflect-cpp-cbor-tests
./build/tests/flexbuffers/reflect-cpp-flexbuffers-tests
...
```

Please make sure that the tests pass before submitting a pull request. Your changes should be covered by the tests.
