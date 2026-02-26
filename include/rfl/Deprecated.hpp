#ifndef RFL_DEPRECATED_HPP_
#define RFL_DEPRECATED_HPP_

#include <algorithm>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "Literal.hpp"
#include "default.hpp"
#include "internal/StringLiteral.hpp"

namespace rfl {

/// Used to mark a field as deprecated in the JSON schema, replacing
/// rfl::Description for deprecated fields. Includes both a deprecation message
/// and a description:
///   rfl::Deprecated<"use X instead", "description of field", T>
template <internal::StringLiteral _deprecation_message,
          internal::StringLiteral _description, class T>
struct Deprecated {
  /// The underlying type.
  using Type = T;

  /// The deprecation message.
  using DeprecationMessage = rfl::Literal<_deprecation_message>;

  /// The description of the field (same role as Description::Content).
  using Content = rfl::Literal<_description>;

  using ReflectionType = Type;

  Deprecated() : value_(Type()) {}

  Deprecated(const Type& _value) : value_(_value) {}

  Deprecated(Type&& _value) noexcept : value_(std::move(_value)) {}

  Deprecated(Deprecated&& _field) noexcept = default;

  Deprecated(const Deprecated& _field) = default;

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Deprecated(const Deprecated<_deprecation_message, _description, U>& _field)
      : value_(_field.get()) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Deprecated(Deprecated<_deprecation_message, _description, U>&& _field)
      : value_(std::move(_field.value_)) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Deprecated(const U& _value) : value_(_value) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Deprecated(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  /// Assigns the underlying object to its default value.
  template <class U = Type,
            typename std::enable_if<std::is_default_constructible_v<U>,
                                    bool>::type = true>
  Deprecated(const Default&) : value_(Type()) {}

  ~Deprecated() = default;

  /// The deprecation message, for internal use.
  constexpr static const internal::StringLiteral deprecation_message_ =
      _deprecation_message;

  /// The description, for internal use.
  constexpr static const internal::StringLiteral description_ = _description;

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
  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the underlying object to its default value.
  template <class U = Type,
            typename std::enable_if<std::is_default_constructible_v<U>,
                                    bool>::type = true>
  auto& operator=(const Default&) {
    value_ = Type();
    return *this;
  }

  /// Assigns the underlying object.
  Deprecated& operator=(const Deprecated& _field) = default;

  /// Assigns the underlying object.
  Deprecated& operator=(Deprecated&& _field) = default;

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(
      const Deprecated<_deprecation_message, _description, U>& _field) {
    value_ = _field.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(Deprecated<_deprecation_message, _description, U>&& _field) {
    value_ = std::move(_field.value_);
    return *this;
  }

  /// Returns the underlying object - necessary for the reflection to work.
  const Type& reflection() const { return value_; }

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
