# Processors 

Processors can be used to apply transformations to struct serialization and deserialization.

For instance, C++ [usually](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rl-camel) uses `snake_case`, but JSON uses `camelCase`. One way to handle this is `rfl::Rename`, but a more automated way would be to use a *processor*:

```cpp
struct Person {
    std::string first_name;
    std::string last_name;
    std::vector<Person> children;
};

const auto homer =
    Person{.first_name = "Homer",
           .last_name = "Simpson",
           .age = 45};

const auto json_string = 
  rfl::json::write<rfl::SnakeCaseToCamelCase>(homer);

const auto homer2 = 
  rfl::json::read<Person, rfl::SnakeCaseToCamelCase>(json_string).value();
```

The resulting JSON string looks like this:

```json
{"firstName":"Homer","lastName":"Simpson","age":45}
```

## Supported processors

reflect-cpp currently supports the following processors:

- `rfl::AddStructName` 
- `rfl::DefaultIfMissing` 
- `rfl::NoFieldNames` 
- `rfl::NoOptionals` 
- `rfl::UnderlyingEnums` 
- `rfl::SnakeCaseToCamelCase` 
- `rfl::SnakeCaseToPascalCase` 

### `rfl::AddStructName` 

It is also possible to add the struct name as an additional field, like this:

```cpp
const auto json_string = 
  rfl::json::write<rfl::AddStructName<"type">>(homer);

const auto homer2 = 
  rfl::json::read<Person, rfl::AddStructName<"type">>(json_string).value();
```

The resulting JSON string looks like this:

```json
{"type":"Person","first_name":"Homer","last_name":"Simpson","age":45}
```

### `rfl::DefaultIfMissing`

The `rfl::DefaultIfMissing` processor is only relevant for reading data. For writing data, it will make no difference.

Usually, when fields are missing in the input data, this will lead to an error 
(unless they are optional fields).
But if you pass the `rfl::DefaultIfMissing` processor, then missing fields will be
replaced by their default value.

For instance, consider the following struct:

```cpp
struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::string town;
};
```

Suppose you are reading a JSON like this:

```json
{"first_name":"Homer"}
```

```cpp
rfl::json::read<Person, rfl::DefaultIfMissing>(json_string);
```

Then the resulting struct will be equivalent to what you would
have gotten had you read the following JSON string:

```json
{"first_name":"Homer","last_name":"Simpson","town":""}
```

`last_name` and `town` have been replaced by the default values.
Because you have not passed a default value to town, the default value
of the type is used instead.

### `rfl::NoFieldNames`

We can also remove the field names altogether: 

```cpp
const auto json_string = 
  rfl::json::write<rfl::NoFieldNames>(homer);

const auto homer2 = 
  rfl::json::read<Person, rfl::NoFieldNames>(json_string).value();
```

The resulting JSON string looks like this:

```json
["Homer","Simpson",45]
```

This is particularly relevant for binary formats, which do not emphasize readability,
like msgpack or flexbuffers. Removing the field names can reduce the size of the
resulting bytestrings and significantly speed up read and write time, 
depending on the dataset.

However, it makes it more difficult to maintain backwards compatability.

Note that `rfl::NoFieldNames` is not supported for BSON, TOML, XML, or YAML, due
to limitations of these formats. 

### `rfl::NoOptionals`

As we have seen in the section on optional fields, when a `std::optional` is
`std::nullopt`, it is usually not written at all. But if you want them to be explicitly
written as `null`, you can use this processor. The same thing applies to `std::shared_ptr` and 
`std::unique_ptr`.

```cpp
struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::optional<std::string> town = std::nullopt;
};

const auto homer = Person{.first_name = "Homer"};

rfl::json::write<rfl::NoOptionals>(homer);
```

The resulting JSON string looks like this:

```json
{"first_name":"Homer","last_name":"Simpson","town":null}
```

By default, `rfl::json::read` will accept both `"town":null` and just 
leaving out the field `town`. However, if you want to require the field
`town` to be included, you can add `rfl::NoOptionals` to `read`:

```
rfl::json::read<Person, rfl::NoOptionals>(json_string);
```

### `rfl::UnderlyingEnums`

By passing the processor `rfl::UnderlyingEnums`, fields of the enum type will be written and read as integers

```cpp
enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

const auto circle = Circle{.radius = 2.0, .color = Color::green};

rfl::json::write<rfl::UnderlyingEnums>(circle);
```

The resulting JSON string looks like this:

```json
{"radius":2.0,"color":1}
```

### `rfl::SnakeCaseToCamelCase`

Please refer to the example above.

### `rfl::SnakeCaseToPascalCase`

If you want `PascalCase` instead of `camelCase`, you can use the appropriate processor:

```cpp
const auto json_string = 
  rfl::json::write<rfl::SnakeCaseToPascalCase>(homer);

const auto homer2 = 
  rfl::json::read<Person, rfl::SnakeCaseToPascalCase>(json_string).value();
```

The resulting JSON string looks like this:

```json
{"FirstName":"Homer","LastName":"Simpson","Age":45}
```

## Combining several processors

You can combine several processors:

```cpp
const auto json_string = 
  rfl::json::write<rfl::SnakeCaseToCamelCase, rfl::AddStructName<"type">>(homer);

const auto homer2 = 
  rfl::json::read<Person, rfl::SnakeCaseToCamelCase, rfl::AddStructName<"type">>(json_string).value();
```

The resulting JSON string looks like this:

```json
{"type":"Person","firstName":"Homer","lastName":"Simpson","age":45}
```

When you have several processors, it is probably more convenient to combine them like this:

```cpp
using Processors = rfl::Processors<
    rfl::SnakeCaseToCamelCase, rfl::AddStructName<"type">>;

const auto json_string = rfl::json::write<Processors>(homer);

const auto homer2 = rfl::json::read<Person, Processors>(json_string).value();
```

The resulting JSON string looks like this:

```json
{"type":"Person","firstName":"Homer","lastName":"Simpson","age":45}
```

## Writing your own processors

In principle, writing your own processors is not very difficult. You need to define a struct, which takes has a static method called `process` taking a named tuple as an input and then returning a modified named tuple. The `process` method should accept the type of the original struct as a template parameter.

```cpp
struct MyOwnProcessor {
  template <class StructType>
  static auto process(auto&& _named_tuple) {...}
};
```
