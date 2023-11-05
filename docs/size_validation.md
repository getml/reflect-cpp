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
