# Cap'n Proto 

For Cap'n Proto support, you must also include the header `<rfl/capnproto.hpp>` and link to the [capnproto](https://capnproto.org) library.
Furthermore, when compiling reflect-cpp, you need to pass `-DREFLECTCPP_CAPNPROTO=ON` to cmake. If you are using vcpkg or Conan, there
should be an appropriate feature (vcpkg) or option (Conan) that will abstract this away for you.

Cap'n Proto is a schemaful binary format. This sets it apart from most other formats supported by reflect-cpp, which are schemaless.

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
const std::vector<char> bytes = rfl::capnproto::write(person);
```

You can parse bytes like this:

```cpp
const rfl::Result<Person> result = rfl::capnproto::read<Person>(bytes);
```

## The schema

However, Cap'n Proto is a schemaful format, so before you serialize or
deserialize, you have to declare a schema. In the two function calls
above, this is abstracted away.

If you want to, you can pass the schema explicitly, but it will not
yield any performance gains, because the schemata are always created
upfront:

```cpp
const auto schema = rfl::capnproto::to_schema<Person>();

const auto person = Person{...};
const std::vector<char> bytes = rfl::capnproto::write(person, schema);

const rfl::Result<Person> result = rfl::capnproto::read<Person>(bytes, schema);
```

Cap'n Proto schemas are created using a schema language. You can
retrieve the schema like this:

```cpp
schema.str();
```

In this case, the resulting schema representation looks like this:

```
@0xdbb9ad1f14bf0b36;

struct Person {
  firstName @0 :Text;
  lastName @1 :Text;
  birthday @2 :Text;
  children @3 :List(Person);
}
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::capnproto::load<Person>("/path/to/file.capnproto");

const auto person = Person{...};
rfl::capnproto::save("/path/to/file.capnproto", person);
```

## Reading from and writing into streams

You can also read from and write into any `std::istream` and `std::ostream` respectively.

```cpp
const rfl::Result<Person> result = rfl::capnproto::read<Person>(my_istream);

const auto person = Person{...};
rfl::capnproto::write(person, my_ostream);
```

Note that `std::cout` is also an ostream, so this works as well:

```cpp
rfl::capnproto::write(person, std::cout) << std::endl;
```

(Since Cap'n Proto is a binary format, the readability of this will be limited, but it might be useful for debugging).

## Custom constructors

One of the great things about C++ is that it gives you control over
when and how you code is compiled.

For large and complex systems of structs, it is often a good idea to split up
your code into smaller compilation units. You can do so using custom constructors.

For the Cap'n Proto format, these must be a static function on your struct or class called
`from_capnproto` that take a `rfl::capnproto::Reader::InputVarType` as input and return
the class or the class wrapped in `rfl::Result`.

In your header file you can write something like this:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;

    using InputVarType = typename rfl::capnproto::Reader::InputVarType;
    static rfl::Result<Person> from_capnproto(const InputVarType& _obj);
};
```

And in your source file, you implement `from_capnproto` as follows:

```cpp
rfl::Result<Person> Person::from_capnproto(const InputVarType& _obj) {
    const auto from_nt = [](auto&& _nt) {
        return rfl::from_named_tuple<Person>(std::move(_nt));
    };
    return rfl::capnproto::read<rfl::named_tuple_t<Person>>(_obj)
        .transform(from_nt);
}
```

This will force the compiler to only compile the Cap'n Proto parsing when the source file is compiled.
