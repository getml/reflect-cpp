
## Boxes and References 

In previous sections, we have defined the `Person` class recursively:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    std::vector<Person> children;
};
```

This works, because `std::vector` contains a pointer under-the-hood. But what wouldn't work is something like this:

```cpp
// WILL NOT COMPILE
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    Person child;
};
```

This is because the compiler cannot figure out the intended size of the struct. But recursively defined structures
are important. For instance, if you deal with machine learning, you might be familiar with a decision tree.

A decision tree consists of a `Leaf` containing the prediction and a `Node` which splits the decision tree into
two subtrees.

A naive implementation might look like this:

```cpp
// WILL NOT COMPILE
struct DecisionTree {
    struct Leaf {
        using Tag = rfl::Literal<"Leaf">;
        double prediction;
    };

    struct Node {
        using Tag = rfl::Literal<"Node">;
        rfl::Rename<"criticalValue", double> critical_value;
        DecisionTree lesser;
        DecisionTree greater;
    };

    using LeafOrNode = rfl::TaggedUnion<"type", Leaf, Node>;

    rfl::Field<"leafOrNode", LeafOrNode> leaf_or_node;
};
```

Again, this will not compile, because the compiler cannot figure out the intended size of the struct.

A possible solution might be to use `std::unique_ptr`:

```cpp
// Will compile, but not an ideal design.
struct DecisionTree {
    struct Leaf {
        using Tag = rfl::Literal<"Leaf">;
        double prediction;
    };

    struct Node {
        using Tag = rfl::Literal<"Node">;
        rfl::Rename<"criticalValue", double> critical_value;
        std::unique_ptr<DecisionTree> lesser;
        std::unique_ptr<DecisionTree> greater;
    };

    using LeafOrNode = rfl::TaggedUnion<"type", Leaf, Node>;

    rfl::Field<"leafOrNode", LeafOrNode> leaf_or_node;
};
```

This will compile, but the design is less than ideal. We know for a fact that a `Node` must have
exactly two subtrees. But this is not reflected in the type system. In this encoding, the fields 
"lesser" and "greater" are marked optional and you will have to check at runtime that they are indeed set.

But this violates the principles of reflection. Reflection is all about validating as much of our assumptions
upfront as we possibly can. For a great theoretical discussion of this topic, check out 
[Parse, don't validate](https://lexi-lambda.github.io/blog/2019/11/05/parse-don-t-validate/)
by Alexis King.

So how would we encode our assumptions that the fields "lesser" and "greater" must exist in the type system and
still have code that compiles? By using `rfl::Box` instead of `std::unique_ptr`:

```cpp
struct DecisionTree {
    struct Leaf {
        using Tag = rfl::Literal<"Leaf">;
        double prediction;
    };

    struct Node {
        using Tag = rfl::Literal<"Node">;
        rfl::Rename<"criticalValue", double> critical_value;
        rfl::Box<DecisionTree> lesser;
        rfl::Box<DecisionTree> greater;
    };

    using LeafOrNode = rfl::TaggedUnion<"type", Leaf, Node>;

    rfl::Field<"leafOrNode", LeafOrNode> leaf_or_node;
};
```

`rfl::Box` is a thin wrapper around `std::unique_ptr`, but it is guaranteed to **never be null** (unless you do something egregious such as trying to access it after calling `std::move`). It is a `std::unique_ptr` without the `nullptr`.

If you want to learn more about the evils of null references, check out the 
[Null References: The Billion Dollar Mistake](https://www.infoq.com/presentations/Null-References-The-Billion-Dollar-Mistake-Tony-Hoare/)
by Tony Hoare, who invented the concept in the first place.

You **must** initialize `rfl::Box` the moment you create it and it cannot be dereferenced until it is destroyed.

`rfl::Box` can be initialized using `rfl::make_box<...>(...)`, just like `std::make_unique<...>(...)`:

```cpp
auto leaf1 = DecisionTree::Leaf{.value = 3.0};

auto leaf2 = DecisionTree::Leaf{.value = 5.0};

auto node =
    DecisionTree::Node{.critical_value = 10.0,
                       .lesser = rfl::make_box<DecisionTree>(leaf1),
                       .greater = rfl::make_box<DecisionTree>(leaf2)};

const DecisionTree tree{.leaf_or_node = std::move(node)};

