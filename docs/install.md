
## Installation Options

The following compilers are supported:

- GCC 11.4 or higher
- Clang 16.0 or higher
- MSVC 17.8 (19.38) or higher

### 1. Header-only

If you **do not** need JSON support or you want to link YYJSON yourself, then reflect-cpp is header-only. Simply copy the contents of the folder `include` into your source repository or add it to your include path.

### 2. Include source files into your own build

Simply copy the contents of the folder `include` into your source repository or add it to your include path and also add `src/yyjson.c` to your source files for compilation.

If you need support for other serialization formats like flexbuffers or XML, you should also include and link the respective libraries, as listed in the section on serialization formats.

### 3. Compilation using cmake

This will simply compile YYJSON, which is the JSON library underlying reflect-cpp. You can then include reflect-cpp in your project and link to the binary
to get reflect-cpp with JSON support.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 4  # gcc, clang
cmake --build build --config Release -j 4  # MSVC
```

### 4. Compilation using cmake and vcpkg

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
set(REFLECTCPP_XML ON) # Optional
set(REFLECTCPP_YAML ON) # Optional

target_link_libraries(your_project PRIVATE reflectcpp) # Link against the library
```

##### Troubleshooting vcpkg

vcpkg is a great, but very ambitious and complex project (just like C++ is a great, but very ambitious and complex language). Here are some of the you might run into and how to resolve them:

1. A lot of problems can simply be resolved by deleting the build directory using `rm -rf build`.

2. *Environment variable VCPKG_FORCE_SYSTEM_BINARIES must be set on arm, s390x, ppc64le and riscv platforms.* - This usually happens on arm platforms like the Apple Silicon chips and can be resolved by simply preceding your build with `export VCPKG_FORCE_SYSTEM_BINARIES=arm` or `export VCPKG_FORCE_SYSTEM_BINARIES=1`.

3. On some occasions you might be asked to specify a compiler. You can do so by simply adding it to the cmake command as follows: `cmake -S . -B build ... -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++` or `cmake -S . -B build ... -DCMAKE_C_COMPILER=clang-17 -DCMAKE_CXX_COMPILER=clang++-17` (or whatever supported compiler you would like to use).
