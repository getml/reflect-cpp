#ifndef RFL_OCT_HPP_
#define RFL_OCT_HPP_

#include <algorithm>
#include <ios>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace rfl {

/// Used to define a field in the NamedTuple.
template <class T>
requires std::is_integral_v<T>
struct Oct {
  /// The underlying type.
  using Type = T;

  using ReflectionType = std::string;

  Oct() : value_(0) {}

  Oct(const Type& _value) : value_(_value) {}

  Oct(Oct<T>&& _other) noexcept = default;

  Oct(const Oct<T>& _other) = default;

  template <class U>
  Oct(const Oct<U>& _other) : value_(_other.get()) {}

  template <class U>
  Oct(Oct<U>&& _other) : value_(_other.get()) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Oct(const U& _value) : value_(_value) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Oct(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Oct(const Oct<U>& _other) : value_(_other.value()) {}

  Oct(const std::string& _str) {
    std::istringstream(_str) >> std::oct >> value_;
  }

  ~Oct() = default;

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
  Oct<T>& operator=(const Oct<T>& _other) = default;

  /// Assigns the underlying object.
  Oct<T>& operator=(Oct<T>&& _other) = default;

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const Oct<U>& _other) {
    value_ = _other.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const std::string& _str) {
    std::istringstream(_str) >> std::oct >> value_;
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(Oct<U>&& _other) {
    value_ = std::forward<T>(_other.value_);
    return *this;
  }

  /// Necessary for the automated parsing to work.
  std::string reflection() const {
    std::stringstream stream;
    stream << std::oct << value_;
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
