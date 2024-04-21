# JSON

For JSON support, you must also include the header `<rfl/json.hpp>`.

## Reading and writing

Suppose you have a struct like this:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;
    std::vector<Person> children;
};
```

You can parse JSON strings like this:

```cpp
const rfl::Result<Person> result = rfl::json::read<Person>(json_string);
```

A `Person` struct can be serialized like this:

```cpp
const auto person = Person{...};
const std::string json_string = rfl::json::write(person);
```

If you want a "pretty" JSON representation, you can do this:

```cpp
const std::string json_string = rfl::json::write(person, rfl::json::pretty);
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::json::load<Person>("/path/to/file.json");

const auto person = Person{...};
rfl::json::save("/path/to/file.json", person);
```

`rfl::json::pretty` will work here as well:

```cpp
rfl::json::save("/path/to/file.json", person, rfl::json::pretty);
```

## Reading from and writing into streams

You can also read from and write into any `std::istream` and `std::ostream` respectively.

```cpp
const rfl::Result<Person> result = rfl::json::read<Person>(my_istream);

const auto person = Person{...};
rfl::json::write(person, my_ostream);
```

`rfl::json::pretty` will work here as well:

```cpp
rfl::json::write(person, my_ostream, rfl::json::pretty);
```

Note that `std::cout` is also an ostream, so this works as well:

```cpp
rfl::json::write(person, std::cout) << std::endl;
```

## Custom constructors

One of the great things about C++ is that it gives you control over
when and how you code is compiled.

For large and complex systems of structs, it is often a good idea to split up
your code into smaller compilation units. You can do so using custom constructors.

For the JSON format, these must be a static function on your struct or class called
`from_json_obj` that take a `rfl::json::Reader::InputVarType` as input and return
the class or the class wrapped in `rfl::Result`.

In your header file you can write something like this:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;

    using JSONVar = typename rfl::json::Reader::InputVarType;
    static rfl::Result<Person> from_json_obj(const JSONVar& _obj);
};
```

And in your source file, you implement `from_json_obj` as follows:

```cpp
rfl::Result<Person> Person::from_json_obj(const JSONVar& _obj) {
    const auto from_nt = [](auto&& _nt) {
        return rfl::from_named_tuple<Person>(std::move(_nt));
    };
    return rfl::json::read<rfl::named_tuple_t<Person>>(_obj)
        .transform(from_nt);
}
```

This will force the compiler to only compile the JSON parsing when the
source file is compiled.

