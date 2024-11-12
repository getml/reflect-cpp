# UBJSON 

For UBJSON support, you must also include the header `<rfl/ubjson.hpp>` and include the jsoncons library (https://github.com/danielaparker/jsoncons). Note that it is header-only.

UBJSON is a JSON-like binary format.

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

A `Person` can be serialized to a bytes vector like this:

```cpp
const auto person = Person{...};
const std::vector<char> bytes = rfl::ubjson::write(person);
```

You can parse bytes like this:

```cpp
const rfl::Result<Person> result = rfl::ubjson::read<Person>(bytes);
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::ubjson::load<Person>("/path/to/file.ubjson");

const auto person = Person{...};
rfl::ubjson::save("/path/to/file.ubjson", person);
```

## Reading from and writing into streams

You can also read from and write into any `std::istream` and `std::ostream` respectively.

```cpp
const rfl::Result<Person> result = rfl::ubjson::read<Person>(my_istream);

const auto person = Person{...};
rfl::ubjson::write(person, my_ostream);
```

Note that `std::cout` is also an ostream, so this works as well:

```cpp
rfl::ubjson::write(person, std::cout) << std::endl;
```

(Since UBJSON is a binary format, the readability of this will be limited, but it might be useful for debugging).

## Custom constructors

One of the great things about C++ is that it gives you control over
when and how you code is compiled.

For large and complex systems of structs, it is often a good idea to split up
your code into smaller compilation units. You can do so using custom constructors.

For the UBJSON format, these must be a static function on your struct or class called
`from_ubjson` that take a `rfl::ubjson::Reader::InputVarType` as input and return
the class or the class wrapped in `rfl::Result`.

In your header file you can write something like this:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;

    using InputVarType = typename rfl::ubjson::Reader::InputVarType;
    static rfl::Result<Person> from_ubjson(const InputVarType& _obj);
};
```

And in your source file, you implement `from_ubjson` as follows:

```cpp
rfl::Result<Person> Person::from_ubjson(const InputVarType& _obj) {
    const auto from_nt = [](auto&& _nt) {
        return rfl::from_named_tuple<Person>(std::move(_nt));
    };
    return rfl::ubjson::read<rfl::named_tuple_t<Person>>(_obj)
        .transform(from_nt);
}
```

This will force the compiler to only compile the UBJSON parsing when the source file is compiled.