const auto json_string = rfl::json::write(tree);
```

This will result in the following JSON string:

```json
{"leafOrNode":{"type":"Node","criticalValue":10.0,"lesser":{"leafOrNode":{"type":"Leaf","value":3.0}},"greater":{"leafOrNode":{"type":"Leaf","value":5.0}}}}
```

You can also initialize `rfl::Box<T>` from a `std::unique_ptr<T>`:

```cpp
auto ptr = std::make_unique<std::string>("Hello World!");
const rfl::Result<rfl::Box<std::string>> box = rfl::make_box<std::string>(std::move(ptr));
```

Note that `box` is wrapped in a `Result`. That is, because we cannot guarantee at compile time 
that `ptr` is not `nullptr`, therefore we need to account for that.

If you want to use reference-counted pointers, instead of unique pointers, you can use `rfl::Ref`. 
`rfl::Ref` is the same concept as `rfl::Box`, but using `std::shared_ptr` under-the-hood.

```cpp
struct DecisionTree {
    struct Leaf {
        using Tag = rfl::Literal<"Leaf">;
        double value;
    };

    struct Node {
        using Tag = rfl::Literal<"Node">;
        rfl::Rename<"criticalValue", double> critical_value;
        rfl::Ref<DecisionTree> lesser;
        rfl::Ref<DecisionTree> greater;
    };

    using LeafOrNode = rfl::TaggedUnion<"type", Leaf, Node>;

    rfl::Field<"leafOrNode", LeafOrNode> leaf_or_node;
};

const auto leaf1 = DecisionTree::Leaf{.value = 3.0};

const auto leaf2 = DecisionTree::Leaf{.value = 5.0};

auto node =
    DecisionTree::Node{.critical_value = 10.0,
                       .lesser = rfl::make_ref<DecisionTree>(leaf1),
                       .greater = rfl::make_ref<DecisionTree>(leaf2)};

const DecisionTree tree{.leaf_or_node = std::move(node)};

const auto json_string = rfl::json::write(tree);
```

The resulting JSON string is identical:

```json
{"leafOrNode":{"type":"Node","criticalValue":10.0,"lesser":{"leafOrNode":{"type":"Leaf","value":3.0}},"greater":{"leafOrNode":{"type":"Leaf","value":5.0}}}}
```

## Enums

reflect-cpp supports scoped enumerations. They can either come in the form of normal enumerations or flag enums.

### Normal enumerations

Example:

```cpp
enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

const auto circle = Circle{.radius = 2.0, .color = Color::green};

rfl::json::write(circle);
```

This results in the following JSON string:

```json
{"radius":2.0,"color":"green"}
```

However, some limitations apply:

1. They must be be scoped enumerations.

```cpp
/// OK - scoped enumeration
enum class Color1 { red, green, blue, yellow };

/// OK - scoped enumeration
enum struct Color2 { red, green, blue, yellow };

/// unsupported - unscoped enumerations
enum Color3 { red, green, blue, yellow };
```

2. You cannot have more than 128 values and if you explicitly assign values, they must be between 0 and 127.

```cpp
/// OK
enum class Color1 { red, green, blue, yellow };

/// OK
enum struct Color2 { red, green, blue = 50, yellow };

/// unsupported - red is a negative value
enum Color3 { red = -10, green, blue, yellow };

/// unsupported - red is greater than 127
enum Color3 { red = 200, green, blue, yellow };
```

### Flag enums

Sometimes the enumerations are not mutually exclusive - sticking with the metaphor of colors, it is perfectly
possible for things to have more than one color. C++ programmers sometimes like to model this using a flag enum.

Flag enums work as follows:

1. The bitwise OR operator must be defined on them. If an enum has the bitwise OR operator defined on it, it will be treated as a flag enum.
2. The most important values of the enum must be 1 or 2^N, N being a positive integer (in other words 1,2,4,8,16,32,...).

Example:

```cpp
// The important colors must be 1 or 2^N
enum class Color {
  red = 256,
  green = 512,
  blue = 1024,
  yellow = 2048,
  orange = (256 | 2048)  // red + yellow = orange
};

// The bitwise OR operator must be defined - this is how reflect-cpp knows that
// this is a flag enum.
inline Color operator|(Color c1, Color c2) {
  return static_cast<Color>(static_cast<int>(c1) | static_cast<int>(c2));
}
```

In this particular example, the important colors red, green, blue and yellow are all in the form of 2^N. 
Other colors are ok as well, if they are expressed as combinations of the 2^N-colors. 

When something is a flag enum, then you can also do this:

```cpp
const auto circle =
    Circle{.radius = 2.0, .color = Color::blue | Color::green};
