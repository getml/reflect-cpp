#ifndef RFL_SHORT_HPP_
#define RFL_SHORT_HPP_

#include <type_traits>
#include <utility>

#include "default.hpp"
#include "internal/StringLiteral.hpp"

namespace rfl {

/// Assigns a short CLI alias (e.g. "-p") to a field.
/// For non-CLI formats (JSON, YAML, etc.), this is transparent.
template <internal::StringLiteral _name, class T>
struct Short {
  /// The underlying type.
  using Type = T;

  /// The short name.
  static constexpr auto short_name_ = _name;

  static_assert(_name.length == 1, "Short name must be exactly one character.");

  Short() requires std::is_default_constructible_v<Type>
      : value_(Type()) {}

  Short(const Type& _value) : value_(_value) {}

  Short(Type&& _value) noexcept : value_(std::move(_value)) {}

  Short(Short<_name, T>&& _field) noexcept = default;

  Short(const Short<_name, T>& _field) = default;

  template <class U>
  Short(const Short<_name, U>& _field) : value_(_field.get()) {}

  template <class U>
  Short(Short<_name, U>&& _field) : value_(std::move(_field.value_)) {}

  template <class U>
    requires std::is_convertible_v<U, Type>
  Short(const U& _value) : value_(_value) {}

  template <class U>
    requires std::is_convertible_v<U, Type>
  Short(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  /// Assigns the underlying object to its default value.
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  Short(const Default&) : value_(Type()) {}

  ~Short() = default;

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
  Short<_name, T>& operator=(const Short<_name, T>& _field) = default;

  /// Assigns the underlying object.
  Short<_name, T>& operator=(Short<_name, T>&& _field) = default;

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const Short<_name, U>& _field) {
    value_ = _field.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(Short<_name, U>&& _field) {
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
