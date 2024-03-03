# BSON 

For BSON support, you must also include the header `<rfl/bson.hpp>` and link to the libbson library (https://github.com/mongodb/libbson).

Like the name implies, BSON is a JSON-like binary format. It is most notably used by MongoDB.

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

A `person` can be serialized like this:

```cpp
const auto person = Person{...};
const auto bytes = rfl::bson::write(person);
```

You can parse bytes like this:

```cpp
const rfl::Result<Person> result = rfl::bson::read<Person>(bytes);
```

You can also parse raw pointers (either `const char*` or `const uint8_t*`):

```cpp
const rfl::Result<Person> result = rfl::bson::read<Person>(ptr, length);
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::bson::load<Person>("/path/to/file.bson");

const auto person = Person{...};
rfl::bson::save("/path/to/file.bson", person);
```

## Reading from and writing into streams

You can also read from and write into any `std::istream` and `std::ostream` respectively.

```cpp
const rfl::Result<Person> result = rfl::bson::read<Person>(my_istream);

const auto person = Person{...};
rfl::bson::write(person, my_ostream);
```

Note that `std::cout` is also an ostream, so this works as well:

```cpp
rfl::bson::write(person, std::cout) << std::endl;
```

(Since BSON is a binary format, the readability of this will be limited, but it might be useful for debugging).

## Custom constructors

One of the great things about C++ is that it gives you control over
when and how you code is compiled.

For large and complex systems of structs, it is often a good idea to split up
your code into smaller compilation units. You can do so using custom constructors.

For the BSON format, these must be a static function on your struct or class called
`from_bson` that take a `rfl::bson::Reader::InputVarType` as input and return
the class or the class wrapped in `rfl::Result`.

In your header file you can write something like this:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;

    using InputVarType = typename rfl::bson::Reader::InputVarType;
    static rfl::Result<Person> from_bson(const InputVarType& _obj);
};
```

And in your source file, you implement `from_bson` as follows:

```cpp
rfl::Result<Person> Person::from_bson(const InputVarType& _obj) {
    const auto from_nt = [](auto&& _nt) {
        return rfl::from_named_tuple<Person>(std::move(_nt));
    };
    return rfl::bson::read<rfl::named_tuple_t<Person>>(_obj)
        .transform(from_nt);
}
```

This will force the compiler to only compile the BSON parsing when the source file is compiled.
