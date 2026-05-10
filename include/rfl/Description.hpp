#ifndef RFL_DESCRIPTION_HPP_
#define RFL_DESCRIPTION_HPP_

#include <algorithm>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "Literal.hpp"
#include "default.hpp"
#include "internal/StringLiteral.hpp"

namespace rfl {

/// Used to add a description to a field for documentation and JSON schema generation.
/// The description is used when generating JSON schemas but is ignored during normal
/// serialization/deserialization. This allows you to document fields in a way that
/// appears in generated schemas.
/// @tparam _description The compile-time string literal description text
/// @tparam T The type of the field value
template <internal::StringLiteral _description, class T>
struct Description {
  /// The underlying type.
  using Type = T;

  /// The description as a Literal type.
  using Content = rfl::Literal<_description>;

  using ReflectionType = Type;

  /// Default constructor.
  Description() : value_(Type()) {}

  /// Constructs from a const reference to the value.
  /// @param _value The value to store
  Description(const Type& _value) : value_(_value) {}

  /// Constructs from an rvalue reference to the value.
  /// @param _value The value to store (will be moved)
  Description(Type&& _value) noexcept : value_(std::move(_value)) {}

  /// Move constructor.
  /// @param _field The Description to move from
  Description(Description&& _field) noexcept = default;

  /// Copy constructor.
  /// @param _field The Description to copy from
  Description(const Description& _field) = default;

  /// Copy constructor from Description with same description but compatible type.
  /// @tparam U Type compatible with T
  /// @param _field The Description to copy the value from
  template <class U>
  Description(const Description<_description, U>& _field)
      : value_(_field.get()) {}

  /// Move constructor from Description with same description but compatible type.
  /// @tparam U Type compatible with T
  /// @param _field The Description to move the value from
  template <class U>
  Description(Description<_description, U>&& _field) : value_(_field.get()) {}

  /// Constructs from any type convertible to Type (copy).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store
  template <class U>
    requires std::is_convertible_v<U, Type>
  Description(const U& _value) : value_(_value) {}

  /// Constructs from any type convertible to Type (move).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store (will be moved)
  template <class U>
    requires std::is_convertible_v<U, Type>
  Description(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  /// Constructs from a Description with compatible type.
  /// @tparam U Type convertible to T
  /// @param _field The Description to copy the value from
  template <class U>
    requires std::is_convertible_v<U, Type>
  Description(const Description<_description, U>& _field)
      : value_(_field.value()) {}

  /// Assigns the underlying object to its default value using the Default sentinel.
  /// @tparam U The type (must be default constructible)
  /// @param The default sentinel value
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  Description(const Default&) : value_(Type()) {}

  /// Destructor.
  ~Description() = default;

  /// The description as a compile-time string literal, for internal use.
  constexpr static const internal::StringLiteral description_ = _description;

  /// Returns the underlying value.
  /// @return Const reference to the stored value
  const Type& get() const noexcept { return value_; }

  /// Returns the underlying value.
  /// @return Reference to the stored value
  Type& get() noexcept { return value_; }

  /// Dereference operator - returns the underlying value.
  /// @return Reference to the stored value
  Type& operator*() noexcept { return value_; }

  /// Dereference operator (const) - returns the underlying value.
  /// @return Const reference to the stored value
  const Type& operator*() const noexcept { return value_; }

  /// Function call operator - returns the underlying value.
  /// @return Reference to the stored value
  Type& operator()() noexcept { return value_; }

  /// Function call operator (const) - returns the underlying value.
  /// @return Const reference to the stored value
  const Type& operator()() const noexcept { return value_; }

  /// Assigns a new value.
  /// @param _value The value to assign
  /// @return Reference to this Description
  auto& operator=(const Type& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns a new value (move version).
  /// @param _value The value to assign (will be moved)
  /// @return Reference to this Description
  auto& operator=(Type&& _value) noexcept {
    value_ = std::move(_value);
    return *this;
  }

  /// Assigns from any type convertible to the underlying type.
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and assign
  /// @return Reference to this Description
  template <class U>
    requires std::is_convertible_v<U, Type>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the value to its default using the Default sentinel.
  /// @tparam U The type (must be default constructible)
  /// @param The default sentinel value
  /// @return Reference to this Description
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  auto& operator=(const Default&) {
    value_ = Type();
    return *this;
  }

  /// Copy assignment operator.
  /// @param _field The Description to copy from
  /// @return Reference to this Description
  Description& operator=(const Description& _field) = default;

  /// Move assignment operator.
  /// @param _field The Description to move from
  /// @return Reference to this Description
  Description& operator=(Description&& _field) = default;

  /// Assigns from another Description with same description but compatible type (copy).
  /// @tparam U Type compatible with T
  /// @param _field The Description to copy the value from
  /// @return Reference to this Description
  template <class U>
  auto& operator=(const Description<_description, U>& _field) {
    value_ = _field.get();
    return *this;
  }

  /// Assigns from another Description with same description but compatible type (move).
  /// @tparam U Type compatible with T
  /// @param _field The Description to move the value from
  /// @return Reference to this Description
  template <class U>
  auto& operator=(Description<_description, U>&& _field) {
    value_ = std::forward<T>(_field.value_);
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
  Type& value() noexcept { return value_; }

  /// Returns the underlying value (const).
  /// @return Const reference to the stored value
  const Type& value() const noexcept { return value_; }

  /// The underlying value.
  Type value_;
};

}  // namespace rfl

#endif
