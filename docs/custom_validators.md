# Custom validators

reflect-cpp provides a powerful validation system through `rfl::Validator`. While the library includes many built-in validators (like `rfl::Minimum`, `rfl::Maximum`, `rfl::Pattern`, etc.), you can easily create your own custom validators to enforce domain-specific constraints.

## Requirements

A custom validator is a struct that must fulfill the following requirements:

1. It must contain a static method called `validate` that takes a value and returns `rfl::Result<T>`. This method should be `noexcept`.
2. (Optional) It should contain a static template method called `to_schema` that returns `parsing::schema::ValidationType`. This is used for JSON schema generation.

## Basic example

Let's create a simple validator that ensures an integer is even:

```cpp
struct EvenNumber {
  template <class T>
  static rfl::Result<T> validate(T _value) noexcept {
    if (_value % 2 != 0) {
      return rfl::error("Value expected to be even, but got " + 
                        std::to_string(_value) + ".");
    }
    return _value;
  }
};
```

You can then use it like this:

```cpp
using EvenInt = rfl::Validator<int, EvenNumber>;

struct MyStruct {
  EvenInt some_value;
};

// This will succeed
const auto obj1 = rfl::json::read<MyStruct>(R"({"some_value": 42})");

// This will fail with a validation error
const auto obj2 = rfl::json::read<MyStruct>(R"({"some_value": 43})");
```

## Validators with compile-time parameters

You can create validators that accept compile-time parameters using non-type template parameters. Here's an example that validates divisibility:

```cpp
template <auto _divisor>
struct DivisibleBy {
  template <class T>
  static rfl::Result<T> validate(T _value) noexcept {
    constexpr auto divisor = static_cast<T>(_divisor);
    if (_value % divisor != 0) {
      std::stringstream stream;
      stream << "Value expected to be divisible by " << divisor 
             << ", but got " << _value << ".";
      return rfl::Error(stream.str());
    }
    return _value;
  }
};

// Usage
using DivisibleBy5 = rfl::Validator<int, DivisibleBy<5>>;
```

## String validators with regex

For validating strings with regular expressions, you can use `rfl::internal::StringLiteral` to encode the regex at compile time. Here's how `rfl::PatternValidator` is implemented:

```cpp
template <internal::StringLiteral _regex, internal::StringLiteral _name>
struct PatternValidator {
  using Name = Literal<_name>;
  using Regex = Literal<_regex>;

  static Result<std::string> validate(const std::string& _str) noexcept {
    if (ctre::match<ctll::fixed_string<_regex.length>{
            ctll::construct_from_pointer, _regex.arr_.data()}>(_str)) {
      return _str;
    } else {
      std::stringstream stream;
      stream << "String '" << _str << "' did not match format '" << _name.str()
             << "': '" << _regex.str() << "'.";
      return error(stream.str());
    }
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    return ValidationType{ValidationType::Regex{.pattern_ = Regex().str()}};
  }
};
```

You can use this to create custom pattern types:

```cpp
using PhoneNumber = rfl::Pattern<R"(^\+?[1-9]\d{1,14}$)", "PhoneNumber">;
using ZipCode = rfl::Pattern<R"(^\d{5}(-\d{4})?$)", "ZipCode">;
```

## Container validators

You can create validators that work on containers by accessing their properties. Here's how `rfl::Size` is implemented:

```cpp
template <class V>
struct Size {
  template <class T>
  static rfl::Result<T> validate(const T& _t) {
    const auto to_t = [&](const auto&) { return _t; };
    const auto embellish_error = [](const auto& _err) -> Error {
      return Error("Size validation failed: " + _err.what());
    };
    return V::validate(_t.size()).transform(to_t).transform_error(
        embellish_error);
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    return ValidationType{ValidationType::Size{
        .size_limit_ =
            rfl::Ref<ValidationType>::make(V::template to_schema<size_t>())}};
  }
};
```

This validator wraps another validator `V` and applies it to the size of the container:

