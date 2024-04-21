# Documentation

![image](banner2.png)

## 1) The basics

1.1) [Structs](https://github.com/getml/reflect-cpp/blob/main/docs/structs.md) - Recaps the motivating example in the README. A struct is the equivalent of an AND statement or a product type in type theory.

1.2) [Optional fields](https://github.com/getml/reflect-cpp/blob/main/docs/optional_fields.md) - For defining fields in your serialized format that might be absent in some cases.

1.3) [Struct flattening](https://github.com/getml/reflect-cpp/blob/main/docs/flatten_structs.md) - For making struct A "inherit" the fields of struct B.

1.4) [The rfl::Field-syntax](https://github.com/getml/reflect-cpp/blob/main/docs/field_syntax.md) - Describes an alternative syntax which requires slightly more effort, but allows for some powerful functionalities.

1.5) [String literals](https://github.com/getml/reflect-cpp/blob/main/docs/literals.md) - For representing strings that can only assume a limited number of enumerated values.

1.6) [Enums](https://github.com/getml/reflect-cpp/blob/main/docs/enums.md) - Describes how reflect-cpp handles C++ enums.

1.7) [std::variant and rfl::TaggedUnion](https://github.com/getml/reflect-cpp/blob/main/docs/variants_and_tagged_unions.md) - For structs that can be one of several formats. This is the equivalent of an OR statement or a sum type in type theory.

1.8) [rfl::Box and rfl::Ref](https://github.com/getml/reflect-cpp/blob/main/docs/rfl_ref.md) - For defining recursive structures.

1.9) [rfl::Timestamp](https://github.com/getml/reflect-cpp/blob/main/docs/timestamps.md) - For serializing and deserializing timestamps.

1.10) [rfl::Result](https://github.com/getml/reflect-cpp/blob/main/docs/result.md) - For error handling without exceptions.

1.11) [Standard containers](https://github.com/getml/reflect-cpp/blob/main/docs/standard_containers.md) - Describes how reflect-cpp treats containers in the standard library.

## 2) Validation

2.1) [Regex patterns](https://github.com/getml/reflect-cpp/blob/main/docs/patterns.md) - For requiring that strings follow used-defined regex patterns.

2.2) [Validating numbers](https://github.com/getml/reflect-cpp/blob/main/docs/validating_numbers.md) - For imposing constraints on numbers.

2.3) [Composing validators](https://github.com/getml/reflect-cpp/blob/main/docs/composing_validators.md) - For defining more complex validators using operators such as `rfl::AllOf` or `rfl::AnyOf`.

2.4) [Size validation](https://github.com/getml/reflect-cpp/blob/main/docs/size_validation.md) - For imposing size constraints on containers such as `std::vector` or `std::string`.

2.5) [JSON schema](https://github.com/getml/reflect-cpp/blob/main/docs/json_schema.md) - For validating your schema before you even send it to your C++ backend.

## 3) Custom classes

3.1) [Custom classes](https://github.com/getml/reflect-cpp/blob/main/docs/custom_classes.md) - For custom classes with private fields.

3.2) [Custom parsers for your classes](https://github.com/getml/reflect-cpp/blob/main/docs/custom_parser.md) - For custom classes with private fields that you want to leave absolutely untouched.

## 4) Useful helper functions and classes

4.1) [rfl::replace](https://github.com/getml/reflect-cpp/blob/main/docs/replace.md) - For replacing one or several fields in a struct.

4.2) [rfl::as](https://github.com/getml/reflect-cpp/blob/main/docs/as.md) - For casting structs as other structs.

4.3) [rfl::NamedTuple](https://github.com/getml/reflect-cpp/blob/main/docs/named_tuple.md) - For structural typing.

4.4) [rfl::to_view](https://github.com/getml/reflect-cpp/blob/main/docs/to_view.md) - For accessing fields of a struct by index or name.

## 5) Supported formats

5.1) [JSON](https://github.com/getml/reflect-cpp/blob/main/docs/json.md)

5.2) [BSON](https://github.com/getml/reflect-cpp/blob/main/docs/bson.md)

5.3) [CBOR](https://github.com/getml/reflect-cpp/blob/main/docs/cbor.md)

5.4) [flexbuffers](https://github.com/getml/reflect-cpp/blob/main/docs/flexbuffers.md)

5.5) [msgpack](https://github.com/getml/reflect-cpp/blob/main/docs/msgpack.md)

5.6) [TOML](https://github.com/getml/reflect-cpp/blob/main/docs/toml.md)

5.7) [XML](https://github.com/getml/reflect-cpp/blob/main/docs/xml.md)

5.8) [YAML](https://github.com/getml/reflect-cpp/blob/main/docs/yaml.md)

## 6) Advanced topics

6.1) [Supporting your own format](https://github.com/getml/reflect-cpp/blob/main/docs/supporting_your_own_format.md) - For supporting your own serialization and deserialization formats.

6.2) [Maintaining backwards compatability](https://github.com/getml/reflect-cpp/blob/main/docs/backwards_compatability.md) - Instructions on how to ensure that your API is backwards-compatible with older versions.
