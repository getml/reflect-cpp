# Documentation

## The basics

{{< cards >}}
  {{< card link="install" title="Installation" icon="book-open" subtitle="How to install reflect-cpp using vcpkg, conan, cmake or direct inclusion.">}}
  {{< card link="supported_formats" title="Supported formats" icon="book-open" subtitle="What formats are supported by reflect-cpp, such as JSON, XML, msgpack, etc.">}}
  {{< card link="structs" title="Structs" icon="book-open" subtitle="Recaps the motivating example in the README.">}}
  {{< card link="optional_fields" title="Optional fields" icon="book-open" subtitle="For defining fields that might be absent in your serialized format.">}}
  {{< card link="flatten_structs" title="Struct flattening" icon="book-open" subtitle="For making struct A inherit the fields of struct B.">}}
  {{< card link="processors" title="Processors" icon="book-open" subtitle="For modifying structs before serialization and deserialization.">}}
  {{< card link="field_syntax" title="The rfl::Field-syntax" icon="book-open" subtitle="An alternative syntax that allows for powerful functionalities.">}}
  {{< card link="literals" title="String literals" icon="book-open" subtitle="For representing strings that can only assume a limited number of enumerated values.">}}
  {{< card link="enums" title="Enums" icon="book-open" subtitle="How reflect-cpp handles C++ enums.">}}
  {{< card link="variants_and_tagged_unions" title="std::variant and rfl::TaggedUnion" icon="book-open" subtitle="For structs that can be one of several formats.">}}
  {{< card link="rfl_ref" title="rfl::Box and rfl::Ref" icon="book-open" subtitle="For defining recursive structures.">}}
  {{< card link="timestamps" title="rfl::Timestamp and std::chrono::duration" icon="book-open" subtitle="For serializing and deserializing timestamps and durations.">}}
  {{< card link="rfl_skip" title="rfl::Skip" icon="book-open" subtitle="For skipping fields during serialization and/or deserialization.">}}
  {{< card link="commented" title="rfl::Commented" icon="book-open" subtitle="For adding comments to your serialized format.">}}
  {{< card link="result" title="rfl::Result" icon="book-open" subtitle="For error handling without exceptions.">}}
  {{< card link="standard_containers" title="Standard containers" icon="book-open" subtitle="How reflect-cpp treats containers in the standard library.">}}
  {{< card link="expected" title="std::expected" icon="book-open" subtitle="For serializing and deserializing std::expected, the C++-23 result type.">}}
  {{< card link="c_arrays_and_inheritance" title="C arrays and inheritance" icon="book-open" subtitle="How reflect-cpp handles C arrays and inheritance.">}}
  {{< card link="bytestring" title="rfl::Bytestring" icon="book-open" subtitle="How reflect-cpp handles binary strings for formats that support them.">}}
  {{< card link="number_systems" title="rfl::Binary, rfl::Hex and rfl::Oct" icon="book-open" subtitle="For expressing numbers in different formats.">}}
  {{< card link="default_val" title="Default values" icon="book-open" subtitle="For defining default values for fields that might be absent.">}}
  {{< card link="atomic" title="Atomic types" icon="book-open" subtitle="For serializing and deserializing atomic types.">}}
{{< /cards >}}

## Validation

{{< cards >}}
  {{< card link="patterns" title="Regex patterns" icon="book-open" subtitle="For requiring that strings follow user-defined regex patterns.">}}
  {{< card link="validating_numbers" title="Validating numbers" icon="book-open" subtitle="For imposing constraints on numbers.">}}
  {{< card link="composing_validators" title="Composing validators" icon="book-open" subtitle="For defining more complex validators using operators.">}}
  {{< card link="size_validation" title="Size validation" icon="book-open" subtitle="For imposing size constraints on containers.">}}
  {{< card link="json_schema" title="JSON schema" icon="book-open" subtitle="For validating your schema before you send it to your C++ backend.">}}
{{< /cards >}}

## Generic elements

{{< cards >}}
  {{< card link="object" title="rfl::Object" icon="book-open" subtitle="A map-like type representing an object with unknown field names.">}}
  {{< card link="generic" title="rfl::Generic" icon="book-open" subtitle="A catch-all type that can represent almost anything.">}}
  {{< card link="extra_fields" title="rfl::ExtraFields" icon="book-open" subtitle="For adding extra fields with unknown names.">}}
{{< /cards >}}

## Custom classes

{{< cards >}}
  {{< card link="custom_classes" title="Custom classes" icon="book-open" subtitle="For custom classes with private fields.">}}
  {{< card link="custom_parser" title="Custom parsers for your classes" icon="book-open" subtitle="For custom classes that you want to leave untouched.">}}
{{< /cards >}}

## Useful helper functions and classes

{{< cards >}}
  {{< card link="replace" title="rfl::replace" icon="book-open" subtitle="For replacing one or several fields in a struct.">}}
  {{< card link="as" title="rfl::as" icon="book-open" subtitle="For casting structs as other structs.">}}
  {{< card link="named_tuple" title="rfl::NamedTuple" icon="book-open" subtitle="For structural typing.">}}
  {{< card link="rfl_tuple" title="rfl::Tuple" icon="book-open" subtitle="An alternative to std::tuple that compiles more quickly.">}}
  {{< card link="to_view" title="rfl::to_view" icon="book-open" subtitle="For accessing fields of a struct by index or name.">}}
{{< /cards >}}

## Advanced topics

{{< cards >}}
  {{< card link="supported_formats/supporting_your_own_format" title="Supporting your own format" icon="book-open" subtitle="For supporting your own serialization and deserialization formats.">}}
  {{< card link="backwards_compatability" title="Maintaining backwards compatibility" icon="book-open" subtitle="Instructions for ensuring backwards-compatible APIs.">}}
  {{< card link="benchmarks" title="Benchmarks" icon="book-open" subtitle="Extensive benchmarks of popular serialization libraries.">}}
{{< /cards >}}

## Contributing

{{< cards >}}
  {{< card link="contributing" title="How to contribute" icon="book-open" subtitle="How to contribute to reflect-cpp.">}}
  {{< card link="build_docs" title="Build the documentation" icon="book-open" subtitle="How to build the documentation locally.">}}
{{< /cards >}}