```cpp
using NonEmptyVector = rfl::Validator<std::vector<int>, 
                                       rfl::Size<rfl::Minimum<1>>>;

using PasswordWithMinLength = rfl::Validator<std::string, 
                                              rfl::Size<rfl::Minimum<8>>>;
```

## Composing validators

You can compose multiple validators together using `rfl::AllOf`, `rfl::AnyOf`, or `rfl::OneOf`:

```cpp
// Must satisfy all validators
using ValidPort = rfl::Validator<int, 
                                  rfl::AllOf<rfl::Minimum<1>, 
                                             rfl::Maximum<65535>>>;

// Must satisfy at least one validator
using SmallOrLarge = rfl::Validator<int, 
                                     rfl::AnyOf<rfl::Maximum<10>, 
                                                rfl::Minimum<100>>>;
```

You can also compose multiple validators directly in `rfl::Validator`:

```cpp
using ValidPort = rfl::Validator<int, rfl::Minimum<1>, rfl::Maximum<65535>>;
```

## Accessing validated values

The underlying value can be retrieved using any of these methods:

- `.get()` — returns a reference to the underlying value (const and non-const overloads)
- `.value()` — returns a reference to the underlying value (const and non-const overloads)
- `operator*()` — dereferences to the underlying value
- `operator()()` — returns a reference to the underlying value

Example:

```cpp
using PositiveInt = rfl::Validator<int, rfl::Minimum<1>>;

PositiveInt x = 42;
int value1 = x.get();
int value2 = x.value();
int value3 = *x;
int value4 = x();
```

## Exception-free construction

If you want to construct a validator without throwing an exception, use the `from_value` static method:

```cpp
using PositiveInt = rfl::Validator<int, rfl::Minimum<1>>;

auto result = PositiveInt::from_value(42);
if (result) {
  PositiveInt valid_value = std::move(result.value());
} else {
  std::cout << "Validation error: " << result.error()->what() << std::endl;
}
```

## Advanced example: Range validator

Here's a complete example of a custom range validator:

```cpp
template <auto _min, auto _max>
struct InRange {
  template <class T>
  static rfl::Result<T> validate(T _value) noexcept {
    constexpr auto min_val = static_cast<T>(_min);
    constexpr auto max_val = static_cast<T>(_max);
    
    if (_value < min_val || _value > max_val) {
      std::stringstream stream;
      stream << "Value expected to be in range [" << min_val << ", " 
             << max_val << "], but got " << _value << ".";
      return rfl::Error(stream.str());
    }
    return _value;
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    
    const auto min_variant = 
        std::is_floating_point_v<T>
            ? rfl::Variant<double, int>(static_cast<double>(_min))
            : rfl::Variant<double, int>(static_cast<int>(_min));
    
    const auto max_variant = 
        std::is_floating_point_v<T>
            ? rfl::Variant<double, int>(static_cast<double>(_max))
            : rfl::Variant<double, int>(static_cast<int>(_max));
    
    return ValidationType{ValidationType::AllOf{
        .validators_ = {
            rfl::Ref<ValidationType>::make(
                ValidationType{ValidationType::Minimum{.value_ = min_variant}}),
            rfl::Ref<ValidationType>::make(
                ValidationType{ValidationType::Maximum{.value_ = max_variant}})
        }}};
  }
};

// Usage
using Percentage = rfl::Validator<double, InRange<0, 100>>;
using ValidPort = rfl::Validator<int, InRange<1, 65535>>;
```

## Best practices

1. **Always make `validate` noexcept**: Return `rfl::Error` instead of throwing exceptions.
2. **Provide clear error messages**: Users should understand why validation failed.
3. **Implement `to_schema` when possible**: This enables JSON schema generation for your validators.
4. **Use compile-time parameters**: Prefer template parameters over runtime configuration for better type safety.
5. **Test edge cases**: Ensure your validator handles boundary conditions correctly.
6. **Compose existing validators**: Before writing a new validator, check if you can compose existing ones.
