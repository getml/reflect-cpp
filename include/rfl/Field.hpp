#ifndef RFL_FIELD_HPP_
#define RFL_FIELD_HPP_

#include <algorithm>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "rfl/Literal.hpp"
#include "rfl/default.hpp"
#include "rfl/internal/StringLiteral.hpp"

namespace rfl {

/// Used to define a field in the NamedTuple.
template <internal::StringLiteral _name, class T>
struct Field {
  /// The underlying type.
  using Type = T;

  /// The name of the field.
  using Name = rfl::Literal<_name>;

  Field(const Type& _value) : value_(_value) {}

  Field(Type&& _value) noexcept : value_(std::move(_value)) {}

  Field(Field<_name, T>&& _field) noexcept = default;

  Field(const Field<_name, Type>& _field) = default;

  template <class U>
  Field(const Field<_name, U>& _field) : value_(_field.get()) {}

  template <class U>
  Field(Field<_name, U>&& _field) : value_(_field.get()) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Field(const U& _value) : value_(_value) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Field(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Field(const Field<_name, U>& _field) : value_(_field.value()) {}

  /// Assigns the underlying object to its default value.
  template <class U = Type,
            typename std::enable_if<std::is_default_constructible_v<U>,
                                    bool>::type = true>
  Field(const Default& _default) : value_(Type()) {}

  ~Field() = default;

  /// The name of the field, for internal use.
  constexpr static const internal::StringLiteral name_ = _name;

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
  auto& operator=(const Default& _default) {
    value_ = Type();
    return *this;
  }

  /// Assigns the underlying object.
  Field<_name, T>& operator=(const Field<_name, T>& _field) = default;

  /// Assigns the underlying object.
  Field<_name, T>& operator=(Field<_name, T>&& _field) = default;

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const Field<_name, U>& _field) {
    value_ = _field.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(Field<_name, U>&& _field) {
    value_ = std::forward<T>(_field.value_);
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

template <internal::StringLiteral _name, class T>
inline auto make_field(T&& _value) {
  return Field<_name, T>(std::forward<T>(_value));
}

template <internal::StringLiteral _name, class T>
inline auto make_field(const T& _value) {
  return Field<_name, T>(_value);
}

}  // namespace rfl

#endif  // RFL_FIELD_HPP_
