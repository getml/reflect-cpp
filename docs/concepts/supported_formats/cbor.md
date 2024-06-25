# CBOR 

For CBOR support, you must also include the header `<rfl/cbor.hpp>` and link to the tinycbor library (https://github.com/intel/tinycbor).

CBOR or Concise Binary Object Representation, is a JSON-like binary format with an emphasis on small binary sizes.

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

A `Person` can be turned into a bytes vector like this:

```cpp
const auto person = Person{...};
const std::vector<char> bytes = rfl::cbor::write(person);
```

You can parse bytes like this:

```cpp
const rfl::Result<Person> result = rfl::cbor::read<Person>(bytes);
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::cbor::load<Person>("/path/to/file.cbor");

const auto person = Person{...};
rfl::cbor::save("/path/to/file.cbor", person);
```

## Reading from and writing into streams

You can also read from and write into any `std::istream` and `std::ostream` respectively.

```cpp
const rfl::Result<Person> result = rfl::cbor::read<Person>(my_istream);

const auto person = Person{...};
rfl::cbor::write(person, my_ostream);
```

Note that `std::cout` is also an ostream, so this works as well:

```cpp
rfl::cbor::write(person, std::cout) << std::endl;
```

(Since CBOR is a binary format, the readability of this will be limited, but it might be useful for debugging).

## Custom constructors

One of the great things about C++ is that it gives you control over
when and how you code is compiled.

For large and complex systems of structs, it is often a good idea to split up
your code into smaller compilation units. You can do so using custom constructors.

For the CBOR format, these must be a static function on your struct or class called
`from_cbor` that take a `rfl::cbor::Reader::InputVarType` as input and return
the class or the class wrapped in `rfl::Result`.

In your header file you can write something like this:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;

    using InputVarType = typename rfl::cbor::Reader::InputVarType;
    static rfl::Result<Person> from_cbor(const InputVarType& _obj);
};
```

And in your source file, you implement `from_cbor` as follows:

```cpp
rfl::Result<Person> Person::from_cbor(const InputVarType& _obj) {
    const auto from_nt = [](auto&& _nt) {
        return rfl::from_named_tuple<Person>(std::move(_nt));
    };
    return rfl::cbor::read<rfl::named_tuple_t<Person>>(_obj)
        .transform(from_nt);
}
```

This will force the compiler to only compile the CBOR parsing when the source file is compiled.
