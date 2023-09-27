# reflect-cpp

![image](banner1.png)

**reflect-cpp** is a C++-20 library for fast serialization and deserialization using compile-time reflection, similar to [serde](https://github.com/serde-rs) in Rust, [pydantic](https://github.com/pydantic/pydantic) in Python, [encoding](https://github.com/golang/go/tree/master/src/encoding) in Go or [aeson](https://github.com/haskell/aeson/tree/master) in Haskell.

As the aformentioned libraries are among the most widely used in the respective languages, reflect-cpp fills an important gap in C++ development. It reduces boilerplate code and increases code safety.

Design principles for reflect-cpp include:

- Close integration with containers from the C++ standard library
- Close adherence to C++ idioms
- Out-of-the-box support for JSON, no dependencies
- Simple installation: If no JSON support is required, reflect-cpp is header-only. For JSON support, only a single source file needs to be compiled.
- Simple extendability to other serialization formats
- Simple extendability to custom classes
- No macros

```cpp
#include <iostream>
#include <rfl>
#include <rfl/json>

// "firstName", "lastName" and "children" are the field names
// as they will appear in the JSON. The C++ standard is
// snake case, the JSON standard is camel case, so the names
// will not always be identical.
struct Person {
    rfl::Field<std::string, "firstName"> first_name;
    rfl::Field<std::string, "lastName"> last_name;
    rfl::Field<std::vector<Person>, "children"> children;
};

const auto bart = Person{
    .first_name = "Bart", 
    .last_name = "Simpson", 
    .children = std::vector<Person>()
};

const auto lisa = Person{
    .first_name = "Lisa", 
    .last_name = "Simpson", 
    .children = rfl::default_value // same as std::vector<Person>() 
};

// Returns a deep copy of the original object,
// replacing first_name.
const auto maggie = rfl::replace(
    lisa, rfl::make_field<"firstName">("Maggie"));

const auto homer = Person{
    .first_name = "Homer", 
    .last_name = "Simpson", 
    .children = std::vector<Person>({bart, lisa, maggie})
}; 

// We can now transform this into a JSON string.
const std::string json_string = rfl::json::write(homer);

// And we can parse the string to recreate the struct.
auto homer2 = rfl::json::read<Person>(json_string).value();

// Fields can be accessed like this:
std::cout << "Hello, my name is " << homer.first_name() << " " << homer.last_name() << "." << std::endl;

// Since homer2 is mutable, we can also change the values like this:
homer2.first_name = "Marge";
```

## Support for containers

### C++ standard library

reflect-cpp supports the following containers from the C++ standard library:

- `std::array` (TODO)
- `std::deque` (TODO)
- `std::forward_list` (TODO)
- `std::map` (TODO)
- `std::list` (TODO)
- `std::optional`
- `std::pair`
- `std::set`
- `std::shared_ptr`
- `std::string`
- `std::tuple`
- `std::unique_ptr`
- `std::unordered_map` (TODO)
- `std::unordered_set` (TODO)
- `std::variant`
- `std::vector`

### Additional containers

In addition, it includes the following custom containers:

- `rfl::Box`: Similar to `std::unique_ptr`, but guaranteed to never be null.
- `rfl::Enum`: Similar to `std::variant`, but with explicit tags that make parsing more efficient.
- `rfl::Literal`: An explicitly enumerated string.
- `rfl::NamedTuple`: Similar to `std::tuple`, but with named fields that can be retrieved via their name at compile time.
- `rfl::Ref`: Similar to `std::shared_ptr`, but guaranteed to never be null. 
- `rfl::TaggedUnion`: Similar to `std::variant`, but with explicit tags that make parsing more efficient, results in a different serialization format than `rfl::Enum`.

### Custom classes

Finally, it is very easy to extend full support to your own classes, refer to the [documentation](TODO) for details.

## Serialization formats

reflect-cpp currently supports the following serialization formats:

- **JSON**: Out-of-the-box support, no additional dependencies required.
- **XML**: Requires [libxml2](https://github.com/GNOME/libxml2) (TODO).
- **flexbuffers**: Requires [flatbuffers](https://github.com/google/flatbuffers).

reflect-cpp is deliberately designed in a very modular format, using [concepts](https://en.cppreference.com/w/cpp/language/constraints), to make it as easy as possible to support additional serialization formats. Refer to the [documentation](TODO) for details. PRs related to serialization formats are welcome.

## Installation

If you **do not** need JSON support, then reflect-cpp is header-only. Simply copy the contents of the folder `include` into your source repository or add it to your include path.

If you **do** need JSON support, then you should also add `src/yyjson.c` to your source files for compilation. Note that `yyjson.c` is written in ANSI C.

If you need support for other serialization formats like XML or flexbuffers, you should also include and link the respective libraries, as listed in the previous section.

## Authors

reflect-cpp has been developed by [scaleML](https://www.scaleml.de), a company specializing in software engineering and machine learning for enterprise applications. It is extensively used for [getML](https://getml.com), a software for automated feature engineering using relational learning.

## License

reflect-cpp is released under the MIT License. Refer to the LICENSE file for details.

reflect-cpp includes [YYJSON](https://github.com/ibireme/yyjson), the fastest JSON library currently in existence. YYJSON is written by YaoYuan (<ibireme@gmail.com>) and also released under the MIT License.




