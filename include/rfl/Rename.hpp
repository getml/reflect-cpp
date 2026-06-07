#ifndef RFL_RENAME_HPP_
#define RFL_RENAME_HPP_

#include <algorithm>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "Literal.hpp"
#include "default.hpp"
#include "internal/StringLiteral.hpp"

namespace rfl {

/// Used to assign a new name to a field for serialization/deserialization.
/// This allows the field name in the struct to differ from the name used in
/// serialized formats. For example, a C++ field 'firstName' can be serialized
/// as 'first_name' in JSON.
/// @tparam _name The compile-time string literal name to use in serialized
/// formats
/// @tparam T The type of the value stored in the field
template <internal::StringLiteral _name, class T>
struct Rename {
  /// The underlying type.
  using Type = T;

  /// The name of the field as a Literal type.
  using Name = rfl::Literal<_name>;

  /// Default constructor - creates a Rename with a default-constructed value.
  Rename() : value_(Type()) {}

  /// Constructs from a const reference to the value.
  /// @param _value The value to store
  Rename(const Type& _value) : value_(_value) {}

  /// Constructs from an rvalue reference to the value.
  /// @param _value The value to store (will be moved)
  Rename(Type&& _value) noexcept : value_(std::move(_value)) {}

  /// Move constructor.
  /// @param _field The Rename to move from
  Rename(Rename<_name, T>&& _field) noexcept = default;

  /// Copy constructor.
  /// @param _field The Rename to copy from
  Rename(const Rename<_name, Type>& _field) = default;

  /// Copy constructor from Rename with compatible type.
  /// @tparam U Type compatible with T
  /// @param _field The Rename to copy the value from
  template <class U>
  Rename(const Rename<_name, U>& _field) : value_(_field.get()) {}

  /// Move constructor from Rename with compatible type.
  /// @tparam U Type compatible with T
  /// @param _field The Rename to move the value from
  template <class U>
  Rename(Rename<_name, U>&& _field) : value_(_field.get()) {}

  /// Constructs from any type convertible to Type (copy).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store
  template <class U>
    requires std::is_convertible_v<U, Type>
  Rename(const U& _value) : value_(_value) {}

  /// Constructs from any type convertible to Type (move).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store (will be moved)
  template <class U>
    requires std::is_convertible_v<U, Type>
  Rename(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  /// Constructs from a Rename with compatible type.
  /// @tparam U Type convertible to T
  /// @param _field The Rename to copy the value from
  template <class U>
    requires std::is_convertible_v<U, Type>
  Rename(const Rename<_name, U>& _field) : value_(_field.value()) {}

  /// Assigns the underlying object to its default value using the Default
  /// sentinel.
  /// @tparam U The type (must be default constructible)
  /// @param _ The default sentinel value
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  Rename(const Default&) : value_(Type()) {}

  /// Destructor.
  ~Rename() = default;

  /// The name of the field as a compile-time string literal, for internal use.
  constexpr static const internal::StringLiteral name_ = _name;

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
  /// @return Reference to this Rename
  auto& operator=(const Type& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns a new value (move version).
  /// @param _value The value to assign (will be moved)
  /// @return Reference to this Rename
  auto& operator=(Type&& _value) noexcept {
    value_ = std::move(_value);
    return *this;
  }

  /// Assigns from any type convertible to the underlying type.
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and assign
  /// @return Reference to this Rename
  template <class U>
    requires std::is_convertible_v<U, Type>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the value to its default using the Default sentinel.
  /// @tparam U The type (must be default constructible)
  /// @param The default sentinel value
  /// @return Reference to this Rename
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  auto& operator=(const Default&) {
    value_ = Type();
    return *this;
  }

  /// Copy assignment operator.
  /// @param _field The Rename to copy from
  /// @return Reference to this Rename
  Rename<_name, T>& operator=(const Rename<_name, T>& _field) = default;

  /// Move assignment operator.
  /// @param _field The Rename to move from
  /// @return Reference to this Rename
  Rename<_name, T>& operator=(Rename<_name, T>&& _field) = default;

  /// Assigns from another Rename with compatible type (copy).
  /// @tparam U Type compatible with T
  /// @param _field The Rename to copy the value from
  /// @return Reference to this Rename
  template <class U>
  auto& operator=(const Rename<_name, U>& _field) {
    value_ = _field.get();
    return *this;
  }

  /// Assigns from another Rename with compatible type (move).
  /// @tparam U Type compatible with T
  /// @param _field The Rename to move the value from
  /// @return Reference to this Rename
  template <class U>
  auto& operator=(Rename<_name, U>&& _field) {
    value_ = std::forward<T>(_field.value_);
    return *this;
  }

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
