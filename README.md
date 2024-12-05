# ![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) reflect-cpp

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://github.com/getml/reflect-cpp/graphs/commit-activity)
[![Generic badge](https://img.shields.io/badge/C++-20-blue.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/gcc-11+-blue.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/clang-14+-blue.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/MSVC-17+-blue.svg)](https://shields.io/)
[![Conan Center](https://img.shields.io/conan/v/reflect-cpp)](https://conan.io/center/recipes/reflect-cpp)


**📖 Documentation**: https://rfl.getml.com — **👨‍💻 Source Code**: https://github.com/getml/reflect-cpp

><br> **📣 From the Author (12.11.2024):** Hi everyone, Patrick ([liuzicheng1987](https://github.com/liuzicheng1987)) here. With reflect-cpp reaching the 1k-star milestone, we’re excited to roll out an overhauled documentation site at [https://rfl.getml.com](https://rfl.getml.com/), giving it a permanent place in our company. Initially developed as an internal tool for our machine learning library, [getML](https://getml.com), reflect-cpp has grown into something much larger.
<br> A big thank you to all contributors for helping us reach this point! Your feedback, ideas, and dedication have been invaluable.
<br> As we look to the project’s future, I would like to hear your thoughts on potential new directions, discuss ideas to expand our user base, or learn more about what you’re building with it. For the next month, I am opening a few slots in my calendar for anyone who wants to [connect (link)](https://calendar.app.google/AaeziooCNierbwAZ8).
<br> *— Best, Patrick*
<br>&nbsp;


![image](banner1.png)

**reflect-cpp** is a C++-20 library for **fast serialization, deserialization and validation** using reflection, similar to [pydantic](https://github.com/pydantic/pydantic) in Python, [serde](https://github.com/serde-rs) in Rust, [encoding](https://github.com/golang/go/tree/master/src/encoding) in Go or [aeson](https://github.com/haskell/aeson/tree/master) in Haskell.

As the aforementioned libraries are among the most widely used in the respective languages, reflect-cpp fills an important gap in C++ development. It reduces boilerplate code and increases code safety.


### Design principles for reflect-cpp include:

- Close integration with [containers](https://github.com/getml/reflect-cpp?tab=readme-ov-file#support-for-containers) from the C++ standard library
- Close adherence to C++ idioms
- Out-of-the-box support for [JSON](https://rfl.getml.com/supported_formats/json)
- Simple [installation](https://rfl.getml.com/install)
- Simple extendability to [other serialization formats](https://rfl.getml.com/supported_formats/supporting_your_own_format)
- Simple extendability to [custom classes](https://rfl.getml.com/concepts/custom_classes)
- Being one of the fastest serialization libraries in existence, as demonstrated by our [benchmarks](https://rfl.getml.com/benchmarks)

<br>


## Table of Contents

### On this page
  - [Serialization formats](#serialization-formats)
  - [Feature Overview](#feature-overview)
    - [Simple Example](#simple-example)
    - [More Comprehensive Example](#more-comprehensive-example)
    - [Error messages](#error-messages)
    - [JSON schema](#json-schema)
    - [Enums](#enums)
    - [Algebraic data types](#algebraic-data-types)
    - [Extra fields](#extra-fields)
    - [Reflective programming](#reflective-programming)
    - [Standard Library Integration](#support-for-containers) 
  - [The team behind reflect-cpp](#the-team-behind-reflect-cpp)
  - [License](#license)

### More in our [documentation](https://rfl.getml.com):
  - [Installation ↗](https://rfl.getml.com/install/#option-2-compilation-using-cmake)
  - [Benchmarks ↗](https://rfl.getml.com/benchmarks)
  - [How to contribute ↗](https://rfl.getml.com/contributing) 
  - [Compiling and running the tests ↗](https://rfl.getml.com/contributing/#compiling-and-running-the-tests)


## Serialization formats

reflect-cpp provides a unified reflection-based interface across different serialization formats. It is deliberately designed in a very modular way, using [concepts](https://en.cppreference.com/w/cpp/language/constraints), to make it as easy as possible to interface various C or C++ libraries related to serialization. Refer to the [documentation](https://rfl.getml.com/supported_formats/bson/) for details.

The following table lists the serialization formats currently supported by reflect-cpp and the underlying libraries used:

| Format       | Library                                              | Version      | License    | Remarks                                              |
|--------------|------------------------------------------------------|--------------|------------| -----------------------------------------------------|
| JSON         | [yyjson](https://github.com/ibireme/yyjson)          | >= 0.8.0     | MIT        | out-of-the-box support, included in this repository  |
| BSON         | [libbson](https://github.com/mongodb/mongo-c-driver) | >= 1.25.1    | Apache 2.0 | JSON-like binary format                              |
| CBOR         | [tinycbor](https://github.com/intel/tinycbor)        | >= 0.6.0     | MIT        | JSON-like binary format                              |
| flexbuffers  | [flatbuffers](https://github.com/google/flatbuffers) | >= 23.5.26   | Apache 2.0 | Schema-less version of flatbuffers, binary format    |
| msgpack      | [msgpack-c](https://github.com/msgpack/msgpack-c)    | >= 6.0.0     | BSL 1.0    | JSON-like binary format                              |
| TOML         | [toml++](https://github.com/marzer/tomlplusplus)     | >= 3.4.0     | MIT        | Textual format with an emphasis on readability       |
| UBJSON       | [jsoncons](https://github.com/danielaparker/jsoncons)| >= 0.176.0   | BSL 1.0    | JSON-like binary format                              |
| XML          | [pugixml](https://github.com/zeux/pugixml)           | >= 1.14      | MIT        | Textual format used in many legacy projects          |
| YAML         | [yaml-cpp](https://github.com/jbeder/yaml-cpp)       | >= 0.8.0     | MIT        | Textual format with an emphasis on readability       |

Support for more serialization formats is in development. Refer to the [issues](https://github.com/getml/reflect-cpp/issues) for details.

Please also refer to the *conanfile.py* or *vcpkg.json* in this repository.


## Feature Overview

### Simple Example

```cpp
#include <rfl/json.hpp>
#include <rfl.hpp>

struct Person {
  std::string first_name;
  std::string last_name;
  int age;
};

const auto homer =
    Person{.first_name = "Homer",
           .last_name = "Simpson",
           .age = 45};

// We can now write into and read from a JSON string.
const std::string json_string = rfl::json::write(homer);
auto homer2 = rfl::json::read<Person>(json_string).value();
```

The resulting JSON string looks like this:

```json
{"first_name":"Homer","last_name":"Simpson","age":45}
```

You can transform the field names from `snake_case` to `camelCase` like this:

```cpp
const std::string json_string = 
  rfl::json::write<rfl::SnakeCaseToCamelCase>(homer);
auto homer2 = 
  rfl::json::read<Person, rfl::SnakeCaseToCamelCase>(json_string).value();
```

The resulting JSON string looks like this:

```json
{"firstName":"Homer","lastName":"Simpson","age":45}
```

Or you can use another format, such as YAML.

```cpp
#include <rfl/yaml.hpp>

// ... (same as above)

const std::string yaml_string = rfl::yaml::write(homer);
auto homer2 = rfl::yaml::read<Person>(yaml_string).value();
```

The resulting YAML string looks like this:

```yaml
first_name: Homer
last_name: Simpson
age: 45
```

This will work for just about any example in the entire documentation 
and any supported format, except where explicitly noted otherwise:

```cpp
rfl::bson::write(homer);
rfl::cbor::write(homer);
rfl::flexbuf::write(homer);
rfl::msgpack::write(homer);
rfl::toml::write(homer);
rfl::ubjson::write(homer);
rfl::xml::write(homer);

rfl::bson::read<Person>(bson_bytes);
rfl::cbor::read<Person>(cbor_bytes);
rfl::flexbuf::read<Person>(flexbuf_bytes);
rfl::msgpack::read<Person>(msgpack_bytes);
rfl::toml::read<Person>(toml_string);
rfl::ubjson::read<Person>(ubjson_bytes);
rfl::xml::read<Person>(xml_string);
```

### More Comprehensive Example

```cpp
#include <iostream>
#include <rfl/json.hpp>
#include <rfl.hpp>

// Age must be a plausible number, between 0 and 130. This will
// be validated automatically.
using Age = rfl::Validator<int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::string town = "Springfield";
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  rfl::Email email;
  std::vector<Person> children;
};

const auto bart = Person{.first_name = "Bart",
                         .birthday = "1987-04-19",
                         .age = 10,
                         .email = "bart@simpson.com"};

const auto lisa = Person{.first_name = "Lisa",
                         .birthday = "1987-04-19",
                         .age = 8,
                         .email = "lisa@simpson.com"};

const auto maggie = Person{.first_name = "Maggie",
                           .birthday = "1987-04-19",
                           .age = 0,
                           .email = "maggie@simpson.com"};

const auto homer =
    Person{.first_name = "Homer",
           .birthday = "1987-04-19",
           .age = 45,
           .email = "homer@simpson.com",
           .children = std::vector<Person>({bart, lisa, maggie})};

// We can now transform this into a JSON string.
const std::string json_string = rfl::json::write(homer);
std::cout << json_string << std::endl;

// We can also directly write into std::cout (or any other std::ostream).
rfl::json::write(homer, std::cout) << std::endl;
```

This results in the following JSON string:

```json
{"firstName":"Homer","lastName":"Simpson","town":"Springfield","birthday":"1987-04-19","age":45,"email":"homer@simpson.com","children":[{"firstName":"Bart","lastName":"Simpson","town":"Springfield","birthday":"1987-04-19","age":10,"email":"bart@simpson.com","children":[]},{"firstName":"Lisa","lastName":"Simpson","town":"Springfield","birthday":"1987-04-19","age":8,"email":"lisa@simpson.com","children":[]},{"firstName":"Maggie","lastName":"Simpson","town":"Springfield","birthday":"1987-04-19","age":0,"email":"maggie@simpson.com","children":[]}]}
```

We can also create structs from the string:

```cpp
auto homer2 = rfl::json::read<Person>(json_string).value();

// Fields can be accessed like this:
std::cout << "Hello, my name is " << homer.first_name() << " "
          << homer.last_name() << "." << std::endl;

// Since homer2 is mutable, we can also change the values like this:
homer2.first_name = "Marge";

std::cout << "Hello, my name is " << homer2.first_name() << " "
          << homer2.last_name() << "." << std::endl;
```

### Error messages

reflect-cpp returns clear and comprehensive error messages:

```cpp
const std::string faulty_json_string =
    R"({"firstName":"Homer","lastName":12345,"town":"Springfield","birthday":"04/19/1987","age":145,"email":"homer(at)simpson.com"})";
const auto result = rfl::json::read<Person>(faulty_json_string);
```

Yields the following error message:

```
Found 5 errors:
1) Failed to parse field 'lastName': Could not cast to string.
2) Failed to parse field 'birthday': String '04/19/1987' did not match format '%Y-%m-%d'.
3) Failed to parse field 'age': Value expected to be less than or equal to 130, but got 145.
4) Failed to parse field 'email': String 'homer(at)simpson.com' did not match format 'Email': '^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$'.
5) Field named 'children' not found.
```

### JSON schema

reflect-cpp also supports generating JSON schemata:

```cpp
struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Description<"Must be a proper email in the form xxx@xxx.xxx.",
                   rfl::Email>
      email;
  rfl::Description<
      "The person's children. Pass an empty array for no children.",
      std::vector<Person>>
      children;
  float salary;
};

const std::string json_schema = rfl::json::to_schema<Person>();
```

The resulting JSON schema looks like this:

```json
{"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/definitions/Person","definitions":{"Person":{"type":"object","properties":{"children":{"type":"array","description":"The person's children. Pass an empty array for no children.","items":{"$ref":"#/definitions/Person"}},"email":{"type":"string","description":"Must be a proper email in the form xxx@xxx.xxx.","pattern":"^[a-zA-Z0-9._%+\\-]+@[a-zA-Z0-9.\\-]+\\.[a-zA-Z]{2,}$"},"first_name":{"type":"string"},"last_name":{"type":"string"},"salary":{"type":"number"}},"required":["children","email","first_name","last_name","salary"]}}}
```

Note that this is currently supported for JSON only, since most other formats do not support schemata in the first place.

### Enums

reflect-cpp supports scoped enumerations:

```cpp
enum class Shape { circle, square, rectangle };

enum class Color { red = 256, green = 512, blue = 1024, yellow = 2048 };

struct Item {
  float pos_x;
  float pos_y;
  Shape shape;
  Color color;
};

const auto item = Item{.pos_x = 2.0,  
                       .pos_y = 3.0,
                       .shape = Shape::square,
                       .color = Color::red | Color::blue};

rfl::json::write(item);
```

This results in the following JSON string:

```json
{"pos_x":2.0,"pos_y":3.0,"shape":"square","color":"red|blue"}
```

You can also directly convert between enumerator values and strings with `rfl::enum_to_string()` and `rfl::string_to_enum()`, or
obtain list of enumerator name and value pairs with `rfl::get_enumerators<EnumType>()` or `rfl::get_enumerator_array<EnumType>()`.

### Algebraic data types

reflect-cpp supports Pydantic-style tagged unions, which allow you to form algebraic data types:

```cpp
struct Circle {
    double radius;
};

struct Rectangle {
    double height;
    double width;
};

struct Square {
    double width;
};

using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

const Shapes r = Rectangle{.height = 10, .width = 5};

const auto json_string = rfl::json::write(r);
```

This results in the following JSON string:

```json
{"shape":"Rectangle","height":10.0,"width":5.0}
```

Other forms of tagging are supported as well. Refer to the [documentation](https://rfl.getml.com/docs-readme) for details.

### Extra fields

If you don't know all of your fields at compile time, no problem. Just use `rfl::ExtraFields`:

```cpp
struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  rfl::ExtraFields<rfl::Generic> extra_fields;
};

auto homer = Person{.first_name = "Homer"};

homer.extra_fields["age"] = 45;
homer.extra_fields["email"] = "homer@simpson.com";
homer.extra_fields["town"] = "Springfield";
```

This results in the following JSON string:

```json
{"firstName":"Homer","lastName":"Simpson","age":45,"email":"homer@simpson.com","town":"Springfield"}
```

### Reflective programming

Beyond serialization and deserialization, reflect-cpp also supports reflective programming in general.

For instance:

```cpp
struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::string town = "Springfield";
  unsigned int age;
  std::vector<Person> children;
};

for (const auto& f : rfl::fields<Person>()) {
  std::cout << "name: " << f.name() << ", type: " << f.type() << std::endl;
}
```

You can also create a view and then access these fields using `std::get` or `rfl::get`, or iterate over the fields at compile-time:

```cpp
auto lisa = Person{.first_name = "Lisa", .last_name = "Simpson", .age = 8};

const auto view = rfl::to_view(lisa);

// view.values() is a std::tuple containing
// pointers to the original fields.
// This will modify the struct `lisa`:
*std::get<0>(view.values()) = "Maggie";

// All of this is supported as well:
*view.get<1>() = "Simpson";
*view.get<"age">() = 0;
*rfl::get<0>(view) = "Maggie";
*rfl::get<"first_name">(view) = "Maggie";

view.apply([](const auto& f) {
  // f is an rfl::Field pointing to the original field.
  std::cout << f.name() << ": " << rfl::json::write(*f.value()) << std::endl;
});
```

It also possible to replace fields:

```cpp
struct Person {
  std::string first_name;
  std::string last_name;
  std::vector<Person> children;
};

const auto lisa = Person{.first_name = "Lisa", .last_name = "Simpson"};

// Returns a deep copy of "lisa" with the first_name replaced.
const auto maggie = rfl::replace(
    lisa, rfl::make_field<"first_name">(std::string("Maggie")));
```

Or you can create structs from other structs:

```cpp
struct A {
  std::string f1;
  std::string f2;
};

struct B {
  std::string f3;
  std::string f4;
};

struct C {
  std::string f1;
  std::string f2;
  std::string f4;
};

const auto a = A{.f1 = "Hello", .f2 = "World"};

const auto b = B{.f3 = "Hello", .f4 = "World"};

// f1 and f2 are taken from a, f4 is taken from b, f3 is ignored.
const auto c = rfl::as<C>(a, b);
```

You can also replace fields in structs using fields from other structs:

```cpp
const auto a = A{.f1 = "Hello", .f2 = "World"};

const auto c = C{.f1 = "C++", .f2 = "is", .f4 = "great"};

// The fields f1 and f2 are replaced with the fields f1 and f2 in a.
const auto c2 = rfl::replace(c, a);
```


### Support for containers

#### C++ standard library

reflect-cpp supports the following containers from the C++ standard library:

- `std::array`
- `std::deque`
- `std::filesystem::path`
- `std::forward_list`
- `std::map`
- `std::multimap`
- `std::multiset`
- `std::list`
- `std::optional`
- `std::pair`
- `std::set`
- `std::shared_ptr`
- `std::string`
- `std::tuple`
- `std::unique_ptr`
- `std::unordered_map`
- `std::unordered_multimap`
- `std::unordered_multiset`
- `std::unordered_set`
- `std::variant`
- `std::vector`
- `std::wstring`

#### Additional containers

In addition, it supports the following custom containers:

- `rfl::Binary`: Used to express numbers in binary format.
- `rfl::Box`: Similar to `std::unique_ptr`, but (almost) guaranteed to never be null.
- `rfl::Bytestring`: An alias for `std::basic_string<std::byte>`. Supported by BSON, CBOR, flexbuffers, msgpack and UBJSON. 
- `rfl::Generic`: A catch-all type that can represent (almost) anything.
- `rfl::Hex`: Used to express numbers in hex format.
- `rfl::Literal`: An explicitly enumerated string.
- `rfl::NamedTuple`: Similar to `std::tuple`, but with named fields that can be retrieved via their name at compile time.
- `rfl::Object`: A map-like type representing a object with field names that are unknown at compile time.
- `rfl::Oct`: Used to express numbers in octal format.
- `rfl::Ref`: Similar to `std::shared_ptr`, but (almost) guaranteed to never be null.
- `rfl::Result`: Allows for exception-free programming.
- `rfl::TaggedUnion`: Similar to `std::variant`, but with explicit tags that make parsing more efficient.
- `rfl::Tuple`: An alternative to `std::tuple` that compiles considerably faster.
- `rfl::Validator`: Allows for automatic input validation.
- `rfl::Variant`: An alternative to `std::variant` that compiles considerably faster.

#### Custom classes

Finally, it is very easy to extend full support to your own classes, refer to the [documentation](https://rfl.getml.com/docs-readme) for details.


## Installation

The following compilers are supported:
- GCC 11.4 or higher
- Clang 14.0 or higher
- MSVC 17.8 (19.38) or higher

### Using vcpkg

https://vcpkg.io/en/package/reflectcpp

### Using Conan

https://conan.io/center/recipes/reflect-cpp

### Compilation using cmake

This will compile reflect-cpp with JSON support only. You can then include reflect-cpp in your project and link to the binary.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 4  # gcc, clang
cmake --build build --config Release -j 4  # MSVC
```

If you need support for any other supported [serialization formats](#serialization-formats), refer to the [documentation](https://rfl.getml.com/docs-readme) for installation instructions.

You can also [include the source files](https://rfl.getml.com/install/#option-1-include-source-files-into-your-own-build) into your build or compile it using [cmake and vcpkg.](https://rfl.getml.com/install/#option-3-compilation-using-cmake-and-vcpkg) For detailed installation instructions, please refer to the [install guide](https://rfl.getml.com/install).

## The team behind reflect-cpp

reflect-cpp has been developed by [getML (Code17 GmbH)](https://getml.com), a company specializing in software engineering and machine learning for enterprise applications. reflect-cpp is currently maintained by Patrick Urbanke and Manuel Bellersen, with major contributions coming from the community.

### Related projects

reflect-cpp was originally developed for [getml-community](https://github.com/getml/getml-community), the fastest open-source tool for feature engineering on relational data and time series. If you are interested in Data Science and/or Machine Learning, please check it out.

### Professional C++ Support

For comprehensive C++ support beyond the scope of GitHub discussions, we’re here to help! Reach out at [support@getml.com](mailto:support%40getml.com?subject=C++%20support%20request) to discuss any technical challenges or project requirements. We’re excited to support your work as independent software consultants.


## License

reflect-cpp is released under the MIT License. Refer to the LICENSE file for details.

reflect-cpp includes [YYJSON](https://github.com/ibireme/yyjson), the fastest JSON library currently in existence. YYJSON is written by YaoYuan and also released under the MIT License.

reflect-cpp includes [compile-time-regular-expressions](https://github.com/hanickadot/compile-time-regular-expressions). CTRE is written by Hana Dusíková and released under the Apache-2.0 License with LLVM exceptions.
