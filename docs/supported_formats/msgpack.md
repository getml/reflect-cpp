# msgpack 

For msgpack support, you must also include the header `<rfl/msgpack.hpp>` and link to the msgpack-c library (https://github.com/msgpack/msgpack-c).

msgpack is a JSON-like binary format with an emphasis on small binary sizes.

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

A `Person` can be serialized like this:

```cpp
const auto person = Person{...};
const std::vector<char> bytes = rfl::msgpack::write(person);
```

You can parse bytes like this:

```cpp
const rfl::Result<Person> result = rfl::msgpack::read<Person>(bytes);
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::msgpack::load<Person>("/path/to/file.msgpack");

const auto person = Person{...};
rfl::msgpack::save("/path/to/file.msgpack", person);
```

## Reading from and writing into streams

You can also read from and write into any `std::istream` and `std::ostream` respectively.

```cpp
const rfl::Result<Person> result = rfl::msgpack::read<Person>(my_istream);

const auto person = Person{...};
rfl::msgpack::write(person, my_ostream);
```

Note that `std::cout` is also an ostream, so this works as well:

```cpp
rfl::msgpack::write(person, std::cout) << std::endl;
```

(Since msgpack is a binary format, the readability of this will be limited, but it might be useful for debugging).

## Custom constructors

One of the great things about C++ is that it gives you control over
when and how you code is compiled.

For large and complex systems of structs, it is often a good idea to split up
your code into smaller compilation units. You can do so using custom constructors.

For the msgpack format, these must be a static function on your struct or class called
`from_msgpack` that take a `rfl::msgpack::Reader::InputVarType` as input and return
the class or the class wrapped in `rfl::Result`.

In your header file you can write something like this:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;

    using InputVarType = typename rfl::msgpack::Reader::InputVarType;
    static rfl::Result<Person> from_msgpack(const InputVarType& _obj);
};
```

And in your source file, you implement `from_msgpack` as follows:

```cpp
rfl::Result<Person> Person::from_msgpack(const InputVarType& _obj) {
    const auto from_nt = [](auto&& _nt) {
        return rfl::from_named_tuple<Person>(std::move(_nt));
    };
    return rfl::msgpack::read<rfl::named_tuple_t<Person>>(_obj)
        .transform(from_nt);
}
```

This will force the compiler to only compile the msgpack parsing when the source file is compiled.
