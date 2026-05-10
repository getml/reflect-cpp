#ifndef RFL_ATTRIBUTE_HPP_
#define RFL_ATTRIBUTE_HPP_

#include <algorithm>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "Literal.hpp"
#include "default.hpp"

namespace rfl {

/// Wraps a field value to mark it as an XML attribute.
/// In XML serialization, fields wrapped in Attribute will be rendered as attributes
/// on the XML element rather than child elements. For non-XML formats, this wrapper
/// is transparent and behaves like a normal field.
/// Example: <element attr="value"> instead of <element><field>value</field></element>
/// @tparam T The type of the attribute value
template <class T>
struct Attribute {
  using Type = T;
  using ReflectionType = T;

  /// Default constructor.
  Attribute() : value_(Type()) {}

  /// Constructs from a const reference to the value.
  /// @param _value The value to store
  Attribute(const Type& _value) : value_(_value) {}

  /// Constructs from an rvalue reference to the value.
  /// @param _value The value to store (will be moved)
  Attribute(Type&& _value) noexcept : value_(std::move(_value)) {}

  /// Move constructor.
  /// @param _attr The Attribute to move from
  Attribute(Attribute<T>&& _attr) noexcept = default;

  /// Copy constructor.
  /// @param _attr The Attribute to copy from
  Attribute(const Attribute<Type>& _attr) = default;

  /// Copy constructor from Attribute with compatible type.
  /// @tparam U Type compatible with T
  /// @param _attr The Attribute to copy the value from
  template <class U>
  Attribute(const Attribute<U>& _attr) : value_(_attr.get()) {}

  /// Move constructor from Attribute with compatible type.
  /// @tparam U Type compatible with T
  /// @param _attr The Attribute to move the value from
  template <class U>
  Attribute(Attribute<U>&& _attr) : value_(_attr.get()) {}

  /// Constructs from any type convertible to Type (copy).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store
  template <class U>
    requires std::is_convertible_v<U, Type>
  Attribute(const U& _value) : value_(_value) {}

  /// Constructs from any type convertible to Type (move).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store (will be moved)
  template <class U>
    requires std::is_convertible_v<U, Type>
  Attribute(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  /// Constructs from an Attribute with compatible type.
  /// @tparam U Type convertible to T
  /// @param _attr The Attribute to copy the value from
  template <class U>
    requires std::is_convertible_v<U, Type>
  Attribute(const Attribute<U>& _attr) : value_(_attr.value()) {}

  /// Assigns the underlying object to its default value using the Default sentinel.
  /// @tparam U The type (must be default constructible)
  /// @param The default sentinel value
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  Attribute(const Default&) : value_(Type()) {}

  /// Destructor.
  ~Attribute() = default;

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
  /// @return Reference to this Attribute
  auto& operator=(const Type& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns a new value (move version).
  /// @param _value The value to assign (will be moved)
  /// @return Reference to this Attribute
  auto& operator=(Type&& _value) noexcept {
    value_ = std::move(_value);
    return *this;
  }

  /// Assigns from any type convertible to the underlying type.
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and assign
  /// @return Reference to this Attribute
  template <class U>
    requires std::is_convertible_v<U, Type>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the value to its default using the Default sentinel.
  /// @tparam U The type (must be default constructible)
  /// @param The default sentinel value
  /// @return Reference to this Attribute
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  auto& operator=(const Default&) {
    value_ = Type();
    return *this;
  }

  /// Copy assignment operator.
  /// @param _attr The Attribute to copy from
  /// @return Reference to this Attribute
  Attribute<T>& operator=(const Attribute<T>& _attr) = default;

  /// Move assignment operator.
  /// @param _attr The Attribute to move from
  /// @return Reference to this Attribute
  Attribute<T>& operator=(Attribute<T>&& _attr) = default;

  /// Assigns from another Attribute with compatible type (copy).
  /// @tparam U Type compatible with T
  /// @param _attr The Attribute to copy the value from
  /// @return Reference to this Attribute
  template <class U>
  auto& operator=(const Attribute<U>& _attr) {
    value_ = _attr.get();
    return *this;
  }

  /// Assigns from another Attribute with compatible type (move).
  /// @tparam U Type compatible with T
  /// @param _attr The Attribute to move the value from
  /// @return Reference to this Attribute
  template <class U>
  auto& operator=(Attribute<U>&& _attr) {
    value_ = std::move(_attr.value_);
    return *this;
  }

  /// Returns the underlying value for reflection (used by parsers).
  /// Non-XML parsers treat attributes like normal fields.
  /// @return Const reference to the stored value
  const ReflectionType& reflection() const { return value_; }

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
