# Enums

reflect-cpp supports scoped enumerations.

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

2. You cannot have more than 100 values and if you explicitly assign values, they must be between 0 and 99.

```cpp
/// OK
enum class Color1 { red, green, blue, yellow };

/// OK
enum struct Color2 { red, green, blue = 50, yellow };

/// unsupported - red is a negative value
enum Color3 { red = -10, green, blue, yellow };

/// unsupported - red is greater than 99
enum Color3 { red = 200, green, blue, yellow };
```