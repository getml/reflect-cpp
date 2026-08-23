# C++26 reflection

reflect-cpp can be compiled in two modes, which use two different reflection implementations:

* **C++-20/23 mode (the default):** reflection is implemented using metaprogramming
  techniques (structured bindings, aggregate-initialization analysis, and
  [enchantum](https://github.com/ZXShady/enchantum)). This works on a wide range of
  compilers, but it has some limitations and comes with a higher compile-time cost.
* **C++-26 mode:** reflection uses the standard C++ reflection facilities
  (`<meta>`, [P2996](https://wg21.link/P2996)). This is more powerful, because members
  and enumerators are queried directly from the compiler instead of being inferred.

C++-26 mode is opt-in. It requires a compiler that supports the C++ reflection proposal
and a compiler flag to activate it. The rest of this documentation applies to both modes,
except where explicitly noted otherwise.

## Compiling with C++-26 reflection

The following compilers are supported for C++-26:

* GCC 16.2 or higher
* Clang: experimental, only available in Clang builds that implement
  [P2996](https://wg21.link/P2996) (such as Bloomberg's
  [clang-p2996](https://github.com/bloomberg/clang-p2996) fork)

To compile reflect-cpp with C++-26 reflection, pass the CMake option
`REFLECTCPP_USE_CPP26_REFLECTION` and the compiler flag that activates reflection
support in your compiler:

* GCC: `-freflection`
* Clang: `-freflection-latest`

For example, using cmake:

```bash
cmake -S . -B build -DCMAKE_CXX_STANDARD=26 -DCMAKE_BUILD_TYPE=Release -DREFLECTCPP_USE_CPP26_REFLECTION=ON -DCMAKE_CXX_FLAGS="-freflection"
cmake --build build -j 4
```

The individual parts:

* `-DREFLECTCPP_USE_CPP26_REFLECTION=ON` switches reflect-cpp to the C++-26 reflection
  implementation and defines the macro `REFLECTCPP_USE_CPP26_REFLECTION`. If
  `CMAKE_CXX_STANDARD` is not set, it will be set to 26 automatically.
* `-DCMAKE_CXX_STANDARD=26` sets the C++ standard to 26.
* `-DCMAKE_CXX_FLAGS="-freflection"` activates the C++ reflection facilities in the
  compiler itself. Use `-freflection-latest` on Clang instead.

When compiling with C++-26 reflection, the option `REFLECTCPP_USE_STD_EXPECTED` is
enabled by default as well, which means that `rfl::Result<T>` is an alias for
`std::expected<T, rfl::Error>`. If you want to keep using the built-in `rfl::Result`
type, pass `-DREFLECTCPP_USE_STD_EXPECTED=OFF`.

If you include the source files directly into your own build (see
[Installation](install.md#option-4-include-source-files-into-your-own-build)), add the
compile definition `-DREFLECTCPP_USE_CPP26_REFLECTION` and the appropriate compiler flag
to all translation units that include reflect-cpp.

## What C++-26 reflection changes

### C arrays and inheritance are supported out of the box

In C++-20/23 mode, fixed-size C arrays and inheritance are only supported when you pass
the flag `-DREFLECT_CPP_C_ARRAYS_OR_INHERITANCE`, and inheritance only works when all of
the fields are inside the same struct. Refer to
[C arrays and inheritance](c_arrays_and_inheritance.md) for details.

In C++-26 mode, neither restriction applies:

* Fixed-size C arrays are regular fields as far as the compiler's reflection is concerned,
  so they work without any flag.
* The fields of base classes are combined with the fields of the derived class, so the
  fields may be spread out over multiple structs.

For example, this is not supported in C++-20/23 mode, but works as-is in C++-26 mode:

```cpp
struct Base {
  int x;
};

struct Derived : Base {
  int y;
};

const auto derived = Derived{1, 2};

rfl::json::write(derived);
```

This results in the following JSON string:

```json
{"x":1,"y":2}
```

### No range restrictions for enums

In C++-20/23 mode, enum values must be in the range `[RFL_ENUM_RANGE_MIN,
RFL_ENUM_RANGE_MAX]`, where the default range is `[-256, 256]`. Refer to
[Enums](enums.md) for details.

In C++-26 mode, the enumerators are read directly from the compiler, so there is no
restriction on the range of enum values, and `RFL_ENUM_RANGE_MIN` and `RFL_ENUM_RANGE_MAX`
are not needed.

### Lower compile-time cost

In C++-20/23 mode, reflect-cpp infers the fields of a struct by figuring out how the
struct can be constructed. This requires a lot of compile-time work, especially in the
presence of C arrays and inheritance. In C++-26 mode, the fields and enumerators are
queried directly from the compiler's reflection information, which is considerably
cheaper.

## Summary

| Feature | C++-20/23 | C++-26 |
|---------|-----------|--------|
| Supported compilers | GCC 11.4+, Clang 14.0+, MSVC 17.8+ | GCC 16.2+ (Clang: experimental) |
| Compiler flag | *(none)* | `-freflection` (GCC), `-freflection-latest` (Clang) |
| CMake option | *(none)* | `-DREFLECTCPP_USE_CPP26_REFLECTION=ON` |
| Fixed-size C arrays | requires `-DREFLECT_CPP_C_ARRAYS_OR_INHERITANCE` | supported out of the box |
| Inheritance | requires `-DREFLECT_CPP_C_ARRAYS_OR_INHERITANCE`, fields must be in a single struct | supported out of the box, fields may be spread over multiple structs |
| Enum value range | restricted to `[RFL_ENUM_RANGE_MIN, RFL_ENUM_RANGE_MAX]` (default `[-256, 256]`) | no restriction |
| `rfl::Result` | built-in type (or `std::expected` via `-DREFLECTCPP_USE_STD_EXPECTED`) | `std::expected` by default |
| Compile time | higher | lower |
