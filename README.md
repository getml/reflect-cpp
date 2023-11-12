# ![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) reflect-cpp

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://GitHub.com/Naereen/StrapDown.js/graphs/commit-activity)
[![Generic badge](https://img.shields.io/badge/C++-20-blue.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/gcc-11+-blue.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/clang-14+-blue.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/MSVC-TODO-red.svg)](https://shields.io/)

![image](banner1.png)

**reflect-cpp** is a C++-20 library for **fast serialization, deserialization and validation** using compile-time reflection, similar to [pydantic](https://github.com/pydantic/pydantic) in Python, [serde](https://github.com/serde-rs) in Rust, [encoding](https://github.com/golang/go/tree/master/src/encoding) in Go or [aeson](https://github.com/haskell/aeson/tree/master) in Haskell.

As the aformentioned libraries are among the most widely used in the respective languages, reflect-cpp fills an important gap in C++ development. It reduces boilerplate code and increases code safety.

Design principles for reflect-cpp include:

- Close integration with containers from the C++ standard library
- Close adherence to C++ idioms
- Out-of-the-box support for JSON
- Simple installation: If no JSON support is required, reflect-cpp is header-only. For JSON support, only a single source file needs to be compiled.
- Simple extendability to other serialization formats
- Simple extendability to custom classes
- No macros

## Why do we need this?

Suppose your C++ program has complex data structures it needs to save and load. Or maybe it needs to interact with some kind of external API. If you do this the traditional way, you will have a lot of boilerplate code. This is annoying and error-prone.

reflect-cpp is not just a reflection library, it is for **serialization, deserialization and validation** through reflection.

That means that you can encode your assumptions about the input data in the type system and have them validated upfront. This is why the library also includes algebraic data types like tagged unions and numerous validation routines. Having your assumptions encoded in the type system is the most reliable way of ensuring they are fullfilled. It also makes it a lot easier for anyone reading your code. If your assumptions are violated, the user of your software gets a very clear error message.

This increases user experience and developer experience, it makes your code safer (fewer bugs) and more secure (less prone to malicious attacks).

For a more in-depth theoretical discussions of these topics, the following books are warmly recommended:

- *Category Theory for Programmers* by Bartosz Milewski (https://github.com/hmemcpy/milewski-ctfp-pdf/releases) 
- *Domain Modeling Made Functional* by Scott Wlaschin

## Basic Example

```cpp
#include <iostream>
#include <rfl/json.hpp>
#include <rfl.hpp>

// Age must be a plausible number, between 0 and 130. This will
// be validated automatically.
using Age = rfl::Validator<int,
                           rfl::AllOf<rfl::Minimum<0>, rfl::Maximum<130>>>;

// "firstName", "lastName" and "children" are the field names
// as they will appear in the JSON. The C++ standard is
// snake case, the JSON standard is camel case, so the names
// will not always be identical.
struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">> birthday;
    rfl::Field<"age", Age> age;
    rfl::Field<"email", rfl::Email> email;
    rfl::Field<"children", std::vector<Person>> children;
};

const auto bart = Person{.first_name = "Bart",
                         .last_name = "Simpson",
                         .birthday = "1987-04-19",
                         .age = 10,
                         .email = "bart@simpson.com",
                         .children = std::vector<Person>()};

const auto lisa = Person{
      .first_name = "Lisa",
      .last_name = "Simpson",
      .birthday = "1987-04-19",
      .age = 8,
      .email = "lisa@simpson.com",
      .children = rfl::default_value  // same as std::vector<Person>()
};

// Returns a deep copy of the original object,
// replacing first_name, email and age.
const auto maggie =
      rfl::replace(lisa, rfl::make_field<"firstName">(std::string("Maggie")),
                   rfl::make_field<"email">(std::string("maggie@simpson.com")),
                   rfl::make_field<"age">(0));

const auto homer =
      Person{.first_name = "Homer",
             .last_name = "Simpson",
             .birthday = "1987-04-19",
             .age = 45,
             .email = "homer@simpson.com",
             .children = std::vector<Person>({bart, lisa, maggie})};

// We can now transform this into a JSON string.
const std::string json_string = rfl::json::write(homer);

std::cout << json_string << std::endl;
```

This results in the following JSON string:

```json
{"firstName":"Homer","lastName":"Simpson","birthday":"1987-04-19","age":45,"email":"homer@simpson.com","children":[{"firstName":"Bart","lastName":"Simpson","birthday":"1987-04-19","age":10,"email":"bart@simpson.com","children":[]},{"firstName":"Lisa","lastName":"Simpson","birthday":"1987-04-19","age":8,"email":"lisa@simpson.com","children":[]},{"firstName":"Maggie","lastName":"Simpson","birthday":"1987-04-19","age":0,"email":"maggie@simpson.com","children":[]}]}
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

## Error messages

reflect-cpp returns clear and comprehensive error messages:

```cpp
const std::string faulty_json_string =
    R"({"firstName":"Homer","lastName":12345,"birthday":"04/19/1987","age":145,"email":"homer(at)simpson.com"})";
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

## Anonymous fields

`rfl::Field` is only necessary if you want to serialize field names. If you don't (possibly because you are using a binary format), you do not need to annotate your fields:

```cpp
using Age = rfl::Validator<unsigned int,
                           rfl::AllOf<rfl::Minimum<0>, rfl::Maximum<130>>>;

struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  rfl::Email email;
  std::vector<Person> children;
};

const auto bart = Person{.first_name = "Bart",
                         .last_name = "Simpson",
                         .birthday = "1987-04-19",
                         .age = 10,
                         .email = "bart@simpson.com"};

const auto lisa = Person{.first_name = "Lisa",
                         .last_name = "Simpson",
                         .birthday = "1987-04-19",
                         .age = 8,
                         .email = "lisa@simpson.com"};

const auto maggie = Person{.first_name = "Maggie",
                           .last_name = "Simpson",
                           .birthday = "1987-04-19",
                           .age = 0,
                           .email = "maggie@simpson.com"};

const auto homer =
    Person{.first_name = "Homer",
           .last_name = "Simpson",
           .birthday = "1987-04-19",
           .age = 45,
           .email = "homer@simpson.com",
           .children = std::vector<Person>({bart, lisa, maggie})};
```

This results in the following JSON string:

```json
["Homer","Simpson","1987-04-19",45,"homer@simpson.com",[["Bart","Simpson","1987-04-19",10,"bart@simpson.com",[]],["Lisa","Simpson","1987-04-19",8,"lisa@simpson.com",[]],["Maggie","Simpson","1987-04-19",0,"maggie@simpson.com",[]]]]
```

## Algebraic data types

reflect-cpp supports Pydantic-style tagged unions, which allow you to form algebraic data types:

```cpp
  // All alternatives must contain a field named "shape". The type
  // of the field must be an rfl::Literal.
struct Circle {
    rfl::Field<"shape", rfl::Literal<"Circle">> shape = rfl::default_value;
    rfl::Field<"radius", double> radius;
};

struct Rectangle {
    rfl::Field<"shape", rfl::Literal<"Rectangle">> shape =
        rfl::default_value;
    rfl::Field<"height", double> height;
    rfl::Field<"width", double> width;
};

struct Square {
    rfl::Field<"shape", rfl::Literal<"Square">> shape = rfl::default_value;
    rfl::Field<"width", double> width;
};

// Now you tell rfl::TaggedUnion that you want it to look for the field "shape".
using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

const Shapes r = Rectangle{.height = 10, .width = 5};

const auto json_string = rfl::json::write(r);
```

This results in the following JSON string:

```json
{"shape":"Rectangle","height":10.0,"width":5.0}
```

Other forms of tagging are supported as well. Refer to the [documentation](https://github.com/getml/reflect-cpp/tree/main/docs) for details.

## Support for containers

### C++ standard library

reflect-cpp supports the following containers from the C++ standard library:

- `std::array` 
- `std::deque` 
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

### Additional containers

In addition, it supports the following custom containers:

- `rfl::Box`: Similar to `std::unique_ptr`, but (almost) guaranteed to never be null.
- `rfl::Literal`: An explicitly enumerated string.
- `rfl::NamedTuple`: Similar to `std::tuple`, but with named fields that can be retrieved via their name at compile time.
- `rfl::Ref`: Similar to `std::shared_ptr`, but (almost) guaranteed to never be null.
- `rfl::Result`: Allows for exception-free programming.
- `rfl::TaggedUnion`: Similar to `std::variant`, but with explicit tags that make parsing more efficient.
- `rfl::Validator`: Allows for automatic input validation.

### Custom classes

Finally, it is very easy to extend full support to your own classes, refer to the [documentation](https://github.com/getml/reflect-cpp/tree/main/docs) for details.

## Serialization formats

reflect-cpp currently supports the following serialization formats:

- **JSON**: Out-of-the-box support, no additional dependencies required.
- **XML**: Requires [libxml2](https://github.com/GNOME/libxml2) (TODO).
- **flexbuffers**: Requires [flatbuffers](https://github.com/google/flatbuffers) (TODO).

reflect-cpp is deliberately designed in a very modular format, using [concepts](https://en.cppreference.com/w/cpp/language/constraints), to make it as easy as possible to support additional serialization formats. Refer to the [documentation](https://github.com/getml/reflect-cpp/tree/main/docs) for details (TODO). PRs related to serialization formats are welcome.

## Documentation

Click [here](https://github.com/getml/reflect-cpp/tree/main/docs).

## Current status

At the moment, this is work-in-progress. Here are some things that are still missing：

- Benchmarks and optimization
- Documentation is unfinished (missing rfl::Result, rfl::NamedTuple as well as extension to other serialization formats)
- Support for MSVC

## Installation

The following compilers are supported:
- GCC 11.4 or higher
- Clang 14.0 or higher
- MSVC (TODO)

### Option 1: Header-only

If you **do not** need JSON support or you want to link YYJSON yourself, then reflect-cpp is header-only. Simply copy the contents of the folder `include` into your source repository or add it to your include path.

### Option 2: Include source files into your own build

Simply copy the contents of the folder `include` into your source repository or add it to your include path and also add `src/yyjson.c` to your source files for compilation.

If you need support for other serialization formats like XML or flexbuffers, you should also include and link the respective libraries, as listed in the previous section.

### Option 3: Compilation using cmake 

```
mkdir build
cd build
cmake ..
make
```

## Compiling the tests

To compile the tests, do the following:

```
cd tests
mkdir build
cd build
cmake ..
make
```

## Authors

reflect-cpp has been developed by [scaleML](https://www.scaleml.de), a company specializing in software engineering and machine learning for enterprise applications. It is extensively used for [getML](https://getml.com), a software for automated feature engineering using relational learning.

## License

reflect-cpp is released under the MIT License. Refer to the LICENSE file for details.

reflect-cpp includes [YYJSON](https://github.com/ibireme/yyjson), the fastest JSON library currently in existence. YYJSON is written by YaoYuan and also released under the MIT License.




