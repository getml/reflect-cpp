# reflect-cpp

![image](banner1.png)

**reflect-cpp** is a C++-20 library for compile-time reflection, similar to [serde](https://github.com/serde-rs) in Rust, [pydantic](https://github.com/pydantic/pydantic) in Python or [encoding](https://github.com/golang/go/tree/master/src/encoding) in Go. As these libraries are among the most widely used libraries in the respective languages, reflect-cpp fills an important gap in C++ development.

Design principles for reflect-cpp include:

- Close integration with containers in the C++ library
- Close integration with C++ idioms
- Out-of-the-box support for JSON, no dependencies
- Simple installation: If no JSON support is required, reflect-cpp is header-only. For JSON support, only a single source file needs to be compiled.
- Simple extendability to other formats.
- No macros

```cpp
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

// We only need to replace the first name to get from Lisa 
// to Maggie.
const auto maggie = rfl::replace(
    lisa, rfl::make_field<"firstName">("Maggie"));

const auto homer = Person{
    .first_name = "Homer", 
    .last_name = "Simpson", 
    .children = std::vector<Person>({bart, lisa, maggie})
}; 

// We can now transform this into a JSON string.
const std::string json_string = rfl::json::write(homer);

// And we can retrieve the result.
const auto homer2 = rfl::json::read<Person>(json_string).value();
```

## Installation

### Using
