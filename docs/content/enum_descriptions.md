# Enum descriptions

reflect-cpp allows you to add descriptions to individual enum values in JSON schemas. This is particularly useful when generating documentation or providing additional context for enum values in API specifications.

## Basic usage

To add descriptions to enum values, you need to specialize the `rfl::config::enum_descriptions` template for your enum type:

```cpp
enum class Color { red, green, blue };

template <>
struct rfl::config::enum_descriptions<Color> {
  static constexpr bool has_descriptions = true;
  static constexpr std::string_view get(Color value) {
    switch (value) {
      case Color::red:
        return "The color red";
      case Color::green:
        return "The color green";
      case Color::blue:
        return "The color blue";
      default:
        return "";
    }
  }
};
```

When you generate a JSON schema for a struct containing this enum, the descriptions will be included:

```cpp
struct Config {
  Color color;
};

const std::string json_schema = rfl::json::to_schema<Config>(rfl::json::pretty);
```

This will generate a schema where the enum is represented using `oneOf` with `const` and `description` fields:

```json
{
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "$ref": "#/$defs/Config",
    "$defs": {
        "Config": {
            "type": "object",
            "properties": {
                "color": {
                    "oneOf": [
                        {
                            "const": "red",
                            "description": "The color red"
                        },
                        {
                            "const": "green",
                            "description": "The color green"
                        },
                        {
                            "const": "blue",
                            "description": "The color blue"
                        }
                    ]
                }
            },
            "required": ["color"]
        }
    }
}
```

## Enums without descriptions

Enums that do not have `enum_descriptions` specialization will continue to use the standard `enum` format in JSON schemas:

```cpp
enum class Size { small, medium, large };

struct Config {
  Size size;
};
```

This will generate:

```json
{
    "type": "object",
    "properties": {
        "size": {
            "type": "string",
            "enum": ["small", "medium", "large"]
        }
    }
}
```

## Serialization and deserialization

Adding descriptions to enums does not affect serialization or deserialization behavior. The enum values are still serialized as strings and parsed the same way:

```cpp
const Config config{.color = Color::green};

const auto json_string = rfl::json::write(config);
// Result: {"color":"green"}

const auto parsed = rfl::json::read<Config>(json_string);
// Works exactly as before
```

The descriptions only affect the JSON schema generation, making it easier for users to understand what each enum value represents.
