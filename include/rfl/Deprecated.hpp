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

/// Marks a field as deprecated and provides deprecation information for JSON schema generation.
/// Similar to Description, but specifically for deprecated fields. The deprecation message and
/// description are used when generating JSON schemas but are ignored during normal serialization/
/// deserialization. This allows you to document deprecated fields in a way that appears in
/// generated schemas and warns API users.
/// @tparam _deprecation_message The compile-time deprecation message (e.g., "use field X instead")
/// @tparam _description The compile-time description of what the field does
/// @tparam T The type of the field value
template <internal::StringLiteral _deprecation_message,
          internal::StringLiteral _description, class T>
struct Deprecated {
  /// The underlying type.
  using Type = T;

  /// The deprecation message as a Literal type.
  using DeprecationMessage = rfl::Literal<_deprecation_message>;

  /// The description as a Literal type (same role as Description::Content).
  using Content = rfl::Literal<_description>;

  using ReflectionType = Type;

  /// Default constructor.
  Deprecated() : value_(Type()) {}

  /// Constructs from a const reference to the value.
  /// @param _value The value to store
  Deprecated(const Type& _value) : value_(_value) {}

  /// Constructs from an rvalue reference to the value.
  /// @param _value The value to store (will be moved)
  Deprecated(Type&& _value) noexcept : value_(std::move(_value)) {}

  /// Move constructor.
  /// @param _field The Deprecated to move from
  Deprecated(Deprecated&& _field) noexcept = default;

  /// Copy constructor.
  /// @param _field The Deprecated to copy from
  Deprecated(const Deprecated& _field) = default;

  /// Copy constructor from Deprecated with same messages but compatible type.
  /// @tparam U Type compatible with T
  /// @param _field The Deprecated to copy the value from
  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Deprecated(const Deprecated<_deprecation_message, _description, U>& _field)
      : value_(_field.get()) {}

  /// Move constructor from Deprecated with same messages but compatible type.
  /// @tparam U Type compatible with T
  /// @param _field The Deprecated to move the value from
  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Deprecated(Deprecated<_deprecation_message, _description, U>&& _field)
      : value_(std::move(_field.value_)) {}

  /// Constructs from any type convertible to Type (copy).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store
  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Deprecated(const U& _value) : value_(_value) {}

  /// Constructs from any type convertible to Type (move).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store (will be moved)
  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Deprecated(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  /// Assigns the underlying object to its default value using the Default sentinel.
  /// @tparam U The type (must be default constructible)
  /// @param _ The default sentinel value
  template <class U = Type,
            typename std::enable_if<std::is_default_constructible_v<U>,
                                    bool>::type = true>
  Deprecated(const Default&) : value_(Type()) {}

  /// Destructor.
  ~Deprecated() = default;

  /// The deprecation message as a compile-time string literal, for internal use.
  constexpr static const internal::StringLiteral deprecation_message_ =
      _deprecation_message;

  /// The description as a compile-time string literal, for internal use.
  constexpr static const internal::StringLiteral description_ = _description;

  /// Returns the underlying value.
  /// @return Const reference to the stored value
  const Type& get() const { return value_; }

  /// Function call operator - returns the underlying value.
  /// @return Reference to the stored value
  Type& operator()() { return value_; }

  /// Function call operator (const) - returns the underlying value.
  /// @return Const reference to the stored value
  const Type& operator()() const { return value_; }

  /// Assigns a new value.
  /// @param _value The value to assign
  /// @return Reference to this Deprecated
  auto& operator=(const Type& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns a new value (move version).
  /// @param _value The value to assign (will be moved)
  /// @return Reference to this Deprecated
  auto& operator=(Type&& _value) noexcept {
    value_ = std::move(_value);
    return *this;
  }

  /// Assigns from any type convertible to the underlying type.
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and assign
  /// @return Reference to this Deprecated
  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the value to its default using the Default sentinel.
  /// @tparam U The type (must be default constructible)
  /// @param The default sentinel value
  /// @return Reference to this Deprecated
  template <class U = Type,
            typename std::enable_if<std::is_default_constructible_v<U>,
                                    bool>::type = true>
  auto& operator=(const Default&) {
    value_ = Type();
    return *this;
  }

  /// Copy assignment operator.
  /// @param _field The Deprecated to copy from
  /// @return Reference to this Deprecated
  Deprecated& operator=(const Deprecated& _field) = default;

  /// Move assignment operator.
  /// @param _field The Deprecated to move from
  /// @return Reference to this Deprecated
  Deprecated& operator=(Deprecated&& _field) = default;

  /// Assigns from another Deprecated with same messages but compatible type (copy).
  /// @tparam U Type compatible with T
  /// @param _field The Deprecated to copy the value from
  /// @return Reference to this Deprecated
  template <class U>
  auto& operator=(
      const Deprecated<_deprecation_message, _description, U>& _field) {
    value_ = _field.get();
    return *this;
  }

  /// Assigns from another Deprecated with same messages but compatible type (move).
  /// @tparam U Type compatible with T
  /// @param _field The Deprecated to move the value from
  /// @return Reference to this Deprecated
  template <class U>
  auto& operator=(Deprecated<_deprecation_message, _description, U>&& _field) {
    value_ = std::move(_field.value_);
    return *this;
  }

  /// Returns the underlying value for reflection (used by parsers).
  /// @return Const reference to the stored value
  const Type& reflection() const { return value_; }

  /// Sets the value (copy version).
  /// @param _value The value to set
  void set(const Type& _value) { value_ = _value; }

  /// Sets the value (move version).
  /// @param _value The value to set (will be moved)
  void set(Type&& _value) { value_ = std::move(_value); }

  /// Returns the underlying value.
  /// @return Reference to the stored value
  Type& value() { return value_; }

  /// Returns the underlying value (const).
  /// @return Const reference to the stored value
  const Type& value() const { return value_; }

  /// The underlying value.
  Type value_;
};

}  // namespace rfl

#endif
