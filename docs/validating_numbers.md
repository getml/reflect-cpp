# Validating numbers

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
