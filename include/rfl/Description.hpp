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

/// Used to add a description to the field - this is only relevant for the JSON
/// schema and will be ignored by the normal serialization routines.
template <internal::StringLiteral _description, class T>
struct Description {
  /// The underlying type.
  using Type = T;

  /// The description of the field.
  using Content = rfl::Literal<_description>;

  using ReflectionType = Type;

  Description() : value_(Type()) {}

  Description(const Type& _value) : value_(_value) {}

  Description(Type&& _value) noexcept : value_(std::move(_value)) {}

  Description(Description<_description, T>&& _field) noexcept = default;

  Description(const Description<_description, Type>& _field) = default;

  template <class U>
  Description(const Description<_description, U>& _field)
      : value_(_field.get()) {}

  template <class U>
  Description(Description<_description, U>&& _field) : value_(_field.get()) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Description(const U& _value) : value_(_value) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Description(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Description(const Description<_description, U>& _field)
      : value_(_field.value()) {}

  /// Assigns the underlying object to its default value.
  template <class U = Type,
            typename std::enable_if<std::is_default_constructible_v<U>,
                                    bool>::type = true>
  Description(const Default&) : value_(Type()) {}

  ~Description() = default;

  /// The description of the field, for internal use.
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
  Description<_description, T>& operator=(
      const Description<_description, T>& _field) = default;

  /// Assigns the underlying object.
  Description<_description, T>& operator=(
      Description<_description, T>&& _field) = default;

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const Description<_description, U>& _field) {
    value_ = _field.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(Description<_description, U>&& _field) {
    value_ = std::forward<T>(_field.value_);
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
