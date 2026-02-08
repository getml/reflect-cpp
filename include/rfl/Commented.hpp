#ifndef RFL_COMMENTED_HPP_
#define RFL_COMMENTED_HPP_

#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "default.hpp"

namespace rfl {

template <class T>
class Commented {
 public:
  using Type = std::remove_cvref_t<T>;

  Commented() : value_(Type()) {}

  Commented(const Type& _value) : value_(_value) {}

  Commented(Type&& _value) noexcept : value_(std::move(_value)) {}

  Commented(const Type& _value, std::optional<std::string> _comment)
      : comment_(std::move(_comment)), value_(_value) {}

  Commented(Type&& _value, std::optional<std::string> _comment) noexcept
      : comment_(std::move(_comment)), value_(std::move(_value)) {}

  Commented(Commented&& _commented) noexcept = default;

  Commented(const Commented& _commented) = default;

  template <class U>
  Commented(const Commented<U>& _commented)
      : comment_(_commented.comment()), value_(_commented.get()) {}

  template <class U>
  Commented(Commented<U>&& _commented) noexcept(
      noexcept(Type(std::move(_commented.value()))))
      : comment_(std::move(_commented.comment())),
        value_(std::move(_commented.value())) {}

  template <class U>
    requires(std::is_convertible_v<U, Type>)
  Commented(const U& _value) : value_(_value) {}

  template <class U>
    requires(std::is_convertible_v<U, Type>)
  Commented(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  template <class U>
    requires(std::is_convertible_v<U, Type>)
  Commented(const Commented<U>& _commented) : value_(_commented.value()) {}

  /// Assigns the underlying object to its default value.
  template <class U = Type>
    requires(std::is_default_constructible_v<U>)
  Commented(const Default&) : value_(Type()) {}

  ~Commented() = default;

  /// Sets the comment associated with the field.
  void add_comment(std::string _comment) { comment_ = std::move(_comment); }

  /// Returns the comment associated with the field, if any.
  const std::optional<std::string>& comment() const { return comment_; }

  /// Returns the underlying object.
  Type& get() { return value_; }

  /// Returns the underlying object.
  const Type& get() const { return value_; }

  /// Returns the underlying object.
  Type& operator()() { return value_; }

  /// Returns the underlying object.
  const Type& operator()() const { return value_; }

  /// Returns the underlying object.
  Type& operator*() { return value_; }

  /// Returns the underlying object.
  const Type& operator*() const { return value_; }

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
  Commented& operator=(const Commented& _commented) = default;

  /// Assigns the underlying object.
  Commented& operator=(Commented&& _commented) = default;

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const Commented<U>& _commented) {
    value_ = _commented.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(Commented<U>&& _commented) {
    value_ = std::forward<U>(_commented.value_);
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

 private:
  /// The comment associated with the field.
  std::optional<std::string> comment_;

  /// The underlying value.
  Type value_;
};

}  // namespace rfl

#endif