```

Which will then be represented as follows:

```json
{"radius":2.0,"color":"blue|green"}
```

Using orange is fine as well:

```cpp
const auto circle =
    Circle{.radius = 2.0, .color = Color::orange};
```

But it will be represented in terms of the base colors (which are in the form 2^N):

```json
{"radius":2.0,"color":"red|yellow"}
```

You can also combine orange with another color:

```cpp
const auto circle =
    Circle{.radius = 2.0, .color = Color::blue | Color::orange};
```

Which will result in this:

```json
{"radius":2.0,"color":"red|blue|yellow"}
```

### What happens if an enum cannot be matched?

If an enum cannot be matched, it will be written as an integer. This works for both normal enums as well as flag enums.

For instance, if you use the `Color` flag enum as described in the previous section, you can do something like this:

```cpp
// Enums are not supposed to be used like this, but it is defined behavior for scoped enumerations.
const auto circle = Circle{.radius = 2.0, .color = static_cast<Color>(10000)};
```

This will be represented as follows:

```json
{"radius":2.0,"color":"16|red|green|blue|8192"}
```

This works, because 16 + 256 + 512 + 1024 + 8192 = 10000. Flag enums are *always* represented in terms of 2^N-numbers.

### General-purpose enumeration utilities

reflect-cpp also allows you to directly convert between enumerator values and strings:

```cpp
enum class Color { red, green, blue };

auto name = rfl::enum_to_string(Color::red);       // "red"
auto value = rfl::string_to_enum<Color>("red");    // result containing Color::red
auto value = rfl::string_to_enum<Color>("greem");  // error result
```

This works with normal and flag enums, and behaves just like serialization of types containing enumerations as described above.

You can also inspect the defined enumerators of an enum type (including at compile-time):

```cpp
enum class Color { red, green, blue };

// This produces a named tuple where the keys are "red", "green", and "blue", with corresponding
// values Color::red, Color::green, and Color::blue.
auto enumerators_named_tuple = rfl::get_enumerators<Color>();

// You can iterate over the enumerators like this:
enumerators_named_tuple.apply([&](const auto& field) {
  // field.name() will be "red", "green", "blue"
  // field.value() will be Color::red, Color::green, Color::blue
});

// This produces the same data as an std::array containing std::pair<std::string_view, Color>,
// which can be inspected at compile-time.
constexpr auto enumerator_array = rfl::get_enumerator_array<Color>();
```

In case it's more convenient to get the enumerator values as values of the enum's underlying type rather than values of the enum type itself,
there is also `rfl::get_underlying_enumerators<EnumType>()` and `rfl::get_underlying_enumerator_array<EnumType>()`.

## Literals

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

**Example**

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
## Support for standard containers

The serialization and deserialization of containers is handled as follows:

`std::map<std::string, ...>`, `std::unordered_map<std::string, ...>`, 
maps with any numeric types (integral and floating points) as keys,
and any objects where the `ReflectionType` is either a `std::string` or
a numeric type are treated as objects.

Consider the following map:

```cpp
std::map<std::string, std::string> homer;
homer["firstName"] = "Homer";
homer["lastName"] = "Simpson";
```

This will be represented as follows:

```json
{"firstName":"Homer","lastName":"Simpson"}
```

The same is true for numeric types:

```cpp
std::map<int, std::string> homer;
homer[1] = "Homer";
homer[2] = "Simpson";
```

This will be represented as follows:

```json
{"1":"Homer","2":"Simpson"}
```

If you wrap your keys into a `rfl::Validator`, this will be treated as an object as well:

```cpp
using Between1and3 = rfl::Validator<int, rfl::Minimum<1>, rfl::Maximum<3>>;
std::map<Between1and3, std::string> homer;
homer[1] = "Homer";
homer[2] = "Simpson";
```

This will also be represented as follows:

```json
{"1":"Homer","2":"Simpson"}
```

All other supported standard containers 
(other than `std::variant`, `std::optional`, `std::unique_ptr` and `std::shared_ptr`) 
will be represented as arrays. Containers for which the `value_type`
is a key-value-pair will be represented as arrays of pairs. 

## Variants and tagged unions

### untagged

Sometimes you know that the JSON object can be one of several alternatives. For example,
you might have several shapes like `Circle`, `Rectangle` or `Square`. For these kind of 
cases, the C++ standard library contains `std::variant`, even though we we recommend you to use `rfl::Variant` instead:

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

using Shapes = rfl::Variant<Circle, Rectangle, Square>;

const Shapes r = Rectangle{.height = 10, .width = 5};

const auto json_string = rfl::json::write(r);

const auto r2 = rfl::json::read<Shapes>(json_string);
```

