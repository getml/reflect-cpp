# yas

For [yas](https://github.com/niXman/yas) support, you must also include the header `<rfl/yas.hpp>` and link to the yas library.
Furthermore, when compiling reflect-cpp, you need to pass `-DREFLECTCPP_YAS=ON` to cmake. If you are using vcpkg, there should be an appropriate feature that will abstract this away for you.

yas is a very fast and compact serialization library. reflect-cpp implements it as a schemaful binary format.

Note that even though yas is probably the fastest format supported by reflect-cpp, that speed comes at the cost of lack of backwards compatibility. If you change the structure of your data, you will not be able to read old files anymore. If you need backwards compatibility, consider using a different format like flexbuffers or msgpack, which are also supported by reflect-cpp. Both of these are very fast and compact as well, but they are schema-less and therefore more flexible when it comes to changing the structure of your data.

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
const std::vector<char> bytes = rfl::yas::write(person);
```

You can parse bytes like this:

```cpp
const rfl::Result<Person> result = rfl::yas::read<Person>(bytes);
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::yas::load<Person>("/path/to/file.bin");

const auto person = Person{...};
rfl::yas::save("/path/to/file.bin", person);
```

## Reading from and writing into streams

You can also read from and write into any `std::ostream` respectively.

```cpp
const auto person = Person{...};
rfl::yas::write(person, my_ostream);
```

