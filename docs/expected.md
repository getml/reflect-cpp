# `std::expected`

C++-23 introduced `std::expected` as the standard way of expressing the result of an operation that might fail. Unlike `std::optional`, it holds both the value on success (of type `T`) and the error on failure (of type `E`). reflect-cpp supports `std::expected` out of the box: you can use it as a top-level type, as a field of a struct, or inside containers.

## Availability

`std::expected` is a C++-23 feature. reflect-cpp detects it via the feature-test macro `__cpp_lib_expected`. Note that some standard libraries only expose `<expected>` when compiled in C++-23 mode — for instance, GCC's libstdc++ requires `-std=c++23`. If your standard library does not provide `std::expected`, reflect-cpp will not recognize the type, and attempting to serialize one will result in a compile-time error.

## How `std::expected` is serialized

A `std::expected<T, E>` is serialized as the two alternatives of an untagged variant (an `rfl::Variant`):

- If it holds a value, the value is written as-is, i.e. exactly the same way as it would be written if it were of type `T`.
- If it holds an error, an object with a single field named `error` is written, containing the value of type `E`.

Wrapping the error in an object makes sure that the error is always recognized as an object, even if `T` itself is a struct.

```cpp
#include <expected>
#include <rfl/json.hpp>

const std::expected<int, std::string> ok = 42;
const std::string ok_json = rfl::json::write(ok);
// -> 42

const std::expected<int, std::string> err = std::unexpected("Something went wrong.");
const std::string err_json = rfl::json::write(err);
// -> {"error":"Something went wrong."}
```

Reading works in the reverse direction:

```cpp
const auto ok2 = rfl::json::read<std::expected<int, std::string>>(ok_json).value();
const auto err2 = rfl::json::read<std::expected<int, std::string>>(err_json).value();

// ok2.value() == 42
// err2.error() == "Something went wrong."
```

## Inside structs and containers

`std::expected` can be used as a field type and inside containers, just like any other supported type:

```cpp
struct Person {
  std::string name;
  std::expected<int, std::string> age;
};

const Person homer = {.name = "Homer", .age = 42};
const Person maggie = {.name = "Maggie", .age = std::unexpected("too young")};

const std::string homer_json = rfl::json::write(homer);
// -> {"name":"Homer","age":42}

const std::string maggie_json = rfl::json::write(maggie);
// -> {"name":"Maggie","age":{"error":"too young"}}
```

Vectors of `std::expected` work as well:

```cpp
struct Person {
  std::string first_name;
  std::vector<std::expected<int, std::string>> ages;
};

const auto homer =
    Person{.first_name = "Homer",
           .ages = {42, std::unexpected("unknown age")}};

const std::string json = rfl::json::write(homer);
// -> {"first_name":"Homer","ages":[42,{"error":"unknown age"}]}
```

## Structs as value types

If `T` is a struct, the success case is serialized as the struct itself:

```cpp
const std::expected<Person, std::string> value =
    Person{.first_name = "Bart", .ages = {10}};

const std::string json_string = rfl::json::write(value);
// -> {"first_name":"Bart","ages":[10]}
```

The error case is still serialized as an object with an `error` field, so the two alternatives remain unambiguous.

## JSON schema

Schemata are generated for `std::expected` as well. The schema of `std::expected<int, std::string>` looks like this:

```json
{"$schema":"https://json-schema.org/draft/2020-12/schema","anyOf":[{"type":"integer"},{"type":"object","properties":{"error":{"type":"string"}},"required":["error"]}],"$defs":{}}
```

## Limitations

- `std::expected<void, E>` is not supported.
- Formats that do not support variants (CSV and Parquet) do not support `std::expected` either, since it is serialized as a variant under the hood.

## Relation to `rfl::Result`

reflect-cpp's own result type, [`rfl::Result`](result.md), is what `rfl::json::read` and `rfl::json::write` return and operate on. Supporting `std::expected` is a separate concern: it means that you can use `std::expected<T, E>` as a *data type* in your structs, which is what this section is about.

Note that there is a CMake option `REFLECTCPP_USE_STD_EXPECTED` that makes `rfl::Result<T>` an alias for `std::expected<T, rfl::Error>`. This is a separate feature from the one described in this section, but the two can be combined.
