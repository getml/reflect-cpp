# Documentation

![image](banner2.png)

## 1) The basics

1.1) [Structs](https://github.com/getml/reflect-cpp/blob/main/docs/structs.md) - Recaps the motivating example in the README. A struct is the equivalent of an AND statement or a product type in type theory.

1.2) [Optional fields](https://github.com/getml/reflect-cpp/blob/main/docs/optional_fields.md) - For when you have fields in your serialized format that might be absent in some cases.

1.3) [Struct flattening](https://github.com/getml/reflect-cpp/blob/main/docs/flatten_structs.md) - For when you want struct A to "inherit" the fields of struct B.

1.4) [String literals](https://github.com/getml/reflect-cpp/blob/main/docs/literals.md) - For when you have strings that can only assume a limited number of enumerated values.

1.5) [std::variant and rfl::TaggedUnion](https://github.com/getml/reflect-cpp/blob/main/docs/variants_and_tagged_unions.md) - For when your structs can be one of several formats. This is the equivalent of an OR statement or a sum type in type theory.

## 2) Custom classes

2.1) [Custom classes](https://github.com/getml/reflect-cpp/blob/main/docs/custom_classes.md) - For when you have custom classes with private fields.

2.2) [Custom parsers for your classes](https://github.com/getml/reflect-cpp/blob/main/docs/custom_parser.md) - For when you have custom classes with private fields and you want to leave them absolutely untouched.
