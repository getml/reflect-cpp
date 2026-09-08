# `rfl::Pattern`

`rfl::Pattern` can be used to verify that a string fits a particular pattern, the latter of which is expressed as a regular expression.

A common problem for backend applications or data pipelines is SQL injection. `rfl::Pattern` can be used to limit the allowed characters
users are able to pass. For instance, for table names, we might only allow uppercase characters and
underscores:

```cpp
using TableName = rfl::Pattern<R"(^[A-Z]+(?:_[A-Z]+)*$)", "TableName">;
```

Because the regex is encoded at compile time, you can be certain that whenever you use the type `TableName`, the string contained
therein will fit the pattern. This is a very strong safeguard against SQL injection.

Note that use also have to give a reasonable name to the pattern. That is, because reflect-cpp emphasizes readable error messages and regex patterns can be somewhat hard to decipher at times.

To retrieve the underlying string, you can use the `.value()` method.

reflect-cpp currently contains the following predefined regex patterns:

- `rfl::AlphaNumeric`
- `rfl::Base64Encoded`
- `rfl::Email`
- `rfl::UUIDv1`
- `rfl::UUIDv2`
- `rfl::UUIDv3`
- `rfl::UUIDv4`
