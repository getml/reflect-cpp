## BSON 

For BSON support, you must also include the header `<rfl/bson.hpp>` and link to the libbson library (https://github.com/mongodb/libbson).

Like the name implies, BSON is a JSON-like binary format. It is most notably used by MongoDB.

### Reading and writing

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

### Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::bson::load<Person>("/path/to/file.bson");

const auto person = Person{...};
rfl::bson::save("/path/to/file.bson", person);
```

### Reading from and writing into streams

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

### Custom constructors

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

## CBOR 

For CBOR support, you must also include the header `<rfl/cbor.hpp>` and link to the tinycbor library (https://github.com/intel/tinycbor).

CBOR or Concise Binary Object Representation, is a JSON-like binary format with an emphasis on small binary sizes.

### Reading and writing

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

### Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::cbor::load<Person>("/path/to/file.cbor");

const auto person = Person{...};
rfl::cbor::save("/path/to/file.cbor", person);
```

### Reading from and writing into streams

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

### Custom constructors

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

## JSON

For JSON support, you must also include the header `<rfl/json.hpp>`.

### Reading and writing

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

### Loading and saving

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

### Reading from and writing into streams

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

### Custom constructors

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

## JSON schema

JSON schemata are a powerful tool for expressing the expected structure of your input. You can use it to validate your input before you even send it to your C++ backend, which will result in better UX.

It can also be used for code generation. For instance, tools such as https://app.quicktype.io/ allow you to generate code in multiple programming languages from the JSON schema (even though the validations are usually omitted). 

If you are interacting with Python, we warmly recommend https://docs.pydantic.dev/latest/integrations/datamodel_code_generator/. This allows you to generate Pydantic dataclasses, including the validation, from the JSON schema.

Note that this is only supported for JSON, not for other formats.

### Basic idea

Suppose you have a struct like this:

```cpp
struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Email email;
  std::vector<Person> children;
  float salary;
};
```

You can generate a JSON schema like this:

```cpp
const std::string json_schema = rfl::json::to_schema<Person>(rfl::json::pretty);
```

You do not have to pass `rfl::json::pretty`, but for the purposes of this documentation it is better to do so.

This will result in the following JSON schema:

```json
{
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "$ref": "#/definitions/Person",
    "definitions": {
        "Person": {
            "type": "object",
            "properties": {
                "children": {
                    "type": "array",
                    "items": {
                        "$ref": "#/definitions/Person"
                    }
                },
                "email": {
                    "type": "string",
                    "pattern": "^[a-zA-Z0-9._%+\\-]+@[a-zA-Z0-9.\\-]+\\.[a-zA-Z]{2,}$"
                },
                "first_name": {
                    "type": "string"
                },
                "last_name": {
                    "type": "string"
                },
                "salary": {
                    "type": "number"
                }
            },
            "required": [
                "children",
                "email",
                "first_name",
                "last_name",
                "salary"
            ]
        }
    }
}
```

You can insert this into the tools mentioned above and see the generated code.

### Adding descriptions

JSON schemata also often contain descriptions. reflect-cpp supports this as well.

```cpp
struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Description<"Must be a proper email in the form xxx@xxx.xxx.",
                   rfl::Email>
      email;
  rfl::Description<
      "The person's children. Pass an empty array for no children.",
      std::vector<Person>>
      children;
  float salary;
};
```

```cpp
const std::string json_schema = rfl::json::to_schema<Person>(rfl::json::pretty);
```

```json
{
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "$ref": "#/definitions/Person",
    "definitions": {
        "Person": {
            "type": "object",
            "properties": {
                "children": {
                    "type": "array",
                    "description": "The person's children. Pass an empty array for no children.",
                    "items": {
                        "$ref": "#/definitions/Person"
                    }
                },
                "email": {
                    "type": "string",
                    "description": "Must be a proper email in the form xxx@xxx.xxx.",
                    "pattern": "^[a-zA-Z0-9._%+\\-]+@[a-zA-Z0-9.\\-]+\\.[a-zA-Z]{2,}$"
                },
                "first_name": {
                    "type": "string"
                },
                "last_name": {
                    "type": "string"
                },
                "salary": {
                    "type": "number"
                }
            },
            "required": [
                "children",
                "email",
                "first_name",
                "last_name",
                "salary"
            ]
        }
    }
}
```

You also add a description to the entire JSON schema:

```cpp
const std::string json_schema = rfl::json::to_schema<
    rfl::Description<"JSON schema that describes the required "
                      "attributes for the person class.",
                      Person>>(rfl::json::pretty);
```

```json
{
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "$ref": "#/definitions/Person",
    "description": "JSON schema that describes the required attributes for the person class.",
    "definitions": {
        "Person": {
            "type": "object",
            "properties": {
                "children": {
                    "type": "array",
                    "description": "The person's children. Pass an empty array for no children.",
                    "items": {
                        "$ref": "#/definitions/Person"
                    }
                },
                "email": {
                    "type": "string",
                    "description": "Must be a proper email in the form xxx@xxx.xxx.",
                    "pattern": "^[a-zA-Z0-9._%+\\-]+@[a-zA-Z0-9.\\-]+\\.[a-zA-Z]{2,}$"
                },
                "first_name": {
                    "type": "string"
                },
                "last_name": {
                    "type": "string"
                },
                "salary": {
                    "type": "number"
                }
            },
            "required": [
                "children",
                "email",
                "first_name",
                "last_name",
                "salary"
            ]
        }
    }
}
```

## YAML 

For YAML support, you must also include the header `<rfl/yaml.hpp>` and include the yaml-cpp library (https://github.com/jbeder/yaml-cpp) in your project.

### Reading and writing

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

### Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::yaml::load<Person>("/path/to/file.yml");

const auto person = Person{...};
rfl::yaml::save("/path/to/file.yml", person);
```

### Reading from and writing into streams

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

### Custom constructors

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

