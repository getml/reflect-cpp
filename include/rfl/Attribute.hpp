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

template <class T>
struct Attribute {
  using Type = T;
  using ReflectionType = T;

  Attribute() : value_(Type()) {}

  Attribute(const Type& _value) : value_(_value) {}

  Attribute(Type&& _value) noexcept : value_(std::move(_value)) {}

  Attribute(Attribute<T>&& _attr) noexcept = default;

  Attribute(const Attribute<Type>& _attr) = default;

  template <class U>
  Attribute(const Attribute<U>& _attr) : value_(_attr.get()) {}

  template <class U>
  Attribute(Attribute<U>&& _attr) : value_(_attr.get()) {}

  template <class U>
    requires std::is_convertible_v<U, Type>
  Attribute(const U& _value) : value_(_value) {}

  template <class U>
    requires std::is_convertible_v<U, Type>
  Attribute(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  template <class U>
    requires std::is_convertible_v<U, Type>
  Attribute(const Attribute<U>& _attr) : value_(_attr.value()) {}

  /// Assigns the underlying object to its default value.
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  Attribute(const Default&) : value_(Type()) {}

  ~Attribute() = default;

  /// Returns the underlying object.
  const Type& get() const noexcept { return value_; }

  /// Returns the underlying object.
  Type& get() noexcept { return value_; }

  /// Returns the underlying object.
  Type& operator*() noexcept { return value_; }

  /// Returns the underlying object.
  const Type& operator*() const noexcept { return value_; }

  /// Returns the underlying object.
  Type& operator()() noexcept { return value_; }

  /// Returns the underlying object.
  const Type& operator()() const noexcept { return value_; }

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
  Attribute<T>& operator=(const Attribute<T>& _attr) = default;

  /// Assigns the underlying object.
  Attribute<T>& operator=(Attribute<T>&& _attr) = default;

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const Attribute<U>& _attr) {
    value_ = _attr.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(Attribute<U>&& _attr) {
    value_ = std::forward<T>(_attr.value_);
    return *this;
  }

  /// We want all parsers other than the XML parser to treat attributes like
  /// normal fields, so we just implement the reflection interface.
  const ReflectionType& reflection() const { return value_; }

  /// Assigns the underlying object.
  void set(const Type& _value) { value_ = _value; }

  /// Assigns the underlying object.
  void set(Type&& _value) { value_ = std::move(_value); }

  /// Returns the underlying object.
  Type& value() noexcept { return value_; }

  /// Returns the underlying object.
  const Type& value() const noexcept { return value_; }

  /// The underlying value.
  Type value_;
};

}  // namespace rfl

#endif
