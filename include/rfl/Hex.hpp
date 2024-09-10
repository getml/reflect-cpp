#ifndef RFL_HEX_HPP_
#define RFL_HEX_HPP_

#include <algorithm>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace rfl {

/// Used to define a field in the NamedTuple.
template <class T>
requires std::is_integral_v<T>
struct Hex {
  /// The underlying type.
  using Type = T;

  using ReflectionType = std::string;

  Hex() : value_(0) {}

  Hex(const Type& _value) : value_(_value) {}

  Hex(Hex<T>&& _other) noexcept = default;

  Hex(const Hex<T>& _other) = default;

  template <class U>
  Hex(const Hex<U>& _other) : value_(_other.get()) {}

  template <class U>
  Hex(Hex<U>&& _other) : value_(_other.get()) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Hex(const U& _value) : value_(_value) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Hex(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Hex(const Hex<U>& _other) : value_(_other.value()) {}

  Hex(const std::string& _str) {
    std::istringstream(_str) >> std::hex >> value_;
  }

  ~Hex() = default;

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
  Hex<T>& operator=(const Hex<T>& _other) = default;

  /// Assigns the underlying object.
  Hex<T>& operator=(Hex<T>&& _other) = default;

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const Hex<U>& _other) {
    value_ = _other.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const std::string& _str) {
    std::istringstream(_str) >> std::hex >> value_;
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(Hex<U>&& _other) {
    value_ = std::forward<T>(_other.value_);
    return *this;
  }

  /// Necessary for the automated parsing to work.
  std::string reflection() const {
    std::stringstream stream;
    stream << std::hex << value_;
    return stream.str();
  }

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
