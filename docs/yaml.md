# YAML 

For YAML support, you must also include the header `<rfl/yaml.hpp>` and include the yaml-cpp library (https://github.com/jbeder/yaml-cpp) in your project.

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

You can parse YAML strings like this:

```cpp
const rfl::Result<Person> result = rfl::yaml::read<Person>(yaml_string);
```

A `Person` can be serialized like this:

```cpp
const auto person = Person{...};
const std::string yaml_string = rfl::yaml::write(person);
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::yaml::load<Person>("/path/to/file.yml");

const auto person = Person{...};
rfl::yaml::save("/path/to/file.yml", person);
```

## Reading from and writing into streams

You can also read from and write into any `std::istream` and `std::ostream` respectively.

```cpp
const rfl::Result<Person> result = rfl::yaml::read<Person>(my_istream);

const auto person = Person{...};
rfl::yaml::write(person, my_ostream);
```

Note that `std::cout` is also an ostream, so this works as well:

```cpp
rfl::yaml::write(person, std::cout) << std::endl;
```

## Custom constructors

One of the great things about C++ is that it gives you control over
when and how you code is compiled.

For large and complex systems of structs, it is often a good idea to split up
your code into smaller compilation units. You can do so using custom constructors.

For the YAML format, these must be a static function on your struct or class called
`from_yaml_obj` that take a `rfl::yaml::Reader::InputVarType` as input and return
the class or the class wrapped in `rfl::Result`.

In your header file you can write something like this:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;

    using YAMLVar = typename rfl::yaml::Reader::InputVarType;
    static rfl::Result<Person> from_yaml_obj(const YAMLVar& _obj);
};
```

And in your source file, you implement `from_yaml_obj` as follows:

```cpp
rfl::Result<Person> Person::from_yaml_obj(const YAMLVar& _obj) {
    const auto from_nt = [](auto&& _nt) {
        return rfl::from_named_tuple<Person>(std::move(_nt));
    };
    return rfl::yaml::read<rfl::named_tuple_t<Person>>(_obj)
        .transform(from_nt);
}
```

This will force the compiler to only compile the YAML parsing when the
source file is compiled.

