---
hide:
    - navigation
---

# Installation

The following compilers are supported:
- GCC 11.4 or higher
- Clang 14.0 or higher
- MSVC 17.8 (19.38) or higher

You can include the source files into your build or compile it using cmake and vcpkg.

## Option 1: Using vcpkg

Refer to [this port](https://vcpkg.io/en/package/reflectcpp):

```bash
vcpkg install reflectcpp
```

or, if you are in manifest mode:

```bash
vcpkg add port reflectcpp
```

If the library cannot be found, please update vcpkg itself to make sure the newest ports are installed. You
might also want to update the `builtin-baseline` tag in the *vcpkg.json* of your project.

## Option 2: Using Conan

Simply use [this recipe](https://conan.io/center/recipes/reflect-cpp).

## Option 3: Compilation using cmake

This will compile reflect-cpp with JSON support only. You can then include reflect-cpp in your project and link to the binary.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 4  # gcc, clang
cmake --build build --config Release -j 4  # MSVC
```

## Option 4: Include source files into your own build

Simply copy the contents of the folders `include` and `src` into your source repository or add it to your include path and also add `src/reflectcpp.cpp` and `src/reflectcpp_json.cpp` and `src/yyjson.c` to your source files for compilation.
If you want to link to your own version of YYJSON, then only add `src/reflectcpp.cpp` and `src/reflectcpp_json.cpp`. If you don't need JSON support, then only add `src/reflectcpp.cpp`.

If you need support for other serialization formats like flexbuffers or XML, you should also add `src/reflectcpp_<format>.cpp` and include and link the respective libraries, as listed in the section on serialization formats.

## Option 5: Compilation using cmake and vcpkg

If you want serialization formats other than JSON, you can either install them manually or use vcpkg.

To install vcpkg:

```bash
git submodule update --init
./vcpkg/bootstrap-vcpkg.sh # Linux, macOS
./vcpkg/bootstrap-vcpkg.bat # Windows
# You may be prompted to install additional dependencies.
```

To use reflect-cpp in your project:

```cmake
add_subdirectory(reflect-cpp) # Add this project as a subdirectory

set(REFLECTCPP_BSON ON) # Optional
set(REFLECTCPP_CBOR ON) # Optional
set(REFLECTCPP_FLEXBUFFERS ON) # Optional
set(REFLECTCPP_MSGPACK ON) # Optional
set(REFLECTCPP_TOML ON) # Optional
set(REFLECTCPP_UBJSON ON) # Optional
set(REFLECTCPP_XML ON) # Optional
set(REFLECTCPP_YAML ON) # Optional

target_link_libraries(your_project PRIVATE reflectcpp) # Link against the library
```

### Troubleshooting vcpkg

vcpkg is a great, but very ambitious and complex project (just like C++ is a great, but very ambitious and complex language). Here are some of the you might run into and how to resolve them:

1. A lot of problems can simply be resolved by deleting the build directory using `rm -rf build`.

2. *Environment variable VCPKG_FORCE_SYSTEM_BINARIES must be set on arm, s390x, ppc64le and riscv platforms.* - This usually happens on arm platforms like the Apple Silicon chips and can be resolved by simply preceding your build with `export VCPKG_FORCE_SYSTEM_BINARIES=arm` or `export VCPKG_FORCE_SYSTEM_BINARIES=1`.

3. On some occasions you might be asked to specify a compiler. You can do so by simply adding it to the cmake command as follows: `cmake -S . -B build ... -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++` or `cmake -S . -B build ... -DCMAKE_C_COMPILER=clang-17 -DCMAKE_CXX_COMPILER=clang++-17` (or whatever supported compiler you would like to use).

## Option 6: Compilation using Conan

To install Conan (assuming you have Python and pipx installed):

```bash
pipx install conan
conan profile detect
```

For older versions of pip, you can also use `pip` instead of `pipx`.

To install the basic (JSON-only) version, cd into the `reflect-cpp`
repository and execute the following:

```bash
conan build . --build=missing -s compiler.cppstd=gnu20
```

You can call `conan inspect .` to get an overview of the supported options.

If you want to include all supported formats, do the following:

```bash
conan build . --build=missing -s compiler.cppstd=gnu20 -o *:with_cbor=True -o *:with_flatbuffers=True -o *:with_msgpack=True -o *:with_toml=True -o *:with_ubjson=True -o *:with_xml=True -o *:with_yaml=True
```
