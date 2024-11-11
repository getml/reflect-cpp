---
hide:
    - navigation
---

![image](/assets/images/rfl-robot.jpg)

# Documentation

## 1) The basics

1.1) [Structs](structs.md) - Recaps the motivating example in the README. A struct is the equivalent of an AND statement or a product type in type theory.

1.2) [Optional fields](optional_fields.md) - For defining fields in your serialized format that might be absent in some cases.

1.3) [Struct flattening](flatten_structs.md) - For making struct A "inherit" the fields of struct B.

1.4) [Processors](processors.md) - For modifying the structs before serialization and deserialization. For instance, processors can be used to transform all field names from `snake_case` to `camelCase`.

1.5) [The rfl::Field-syntax](field_syntax.md) - Describes an alternative syntax which requires slightly more effort, but allows for some powerful functionalities.

1.6) [String literals](literals.md) - For representing strings that can only assume a limited number of enumerated values.

1.7) [Enums](enums.md) - Describes how reflect-cpp handles C++ enums.

1.8) [std::variant and rfl::TaggedUnion](variants_and_tagged_unions.md) - For structs that can be one of several formats. This is the equivalent of an OR statement or a sum type in type theory.

1.9) [rfl::Box and rfl::Ref](rfl_ref.md) - For defining recursive structures.

1.10) [rfl::Timestamp](timestamps.md) - For serializing and deserializing timestamps.

1.11) [rfl::Skip](rfl_skip.md) - For skipping fields during serialization and/or deserialization.

1.12) [rfl::Result](result.md) - For error handling without exceptions.

1.13) [Standard containers](standard_containers.md) - Describes how reflect-cpp treats containers in the standard library.

1.14) [C arrays and inheritance](c_arrays_and_inheritance.md) - Describes how reflect-cpp handles C arrays and inheritance. 

1.15) [rfl::Bytestring](bytestring.md) - Describes how reflect-cpp handles binary strings for formats that support them. 

1.16) [rfl::Binary, rfl::Hex and rfl::Oct](number_systems.md)- For expressing numbers in different formats. 

## 2) Validation

2.1) [Regex patterns](patterns.md) - For requiring that strings follow used-defined regex patterns.

2.2) [Validating numbers](validating_numbers.md) - For imposing constraints on numbers.

2.3) [Composing validators](composing_validators.md) - For defining more complex validators using operators such as `rfl::AllOf` or `rfl::AnyOf`.

2.4) [Size validation](size_validation.md) - For imposing size constraints on containers such as `std::vector` or `std::string`.

2.5) [JSON schema](json_schema.md) - For validating your schema before you even send it to your C++ backend.

## 3) Generic elements

3.1) [rfl::Object](object.md) - A map-like type representing a object with field names that are unknown at compile time.

3.2) [rfl::Generic](generic.md) - A catch-all type that can represent (almost) anything.

3.3) [rfl::ExtraFields](extra_fields.md) - For adding extra fields to your structs, the names of which are unknown at compile time.

## 4) Custom classes

4.1) [Custom classes](custom_classes.md) - For custom classes with private fields.

4.2) [Custom parsers for your classes](custom_parser.md) - For custom classes with private fields that you want to leave absolutely untouched.

## 5) Useful helper functions and classes

5.1) [rfl::replace](replace.md) - For replacing one or several fields in a struct.

5.2) [rfl::as](as.md) - For casting structs as other structs.

5.3) [rfl::NamedTuple](named_tuple.md) - For structural typing.

5.4) [rfl::Tuple](rfl_tuple.md) - An alternative to `std::tuple` that compiles more quickly.

5.5) [rfl::to_view](to_view.md) - For accessing fields of a struct by index or name.

<!-- ## 6) Supported formats

6.1) [JSON](json.md)

6.2) [BSON](bson.md)

6.3) [CBOR](cbor.md)

6.4) [flexbuffers](flexbuffers.md)

6.5) [msgpack](msgpack.md)

6.6) [TOML](toml.md)

6.7) [XML](xml.md)

6.8) [YAML](yaml.md) -->

## 7) Advanced topics

7.1) [Supporting your own format](supporting_your_own_format.md) - For supporting your own serialization and deserialization formats.

7.2) [Maintaining backwards compatability](backwards_compatability.md) - Instructions on how to ensure that your API is backwards-compatible with older versions.
