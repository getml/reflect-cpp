#ifndef RFL_POSITIONAL_HPP_
#define RFL_POSITIONAL_HPP_

#include <type_traits>
#include <utility>

#include "default.hpp"

namespace rfl {

/// Marks a field as positional for CLI argument parsing.
/// For non-CLI formats (JSON, YAML, etc.), this is transparent.
template <class T>
struct Positional {
  /// The underlying type.
  using Type = T;

  Positional() : value_(Type()) {}

  Positional(const Type& _value) : value_(_value) {}

  Positional(Type&& _value) noexcept : value_(std::move(_value)) {}

  Positional(Positional<T>&& _field) noexcept = default;

  Positional(const Positional<T>& _field) = default;

  template <class U>
  Positional(const Positional<U>& _field) : value_(_field.get()) {}

  template <class U>
  Positional(Positional<U>&& _field) : value_(std::move(_field.value_)) {}

  template <class U>
    requires std::is_convertible_v<U, Type>
  Positional(const U& _value) : value_(_value) {}

  template <class U>
    requires std::is_convertible_v<U, Type>
  Positional(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  /// Assigns the underlying object to its default value.
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  Positional(const Default&) : value_(Type()) {}

  ~Positional() = default;

  /// Returns the underlying object.
  const Type& get() const { return value_; }

  /// Returns the underlying object.
  Type& operator()() { return value_; }

  /// Returns the underlying object.
  const Type& operator()() const { return value_; }

  /// Assigns the underlying object.
  auto& operator=(const Type& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the underlying object.
  auto& operator=(Type&& _value) noexcept {
    value_ = std::move(_value);
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
    requires std::is_convertible_v<U, Type>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the underlying object to its default value.
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  auto& operator=(const Default&) {
    value_ = Type();
    return *this;
  }

  /// Assigns the underlying object.
  Positional<T>& operator=(const Positional<T>& _field) = default;

  /// Assigns the underlying object.
  Positional<T>& operator=(Positional<T>&& _field) = default;

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const Positional<U>& _field) {
    value_ = _field.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(Positional<U>&& _field) {
    value_ = std::move(_field.value_);
    return *this;
  }

  /// Assigns the underlying object.
  void set(const Type& _value) { value_ = _value; }

  /// Assigns the underlying object.
  void set(Type&& _value) { value_ = std::move(_value); }

  /// Returns the underlying object.
  Type& value() { return value_; }

  /// Returns the underlying object.
  const Type& value() const { return value_; }

  /// The underlying value.
  Type value_;
};

}  // namespace rfl

#endif
