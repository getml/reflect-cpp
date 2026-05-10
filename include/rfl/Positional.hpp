#ifndef RFL_POSITIONAL_HPP_
#define RFL_POSITIONAL_HPP_

#include <type_traits>
#include <utility>

#include "default.hpp"

namespace rfl {

/// Marks a field as positional for CLI argument parsing.
/// In CLI contexts, positional fields are passed without names (e.g., "program arg1 arg2").
/// For non-CLI formats (JSON, YAML, etc.), this wrapper is transparent and has no effect.
/// @tparam T The type of the positional field
template <class T>
struct Positional {
  /// The underlying type.
  using Type = T;

  /// Default constructor - requires Type to be default constructible.
  Positional() requires std::is_default_constructible_v<Type>
      : value_(Type()) {}

  /// Constructs from a const reference to the value.
  /// @param _value The value to store
  Positional(const Type& _value) : value_(_value) {}

  /// Constructs from an rvalue reference to the value.
  /// @param _value The value to store (will be moved)
  Positional(Type&& _value) noexcept : value_(std::move(_value)) {}

  /// Move constructor.
  /// @param _field The Positional to move from
  Positional(Positional<T>&& _field) noexcept = default;

  /// Copy constructor.
  /// @param _field The Positional to copy from
  Positional(const Positional<T>& _field) = default;

  /// Copy constructor from Positional with compatible type.
  /// @tparam U Type compatible with T
  /// @param _field The Positional to copy the value from
  template <class U>
  Positional(const Positional<U>& _field) : value_(_field.get()) {}

  /// Move constructor from Positional with compatible type.
  /// @tparam U Type compatible with T
  /// @param _field The Positional to move the value from
  template <class U>
  Positional(Positional<U>&& _field) : value_(std::move(_field.value_)) {}

  /// Constructs from any type convertible to Type (copy).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store
  template <class U>
    requires std::is_convertible_v<U, Type>
  Positional(const U& _value) : value_(_value) {}

  /// Constructs from any type convertible to Type (move).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store (will be moved)
  template <class U>
    requires std::is_convertible_v<U, Type>
  Positional(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  /// Assigns the underlying object to its default value using the Default sentinel.
  /// @tparam U The type (must be default constructible)
  /// @param _ The default sentinel value
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  Positional(const Default&) : value_(Type()) {}

  /// Destructor.
  ~Positional() = default;

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
  /// @return Reference to this Positional
  auto& operator=(const Type& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns a new value (move version).
  /// @param _value The value to assign (will be moved)
  /// @return Reference to this Positional
  auto& operator=(Type&& _value) noexcept {
    value_ = std::move(_value);
    return *this;
  }

  /// Assigns from any type convertible to the underlying type.
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and assign
  /// @return Reference to this Positional
  template <class U>
    requires std::is_convertible_v<U, Type>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the value to its default using the Default sentinel.
  /// @tparam U The type (must be default constructible)
  /// @param The default sentinel value
  /// @return Reference to this Positional
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  auto& operator=(const Default&) {
    value_ = Type();
    return *this;
  }

  /// Copy assignment operator.
  /// @param _field The Positional to copy from
  /// @return Reference to this Positional
  Positional<T>& operator=(const Positional<T>& _field) = default;

  /// Move assignment operator.
  /// @param _field The Positional to move from
  /// @return Reference to this Positional
  Positional<T>& operator=(Positional<T>&& _field) = default;

  /// Assigns from another Positional with compatible type (copy).
  /// @tparam U Type compatible with T
  /// @param _field The Positional to copy the value from
  /// @return Reference to this Positional
  template <class U>
  auto& operator=(const Positional<U>& _field) {
    value_ = _field.get();
    return *this;
  }

  /// Assigns from another Positional with compatible type (move).
  /// @tparam U Type compatible with T
  /// @param _field The Positional to move the value from
  /// @return Reference to this Positional
  template <class U>
  auto& operator=(Positional<U>&& _field) {
    value_ = std::move(_field.value_);
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
