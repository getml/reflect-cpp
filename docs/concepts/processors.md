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
- `rfl::AddTagsToVariants` 
- `rfl::AllowRawPtrs` 
- `rfl::DefaultIfMissing` 
- `rfl::NoExtraFields` 
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

### `rfl::AddTagsToVariants` 

This processor automatically adds tags to variants. Consider the following example:

```cpp
struct button_pressed_t {};

struct button_released_t {};

struct key_pressed_t {
  char key;
};

using my_event_type_t =
    std::variant<button_pressed_t, button_released_t, key_pressed_t, int>;
```

The problem here is that `button_pressed_t` and `button_released_t` virtually look
indistinguishable when they are serialized. The will both be serialized to `{}`.

But you can add this processor to automatically add tags and avoid the problem:

```cpp
const auto vec = std::vector<my_event_type_t>(
  {button_pressed_t{}, button_released_t{}, key_pressed_t{'c'}, 3});

const auto json_string = rfl::json::write<rfl::AddTagsToVariants>(vec);

rfl::json::write<std::vector<my_event_type_t>, rfl::AddTagsToVariants>(json_string);
```

`vec` will now be serialized as follows:

```json
[{"button_pressed_t":{}},{"button_released_t":{}},{"key_pressed_t":{"key":99}},{"int":3}]
```

You can also set your own custom tags like this:

```cpp
struct key_pressed_t {
  using Tag = rfl::Literal<"your_custom_tag">;
  char key;
};
```

`key_pressed_t` will now be serialized as follows:

```json
{"your_custom_tag":{"key":99}}
```

Note that there are other ways to address problems like this, for instance `rfl::TaggedUnion`.
Please refer to the relevant sections of the documentation.

### `rfl::AllowRawPtrs`

By default, reflect-cpp does not allow *reading into* raw pointers. (*Writing from* raw pointers is never a problem.) This is because reading into raw pointers means that the library will allocate memory that the user then has to manually delete. This can lead to misunderstandings and memory leaks.

You might want to consider using some alternatives, such as `std::unique_ptr`, `rfl::Box`, 
`std::shared_ptr`, `rfl::Ref` or `std::optional`. But if you absolutely have to use raw pointers, you can pass `rfl::AllowRawPtrs` to `read`:

```cpp
struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::vector<Person>* children;`
};

const auto person =
  rfl::json::read<Person, rfl::AllowRawPtrs>(json_str);
```

However, you must keep in mind that it is now **YOUR** responsibility
to clean up. Otherwise, there **WILL** be a memory leak.

```cpp
void delete_raw_pointers(const Person& _person) {
    if (!_person.children) {
        return;
    }
    for (const auto& child: _person.children) {
        delete_raw_pointers(child);
    }
    delete _person.children;
}

delete_raw_pointers(person);
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

### `rfl::NoExtraFields`

When reading an object and the object contains a field that cannot be 
matched to any of the fields in the struct, that field is simply ignored.

However, when `rfl::NoExtraFields` is added to `read`, then such extra fields
will lead to an error. 

This can be overriden by adding `rfl::ExtraFields` to the struct.

Example:

```cpp
struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
};
```

```json
{"first_name":"Homer","last_name":"Simpson","extra_field":0}
```

If you call `rfl::json::read<Person>(json_string)`, then `extra_field` will 
simply be ignored.

But if you call `rfl::json::read<Person, rfl::NoExtraFields>(json_string)`,
you will get an error.

However, suppose the struct looked like this:

```cpp
struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  rfl::ExtraFields<int> extras;
};
```

In this case, `rfl::json::read<Person, rfl::NoExtraFields>(json_string)`
will not fail, because `extra_field` would be included in `extras`.

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
