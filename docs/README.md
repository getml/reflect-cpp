# Documentation

![image](banner2.png)

## 1) The basics

1.1) [Structs](https://github.com/getml/reflect-cpp/blob/main/docs/structs.md) - Recaps the motivating example in the README. A struct is the equivalent of an AND statement or a product type in type theory.

1.2) [Optional fields](https://github.com/getml/reflect-cpp/blob/main/docs/optional_fields.md) - For defining fields in your serialized format that might be absent in some cases.

1.3) [Struct flattening](https://github.com/getml/reflect-cpp/blob/main/docs/flatten_structs.md) - For making struct A "inherit" the fields of struct B.

1.4) [String literals](https://github.com/getml/reflect-cpp/blob/main/docs/literals.md) - For representing strings that can only assume a limited number of enumerated values.

1.5) [std::variant and rfl::TaggedUnion](https://github.com/getml/reflect-cpp/blob/main/docs/variants_and_tagged_unions.md) - For structs that can be one of several formats. This is the equivalent of an OR statement or a sum type in type theory.

1.6) [rfl::Box and rfl::Ref](https://github.com/getml/reflect-cpp/blob/main/docs/rfl_ref.md) - For defining recursive structures.

1.7) [rfl::Timestamp](https://github.com/getml/reflect-cpp/blob/main/docs/timestamps.md) - For serializing and deserializing timestamps.

1.8) [Standard containers](https://github.com/getml/reflect-cpp/blob/main/docs/standard_containers.md) - Describes how reflect-cpp treats containers in the standard library.

## 2) Custom classes

2.1) [Custom classes](https://github.com/getml/reflect-cpp/blob/main/docs/custom_classes.md) - For custom classes with private fields.

2.2) [Custom parsers for your classes](https://github.com/getml/reflect-cpp/blob/main/docs/custom_parser.md) - For custom classes with private fields that you want to leave absolutely untouched.

## 3) Useful helper functions and classes

3.1) [rfl::replace](https://github.com/getml/reflect-cpp/blob/main/docs/replace.md) - For replacing one or several fields in a struct.

3.2) [rfl::as](https://github.com/getml/reflect-cpp/blob/main/docs/as.md) - For casting structs as other structs.

3.3) [rfl::NamedTuple](https://github.com/getml/reflect-cpp/blob/main/docs/named_tuple.md) - For structural typing
