#ifndef RFL_DEFAULTVAL_HPP_
#define RFL_DEFAULTVAL_HPP_

#include <type_traits>
#include <utility>

#include "default.hpp"

namespace rfl {

template <class T>
struct DefaultVal {
 public:
  using Type = std::remove_cvref_t<T>;

  DefaultVal() : value_(Type()) {}

  DefaultVal(const Type& _value) : value_(_value) {}

  DefaultVal(Type&& _value) noexcept : value_(std::move(_value)) {}

  DefaultVal(DefaultVal&& _field) noexcept = default;

  DefaultVal(const DefaultVal& _field) = default;

  template <class U>
  DefaultVal(const DefaultVal<U>& _field) : value_(_field.get()) {}

  template <class U>
  DefaultVal(DefaultVal<U>&& _field) noexcept(
      noexcept(Type(std::move(_field.value()))))
      : value_(std::move(_field.value())) {}

  template <class U>
    requires(std::is_convertible_v<U, Type>)
  DefaultVal(const U& _value) : value_(_value) {}

  template <class U>
    requires(std::is_convertible_v<U, Type>)
  DefaultVal(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  template <class U>
    requires(std::is_convertible_v<U, Type>)
  DefaultVal(const DefaultVal<U>& _field) : value_(_field.value()) {}

  /// Assigns the underlying object to its default value.
  template <class U = Type>
    requires(std::is_default_constructible_v<U>)
  DefaultVal(const Default&) : value_(Type()) {}

  ~DefaultVal() = default;

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
  DefaultVal& operator=(const DefaultVal& _field) = default;

  /// Assigns the underlying object.
  DefaultVal& operator=(DefaultVal&& _field) = default;

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const DefaultVal<U>& _field) {
    value_ = _field.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(DefaultVal<U>&& _field) {
    value_ = std::forward<U>(_field.value_);
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
