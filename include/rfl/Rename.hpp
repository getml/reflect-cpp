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

/// Used to assign a new name to a field, which is different from the name
/// inside the struct.
template <internal::StringLiteral _name, class T>
struct Rename {
  /// The underlying type.
  using Type = T;

  /// The name of the field.
  using Name = rfl::Literal<_name>;

  Rename() : value_(Type()) {}

  Rename(const Type& _value) : value_(_value) {}

  Rename(Type&& _value) noexcept : value_(std::move(_value)) {}

  Rename(Rename<_name, T>&& _field) noexcept = default;

  Rename(const Rename<_name, Type>& _field) = default;

  template <class U>
  Rename(const Rename<_name, U>& _field) : value_(_field.get()) {}

  template <class U>
  Rename(Rename<_name, U>&& _field) : value_(_field.get()) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Rename(const U& _value) : value_(_value) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Rename(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Rename(const Rename<_name, U>& _field) : value_(_field.value()) {}

  /// Assigns the underlying object to its default value.
  template <class U = Type,
            typename std::enable_if<std::is_default_constructible_v<U>,
                                    bool>::type = true>
  Rename(const Default&) : value_(Type()) {}

  ~Rename() = default;

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
  auto& operator=(const Default&) {
    value_ = Type();
    return *this;
  }

  /// Assigns the underlying object.
  Rename<_name, T>& operator=(const Rename<_name, T>& _field) = default;

  /// Assigns the underlying object.
  Rename<_name, T>& operator=(Rename<_name, T>&& _field) = default;

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const Rename<_name, U>& _field) {
    value_ = _field.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(Rename<_name, U>&& _field) {
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

}  // namespace rfl

#endif