This code will compile just fine and work as intended. However, there are
several problems with this:

1. It is in inefficient: The parser has to read the fields for all of the different alternatives until it can't find a required field in the JSON object. It will then move on to the next alternative.

2. It leads to confusing error messages: If none of the alternatives can be matched, you will get an error message telling you why each of the alternatives couldn't be matched. Such error messages are very long-winding and hard to read.

3. It is dangerous. Imagine we had written `std::variant<Circle, Square, Rectangle>` instead of `std::variant<Circle, Rectangle, Square>`. This would mean that `Rectangle` could never be matched, because the fields in `Square` are a subset of `Rectangle`. This leads to very confusing bugs.

### internally tagged

One way to solve this problem is to add a tag inside the class. That is why we have provided a helper class for these purposes: `rfl::TaggedUnion`.

TaggedUnions use the name of the struct as an identifying tag. It will then try to take that field from the JSON object, match it to the correct alternative and then only parse the correct alternative.

We will now rewrite the example from above using `rfl::TaggedUnion`:

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

// Now you tell rfl::TaggedUnion that you want it to write the name
// of the struct into an extra field called "shape".
using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

const Shapes r = Rectangle{.height = 10, .width = 5};

const auto json_string = rfl::json::write(r);

const auto r2 = rfl::json::read<Shapes>(json_string);
```

The resulting JSON looks like this:
```json
{"shape":"Rectangle","height":10.0,"width":5.0}
```

Because the tag is inside the JSON object, this is called *internally tagged*. 
It is the standard in Python's [pydantic](https://docs.pydantic.dev/latest/api/standard_library_types/#union).

It is also possible to set the tag explicitly:

```cpp
struct Circle {
  using Tag = rfl::Literal<"circle", "Circle">;
  double radius;
};

struct Rectangle {
  using Tag = rfl::Literal<"rectangle", "Rectangle">;
  double height;
  double width;
};

struct Square {
  using Tag = rfl::Literal<"square", "Square">;
  double width;
};

using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

const Shapes r = Rectangle{.height = 10, .width = 5};

const auto json_string = rfl::json::write(r);

const auto r2 = rfl::json::read<Shapes>(json_string);
```

The JSON generated by `rfl::json::write` looks like this:

```json
{"shape":"rectangle","height":10.0,"width":5.0}
```

However, `rfl::json::read` would also accept this:

```json
{"shape":"Rectangle","height":10.0,"width":5.0}
```

If the behavior of your program depends on the value the user has decided to pass, then
you can also set the tag as a field explicitly. 

For instance, if it somehow makes a difference
whether the JSON contains "Rectangle" or "rectangle", you can do the following:

```cpp
struct Circle {
  rfl::Literal<"circle", "Circle"> shape;
  double radius;
};

struct Rectangle {
  rfl::Literal<"rectangle", "Rectangle"> shape;
  double height;
  double width;
};

struct Square {
  rfl::Literal<"square", "Square"> shape;
  double width;
};

using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

const Shapes r = Rectangle{
  .shape = rfl::Literal<"rectangle", "Rectangle">::make<"Rectangle">(),
  .height = 10,
  .width = 5};

const auto json_string = rfl::json::write(r);

const auto r2 = rfl::json::read<Shapes>(json_string);
```

Note that in this case the type of the field `shape` MUST be `rfl::Literal`.
Also note that this is exactly how tagged unions work in Pydantic.

### externally tagged

Another approach is to add external tags. 

You can do that using `rfl::Field`:

```cpp
using TaggedVariant = rfl::Variant<rfl::Field<"option1", Type1>, rfl::Field<"option2", Type2>, ...>;
```

The parser can now figure this out and will only try to parse the field that was indicated by the field name.
Duplicate field names will lead to compile-time errors.

We can rewrite the example from above:

```cpp
// Circle, Rectangle and Square are the same as above.
using Shapes = rfl::Variant<rfl::Field<"circle", Circle>,
                            rfl::Field<"rectangle", Rectangle>,
                            rfl::Field<"square", Square>>;

