#ifndef RFL_VALIDATOR_HPP_
#define RFL_VALIDATOR_HPP_

#include <concepts>
#include <functional>
#include <optional>
#include <regex>
#include <string>
#include <type_traits>
#include <utility>

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

  Validator(T&& _value) : value_(V::validate(_value).value()) {}

  Validator(const T& _value) : value_(V::validate(_value).value()) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  Validator(U&& _value)
      : value_(V::validate(T(std::forward<U>(_value))).value()) {}

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
    value_ = V::validate(std::forward<T>(_value)).value();
    return *this;
  }

  /// Assigns the underlying object.
  Validator<T, V>& operator=(const Validator<T, V>& _other) = default;

  /// Assigns the underlying object.
  Validator<T, V>& operator=(Validator<T, V>&& _other) noexcept = default;

  /// Assigns the underlying object.
  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  auto& operator=(U&& _value) noexcept {
    value_ = V::validate(T(std::forward<U>(_value))).value();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  auto& operator=(const U& _value) {
    value_ = V::validate(T(_value)).value();
    return *this;
  }

  /// Equality operator other Validators.
  bool operator==(const Validator<T, V>& _other) const {
    return value() == _other.value();
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

template <class T, class V>
inline auto operator<=>(const Validator<T, V>& _v1,
                        const Validator<T, V>& _v2) {
  return _v1.value() <=> _v2.value();
}

template <class T, class V>
inline auto operator<=>(const Validator<T, V>& _v, const T& _t) {
  return _v.value() <=> _t;
}

}  // namespace rfl

namespace std {

template <class T, class V>
struct hash<rfl::Validator<T, V>> {
  size_t operator()(const rfl::Validator<T, V>& _v) const {
    return hash<T>()(_v.value());
  }
};

}  // namespace std

#endif
