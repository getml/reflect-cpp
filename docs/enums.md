# Enums

reflect-cpp supports scoped enumerations. They can either come in the form of normal enumerations or flag enums.

## Normal enumerations

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

## Flag enums

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

## What happens if an enum cannot be matched?

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

## General-purpose enumeration utilities

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