const Shapes r =
    rfl::make_field<"rectangle">(Rectangle{.height = 10, .width = 5});

const auto json_string = rfl::json::write(r);

const auto r2 = rfl::json::read<Shapes>(json_string);
```

The resulting JSON looks like this:
```json
{"rectangle":{"height":10.0,"width":5.0}}
```

Because the tag is external, this is called *externally tagged*. It is the standard in Rust's [serde-json](https://serde.rs/enum-representations.html).


### The visitor pattern

In C++, the idiomatic way to handle `std::variant` and `rfl::TaggedUnion` is the [visitor pattern](https://en.cppreference.com/w/cpp/utility/variant/visit).

For instance, the externally tagged `rfl::Variant` from the example above could be handled like this:

```cpp
using Shapes = rfl::Variant<rfl::Field<"circle", Circle>,
                            rfl::Field<"rectangle", Rectangle>,
                            rfl::Field<"square", Square>>;

const Shapes my_shape =
    rfl::make_field<"rectangle">(Rectangle{.height = 10, .width = 5});

const auto handle_shapes = [](const auto& field) {
  using Name = typename std::decay_t(decltype(field))::Name;
  if constexpr (std::is_same<Name, rfl::Literal<"circle">>()) {
     std::cout << is circle, radius: << field.value().radius() << std::endl;
  } else if constexpr (std::is_same<Name, rfl::Literal<"rectangle">>()) {
     std::cout << is rectangle, width: << field.value().width() << ", height: " << field.value().height() << std::endl;
  } else if constexpr (std::is_same<Name, rfl::Literal<"square">>()) {
     std::cout << is square, width: << field.value().width() << std::endl;
  } else {
    // reflect-cpp also provides this very useful helper that ensures
    // at compile-time that you didn't forget anything.
    static_assert(rfl::always_false_v<Type>, "Not all cases were covered.");
  }
};

std::visit(handle_shapes, my_shape.variant());
```

Likewise, `rfl::TaggedUnion` contains `std::variant` under-the-hood. The original variant can be
retrieved using `.variant()`:

```cpp
using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

const Shapes my_shape = Rectangle{.height = 10, .width = 5};

const auto handle_shapes = [](const auto& s) {
  using Type = std::decay_t(decltype(s));
  if constexpr (std::is_same<Type, Circle>()) {
     std::cout << is circle, radius: << s.radius() << std::endl;
  } else if constexpr (std::is_same<Type, Rectangle>()) {
     std::cout << is rectangle, width: << s.width() << ", height: " << s.height() << std::endl;
  } else if constexpr (std::is_same<Type, Square>()) {
     std::cout << is square, width: << s.width() << std::endl;
  } else {
    static_assert(rfl::always_false_v<Type>, "Not all cases were covered.");
  }
};

std::visit(handle_shapes, my_shape.variant());
```





## Timestamp

`rfl::Timestamp` is a simple wrapper around `std::tm` (https://en.cppreference.com/w/cpp/chrono/c/tm).

As its template parameter it includes the time stamp format for serialization and deserialization.

Here is a reference for for how you can specify the format:

https://en.cppreference.com/w/cpp/chrono/c/strftime

In fact, `rfl::Timestamp` calls `strftime` and `strptime` under-the-hood.

They can be used in your struct like this:

```cpp
struct Person {
  rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">> birthday;
};
```

An `rfl::Timestamp` can be constructed from either a string or a `std::tm` struct.

```cpp
const auto person1 = Person{.birthday = "1970-01-01"};
const auto person2 = Person{.birthday = std::tm{...}};
```

You can access the underlying `std::tm` struct using the `.tm()` method and you can generate 
the string representation using the `.str()` method.

```cpp
const std::tm birthday = person1.birthday().tm();
std::cout << person1.birthday().str() << std::endl;
```

Note that constructing `rfl::Timestamp` from a string is convenient, but it might result in
an exception.

To avoid the exception you can also use `Timestamp<...>::from_string(...)`, which will
return an `rfl::Result<Timestamp<...>>` or `rfl::Error`.

```cpp
const rfl::Result<rfl::Timestamp<"%Y-%m-%d">> result = rfl::Timestamp<"%Y-%m-%d">::from_string("1970-01-01");
const rfl::Result<rfl::Timestamp<"%Y-%m-%d">> error = rfl::Timestamp<"%Y-%m-%d">::from_string("not a proper time format");
```
