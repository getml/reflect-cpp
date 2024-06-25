
## Validating numbers

reflect-cpp can also be used to impose condition on numbers. For instance, 
if you want an integer that is greater or equal to 10, you can do the following:

```cpp
using IntGreaterThan10 = rfl::Validator<int, rfl::Minimum<10>>;
```

When you then use the type `IntGreaterThan10` inside you `rfl::Field`, the condition will be automatically
validated.

The underlying value can be retrieved using the `.value()` method.

The current conditions are currently supported by reflect-cpp:

- `rfl::EqualTo`
- `rfl::NotEqualTo`
- `rfl::Minimum`
- `rfl::ExclusiveMinimum`
- `rfl::Maximum`
- `rfl::ExclusiveMaximum`

## Pattern matching 

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

## Composing validators

Sometimes the conditions you want to impose are more complex than a simple minimum or a simple regex pattern. That is why reflect-cpp allows you to compose them using the operators `rfl::AnyOf`, `rfl::AllOf` and `rfl::OneOf`.

- `rfl::AnyOf` requires that *at least one* of the conditions contained therein be true. It thus works like an OR operator.
- `rfl::AllOf` requires that *all* of the conditions contained therein be true. It thus works like an AND operator.
- `rfl::OneOf` requires that *exactly one* of the conditions contained therein be true. It thus works like a XOR operator.

For instance, if you want to ensure that a person's age is a reasonable number, you can impose two conditions as follows:

```cpp
using Age = rfl::Validator<int,
                           rfl::AllOf<rfl::Minimum<0>, rfl::Maximum<130>>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  Age age;
};
```

However, if you pass more than one constraint to `rfl::Validator`, they will be automatically connected using `rfl::AllOf`,
so you might as well write:

```cpp
using Age = rfl::Validator<int, rfl::Minimum<0>, rfl::Maximum<130>>;

...
```

You can also nest composed validators. For instance, in this case the age must either be 0-10 or 40-130.

```cpp
using Age = rfl::Validator<
    int,
    rfl::AnyOf<rfl::AllOf<rfl::Minimum<0>, rfl::Maximum<10>>,
               rfl::AllOf<rfl::Minimum<40>, rfl::Maximum<130>>>>;
```

You can also compose regex patterns:

```cpp
using TableOrColumnName = rfl::Validator<
    std::string,
    rfl::AnyOf<rfl::PatternValidator<R"(^[A-Z]+(?:_[A-Z]+)*$)", "TableName">,
               rfl::PatternValidator<R"(^[a-z]+(?:_[a-z]+)*$)", "ColumnName">>>;
```
# `rfl::Size`

`rfl::Size` can be used to impose constraints on any container that contains the method `.size()`, which includes `std::string`.

For instance, you can a require a vector to be non-empty as follows:

```cpp
using NonEmptyStringVector =
    rfl::Validator<std::vector<std::string>, rfl::Size<rfl::Minimum<1>>>;
```

Or you could require it to contain exactly zero or three elements, using validator composition:

```cpp
using NoneOrThree =
    rfl::Validator<std::vector<std::string>,
                   rfl::Size<rfl::AnyOf<rfl::EqualTo<0>, rfl::EqualTo<3>>>>;
```

