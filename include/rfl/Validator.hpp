#ifndef RFL_VALIDATOR_HPP_
#define RFL_VALIDATOR_HPP_

#include <concepts>
#include <optional>
#include <regex>
#include <string>
#include <type_traits>

#include "internal/HasValidation.hpp"
#include "internal/StringLiteral.hpp"
#include "rfl/Result.hpp"

namespace rfl {

template <class T, class V>
requires internal::HasValidation<V, T>
struct Validator {
 public:
  using ReflectionType = T;

  /// Exception-free validation.
  static Result<Validator<T, V>> from_value(const T& _value) noexcept {
    try {
      return Validator<T, V>(_value);
    } catch (std::exception& e) {
      return Error(e.what());
    }
  }

  Validator() : value_(V::validate(T()).value()) {}

  Validator(Validator<T, V>&& _other) noexcept = default;

  Validator(const Validator<T, V>& _other) = default;

  Validator(const T& _value) : value_(V::validate(_value).value()) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  Validator(const U& _value) : value_(V::validate(T(_value)).value()) {}

  ~Validator() = default;

  /// Assigns the underlying object.
  auto& operator=(const T& _value) {
    value_ = V::validate(_value).value();
    return *this;
  }

  /// Assigns the underlying object.
  auto& operator=(T&& _value) {
    value_ = V::validate(_value).value();
    return *this;
  }

  /// Assigns the underlying object.
  Validator<T, V>& operator=(const Validator<T, V>& _other) = default;

  /// Assigns the underlying object.
  Validator<T, V>& operator=(Validator<T, V>&& _other) = default;

  /// Assigns the underlying object.
  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  auto& operator=(U&& _value) {
    value_ = V::validate(T(std::forward<T>(_value))).value();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  auto& operator=(const U& _value) {
    value_ = V::validate(T(_value)).value();
    return *this;
  }

  /// Exposes the underlying value.
  T& value() { return value_; }

  /// Exposes the underlying value.
  const T& value() const { return value_; }

  /// Necessary for the serialization to work.
  const T& reflection() const { return value_; }

 private:
  /// The underlying value.
  T value_;
};

}  // namespace rfl

#endif
