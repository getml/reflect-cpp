# Environment Variables

For environment variable support, you must also include the header `<rfl/env.hpp>`.

Environment variables provide a flat, hierarchical key-value store for configuration. This module serializes nested structs into environment variable names using `_` as a separator (e.g., `DATABASE_HOST`) and parses them back.

All field names are converted to uppercase by default.

## Reading and writing

Suppose you have a struct like this:

```cpp
struct Settings {
    std::string host;
    int port;
    double rate;
    bool verbose;
};
```

You can serialize to environment variables like this:

```cpp
const auto settings = Settings{...};
rfl::env::write(settings);
```

You can parse from environment variables like this:

```cpp
const rfl::Result<Settings> result = rfl::env::read<Settings>();
```

## Nested structs

Nested structs are serialized using `_` as a separator between field names:

```cpp
struct DatabaseSettings {
    std::string host;
    int port;
};

struct Settings {
    std::string host;
    DatabaseSettings database;
};
```

This produces the following environment variables:

- `HOST` — the value of `host`
- `DATABASE_HOST` — the value of `database.host`
- `DATABASE_PORT` — the value of `database.port`

## Arrays

Arrays are serialized with `_INDEX` suffix on the field name:

```cpp
struct Settings {
    std::string host;
    std::vector<std::string> tags;
};
```

This produces:

- `HOST` — the value of `host`
- `TAGS_0` — the first element
- `TAGS_1` — the second element
- ...

## Enums

Enums are serialized using `rfl::enum_to_string()`, which produces the enumerator name (e.g., `"green"` for `Color::green`). If the enumerator has no name, the underlying integer value is used.

```cpp
enum class Color { red, green, blue };

struct Circle {
    float radius;
    Color color;
};
```

`color = Color::green` produces `COLOR=green`.

### Flag enums

Flag enums (detected via `enchantum::is_bitflag`) are serialized by joining matching flag names with `|`:

```cpp
enum class Permissions { read = 1, write = 2, execute = 4 };

inline Permissions operator|(Permissions a, Permissions b) {
    return static_cast<Permissions>(static_cast<int>(a) | static_cast<int>(b));
}
```

`permissions = Permissions::read | Permissions::write` produces `PERMISSIONS=read|write`.

## Tagged unions

Tagged unions work as expected, with the discriminant field serialized as a regular field:

```cpp
using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;
```

This produces a `SHAPE` field with the variant name (e.g., `SHAPE=RECTANGLE`).

## Processors

You can pass processors to `read` and `write` to customize behavior. By default, `ToAllCaps` is applied, which converts all field names to uppercase. You can add additional processors to the template parameter list:

```cpp
rfl::env::write<SomeProcessor>(settings);
```

## Custom constructors

Custom constructors are not supported for environment variable parsing.
