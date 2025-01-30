# Avro 

For Avro support, you must also include the header `<rfl/avro.hpp>` and link to the [avro-c](https://avro.apache.org/docs/1.11.1/api/c/) library.
Furthermore, when compiling reflect-cpp, you need to pass `-DREFLECTCPP_AVRO=ON` to cmake.

Avro is a schemaful binary format. This sets it apart from most other formats supported by reflect-cpp, which are schemaless.

## Reading and writing

Suppose you have a struct like this:

```cpp
struct Person {
    std::string first_name;
    std::string last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;
    std::vector<Person> children;
};
```

A `Person` struct can be serialized to a bytes vector like this:

```cpp
const auto person = Person{...};
const std::vector<char> bytes = rfl::avro::write(person);
```

You can parse bytes like this:

```cpp
const rfl::Result<Person> result = rfl::avro::read<Person>(bytes);
```

## The schema

However, Avro is a schemaful format, so before you serialize or
deserialize, you have to declare a schema. In the two function calls
above, this is abstracted away.

But if you are repeatedly serializing or deserializing the same struct,
it is more efficient to generate the schema explicitly:

```cpp
const auto schema = rfl::avro::to_schema<Person>();

const auto person = Person{...};
const std::vector<char> bytes = rfl::avro::write(person, schema);

const rfl::Result<Person> result = rfl::avro::read<Person>(bytes, schema);
```

Avro schemas are created using a JSON-based schema language. You can
retrieve the JSON representation like this:

```cpp
// Both calls are equivalent.
schema.json_str();
schema.str();
```

In this case, the resulting JSON schema representation looks like this:

```json
{"type":"record","name":"Person","fields":[{"name":"first_name","type":{"type":"string"}},{"name":"last_name","type":{"type":"string"}},{"name":"birthday","type":{"type":"string"}},{"name":"children","type":{"type":"array","items":{"type":"Person"},"default":[]}}]}
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::avro::load<Person>("/path/to/file.avro");

const auto person = Person{...};
rfl::avro::save("/path/to/file.avro", person);
```

## Reading from and writing into streams

You can also read from and write into any `std::istream` and `std::ostream` respectively.

```cpp
const rfl::Result<Person> result = rfl::avro::read<Person>(my_istream);

const auto person = Person{...};
rfl::avro::write(person, my_ostream);
```

Note that `std::cout` is also an ostream, so this works as well:

```cpp
rfl::avro::write(person, std::cout) << std::endl;
```

(Since Avro is a binary format, the readability of this will be limited, but it might be useful for debugging).

## Custom constructors

One of the great things about C++ is that it gives you control over
when and how you code is compiled.

For large and complex systems of structs, it is often a good idea to split up
your code into smaller compilation units. You can do so using custom constructors.

For the Avro format, these must be a static function on your struct or class called
`from_avro` that take a `rfl::avro::Reader::InputVarType` as input and return
the class or the class wrapped in `rfl::Result`.

In your header file you can write something like this:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;

    using InputVarType = typename rfl::avro::Reader::InputVarType;
    static rfl::Result<Person> from_avro(const InputVarType& _obj);
};
```

And in your source file, you implement `from_avro` as follows:

```cpp
rfl::Result<Person> Person::from_avro(const InputVarType& _obj) {
    const auto from_nt = [](auto&& _nt) {
        return rfl::from_named_tuple<Person>(std::move(_nt));
    };
    return rfl::avro::read<rfl::named_tuple_t<Person>>(_obj)
        .transform(from_nt);
}
```

This will force the compiler to only compile the Avro parsing when the source file is compiled.
