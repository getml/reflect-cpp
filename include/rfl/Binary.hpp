#ifndef RFL_BINARY_HPP_
#define RFL_BINARY_HPP_

#include <algorithm>
#include <bitset>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace rfl {

/// Used to define a field in the NamedTuple.
template <class T>
requires std::is_unsigned_v<T>
struct Binary {
  /// The underlying type.
  using Type = T;

  using ReflectionType = std::string;

  using Bitset = std::bitset<sizeof(Type) * 8>;

  Binary() : value_(0) {}

  Binary(const Type& _value) : value_(_value) {}

  Binary(Binary<T>&& _other) noexcept = default;

  Binary(const Binary<T>& _other) = default;

  template <class U>
  Binary(const Binary<U>& _other) : value_(_other.get()) {}

  template <class U>
  Binary(Binary<U>&& _other) : value_(_other.get()) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Binary(const U& _value) : value_(_value) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Binary(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Binary(const Binary<U>& _other) : value_(_other.value()) {}

  Binary(const std::string& _str)
      : value_(static_cast<T>(Bitset{_str}.to_ullong())) {}

  ~Binary() = default;

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
  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the underlying object.
  Binary<T>& operator=(const Binary<T>& _other) = default;

  /// Assigns the underlying object.
  Binary<T>& operator=(Binary<T>&& _other) = default;

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const Binary<U>& _other) {
    value_ = _other.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const std::string& _str) {
    value_ = static_cast<T>(Bitset{_str}.to_ullong());
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(Binary<U>&& _other) {
    value_ = std::forward<T>(_other.value_);
    return *this;
  }

  /// Necessary for the automated parsing to work.
  std::string reflection() const { return Bitset{value_}.to_string(); }

  /// Assigns the underlying object.
  void set(const Type& _value) { value_ = _value; }

  /// Returns the underlying value as a string, alias for .reflection().
  std::string str() const { return reflection(); }

  /// Returns the underlying object.
  Type& value() { return value_; }

  /// Returns the underlying object.
  const Type& value() const { return value_; }

  /// The underlying value.
  Type value_;
};

}  // namespace rfl

#endif  // RFL_FIELD_HPP_
