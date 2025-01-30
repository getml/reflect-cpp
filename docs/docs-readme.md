# Documentation

## The basics

[Structs](concepts/structs.md) - Recaps the motivating example in the README. A struct is the equivalent of an AND statement or a product type in type theory.

[Optional fields](optional_fields.md) - For defining fields in your serialized format that might be absent in some cases.

[Struct flattening](flatten_structs.md) - For making struct A "inherit" the fields of struct B.

[Processors](concepts/processors.md) - For modifying the structs before serialization and deserialization. For instance, processors can be used to transform all field names from `snake_case` to `camelCase`.

[The rfl::Field-syntax](concepts/field_syntax.md) - Describes an alternative syntax which requires slightly more effort, but allows for some powerful functionalities.

[String literals](literals.md) - For representing strings that can only assume a limited number of enumerated values.

[Enums](enums.md) - Describes how reflect-cpp handles C++ enums.

[std::variant and rfl::TaggedUnion](variants_and_tagged_unions.md) - For structs that can be one of several formats. This is the equivalent of an OR statement or a sum type in type theory.

[rfl::Box and rfl::Ref](rfl_ref.md) - For defining recursive structures.

[rfl::Timestamp](timestamps.md) - For serializing and deserializing timestamps.

[rfl::Skip](rfl_skip.md) - For skipping fields during serialization and/or deserialization.

[rfl::Result](result.md) - For error handling without exceptions.

[Standard containers](standard_containers.md) - Describes how reflect-cpp treats containers in the standard library.

[C arrays and inheritance](c_arrays_and_inheritance.md) - Describes how reflect-cpp handles C arrays and inheritance. 

[rfl::Bytestring](bytestring.md) - Describes how reflect-cpp handles binary strings for formats that support them. 

[rfl::Binary, rfl::Hex and rfl::Oct](number_systems.md)- For expressing numbers in different formats. 

## Validation

[Regex patterns](patterns.md) - For requiring that strings follow used-defined regex patterns.

[Validating numbers](validating_numbers.md) - For imposing constraints on numbers.

[Composing validators](composing_validators.md) - For defining more complex validators using operators such as `rfl::AllOf` or `rfl::AnyOf`.

[Size validation](size_validation.md) - For imposing size constraints on containers such as `std::vector` or `std::string`.

[JSON schema](json_schema.md) - For validating your schema before you even send it to your C++ backend.

## Generic elements

[rfl::Object](object.md) - A map-like type representing a object with field names that are unknown at compile time.

[rfl::Generic](generic.md) - A catch-all type that can represent (almost) anything.

[rfl::ExtraFields](extra_fields.md) - For adding extra fields to your structs, the names of which are unknown at compile time.

## Custom classes

[Custom classes](concepts/custom_classes.md) - For custom classes with private fields.

[Custom parsers for your classes](custom_parser.md) - For custom classes with private fields that you want to leave absolutely untouched.

## Useful helper functions and classes

[rfl::replace](replace.md) - For replacing one or several fields in a struct.

[rfl::as](as.md) - For casting structs as other structs.

[rfl::NamedTuple](named_tuple.md) - For structural typing.

[rfl::Tuple](rfl_tuple.md) - An alternative to `std::tuple` that compiles more quickly.

[rfl::to_view](to_view.md) - For accessing fields of a struct by index or name.

## Advanced topics

[Supporting your own format](supported_formats/supporting_your_own_format.md) - For supporting your own serialization and deserialization formats.

[Maintaining backwards compatability](backwards_compatability.md) - Instructions on how to ensure that your API is backwards-compatible with older versions.

[Benchmarks](benchmarks.md) - Extensive benchmarks of popular libraries for serialization, deserialization.

<br>
