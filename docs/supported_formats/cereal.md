# Cereal

For Cereal support, you must also include the header `<rfl/cereal.hpp>` and link to the [Cereal](https://uscilab.github.io/cereal/) library.
Furthermore, when compiling reflect-cpp, you need to pass `-DREFLECTCPP_CEREAL=ON` to cmake.

Cereal is a C++ serialization library that provides multiple archive formats including binary, portable binary, JSON, and XML formats, even though we use the portable binary format by default. 
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
const std::vector<char> bytes = rfl::cereal::write(person);
```

You can parse bytes like this:

```cpp
const rfl::Result<Person> result = rfl::cereal::read<Person>(bytes);
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::cereal::load<Person>("/path/to/file.cereal");

const auto person = Person{...};
rfl::cereal::save("/path/to/file.cereal", person);
```

## Reading from and writing into streams

You can also read from and write into any `std::istream` and `std::ostream` respectively.

```cpp
const rfl::Result<Person> result = rfl::cereal::read<Person>(my_istream);

const auto person = Person{...};
rfl::cereal::write(person, my_ostream);
```

Note that `std::cout` is also an ostream, so this works as well:

```cpp
rfl::cereal::write(person, std::cout) << std::endl;
```

(Since Cereal binary format is a binary format, the readability of this will be limited, but it might be useful for debugging).

## Custom constructors

One of the great things about C++ is that it gives you control over
when and how you code is compiled.

For large and complex systems of structs, it is often a good idea to split up
your code into smaller compilation units. You can do so using custom constructors.

For the Cereal format, these must be a static function on your struct or class called
`from_cereal_obj` that take a `rfl::cereal::Reader::InputVarType` as input and return
the class or the class wrapped in `rfl::Result`.

In your header file you can write something like this:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;

    using InputVarType = rfl::cereal::Reader::InputVarType;
    static rfl::Result<Person> from_cereal_obj(const InputVarType& _obj);
};
```

And in your source file, you implement `from_cereal_obj` as follows:

```cpp
rfl::Result<Person> Person::from_cereal_obj(const InputVarType& _obj) {
    const auto from_nt = [](auto&& _nt) {
        return rfl::from_named_tuple<Person>(std::move(_nt));
    };
    return rfl::cereal::read<rfl::named_tuple_t<Person>>(*_obj.archive_)
        .transform(from_nt);
}
```

This will force the compiler to only compile the Cereal parsing when the source file is compiled.
