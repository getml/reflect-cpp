# Boost.Serialization

For Boost.Serialization support, you must also include the header `<rfl/boost_serialization.hpp>` and link to the [Boost.Serialization](https://www.boost.org/doc/libs/release/libs/serialization/) library.
Furthermore, when compiling reflect-cpp, you need to pass `-DREFLECTCPP_BOOST_SERIALIZATION=ON` to cmake. If you are using vcpkg, there
should be an appropriate feature that will abstract this away for you.

Unlike most other formats supported by reflect-cpp, which use document-tree based libraries, Boost.Serialization streams data sequentially through archive objects. reflect-cpp implements this as a schemaful binary format, similar to Avro and Cap'n Proto.

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
const std::vector<char> bytes = rfl::boost_serialization::write(person);
```

You can parse bytes like this:

```cpp
const rfl::Result<Person> result = rfl::boost_serialization::read<Person>(bytes);
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::boost_serialization::load<Person>("/path/to/file.bin");

const auto person = Person{...};
rfl::boost_serialization::save("/path/to/file.bin", person);
```

## Reading from and writing into streams

You can also read from and write into any `std::istream` and `std::ostream` respectively.

```cpp
const rfl::Result<Person> result = rfl::boost_serialization::read<Person>(my_istream);

const auto person = Person{...};
rfl::boost_serialization::write(person, my_ostream);
```

## Archive interop

One of the key features of the Boost.Serialization backend is that you can use it with existing Boost archives directly. This allows reflect-cpp types to participate in larger Boost serialization workflows.

```cpp
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <rfl/boost_serialization.hpp>

// Writing into an existing Boost archive
boost::archive::binary_oarchive oa(my_ostream);
rfl::boost_serialization::write(oa, person);

// Reading from an existing Boost archive
boost::archive::binary_iarchive ia(my_istream);
const auto result = rfl::boost_serialization::read_from_archive<
    Person, boost::archive::binary_iarchive,
    boost::archive::binary_oarchive>(ia);
```

This also works with text archives:

```cpp
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

boost::archive::text_oarchive oa(my_ostream);
rfl::boost_serialization::write(oa, person);

boost::archive::text_iarchive ia(my_istream);
const auto result = rfl::boost_serialization::read_from_archive<
    Person, boost::archive::text_iarchive,
    boost::archive::text_oarchive>(ia);
```
