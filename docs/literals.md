# Literals

In many cases, when you have strings, you know in advance that these strings can only
assume a limited number of values. For reasons of efficiency and safety, it is a good
idea to use Literals instead of strings.

You can declare a Literal as follows:

```cpp
using MyLiteral = rfl::Literal<"option1", "option2", ...>;
```

You can construct literals as follows:

```cpp
const auto my_literal = MyLiteral::make<"option1">();
```

Literals that contain only one option also have a default constructor.

Literals can be used inside switches:

```cpp
switch(my_literal.value()) {
  case MyLiteral::value_of<"option1">():
    ...

  case MyLiteral::value_of<"option2">():
    ...

  ...
}
```

Literals are stored as `uint8_t` or `uint16_t` under-the-hood. But you can extract their name as a string using `.name()` 
or the underlying value using `.value()`.

```cpp
using MyLiteral = rfl::Literal<"option1", "option2", ...>;

const auto my_literal = MyLiteral::make<"option1">();

// Prints "option1"
std::cout << my_literal.name() << std::endl;

// Prints "0"
std::cout << my_literal.value() << std::endl;
```

## Example

```cpp
// There are only five Simpsons.
using FirstName = rfl::Literal<"Homer", "Marge", "Bart", "Lisa", "Maggie">;

// All Simpsons have the last name.
using LastName = rfl::Literal<"Simpson">;

struct Person {
    rfl::Rename<"firstName", FirstName> first_name;
    rfl::Rename<"lastName", LastName> last_name;
    std::vector<Person> children;
};

// Leads to a runtime error, if the field "lastName" is not "Simpson"
// and the field "firstName" is not "Homer", "Marge", "Bart", "Lisa" or "Maggie".
const auto simpson_family_member = rfl::json::read<Person>(some_json_string).value();
```
