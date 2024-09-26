#ifndef RFL_GENERIC_HPP_
#define RFL_GENERIC_HPP_

#include <string>
#include <variant>
#include <vector>

#include "Object.hpp"
#include "Result.hpp"

namespace rfl {

class Generic {
 public:
  using Array = std::vector<Generic>;
  using Object = rfl::Object<Generic>;
  using ReflectionType =
      std::variant<bool, int, double, std::string, Object, Array>;

  Generic();

  Generic(Generic&& _other) noexcept;

  Generic(const Generic& _other);

  Generic(const ReflectionType& _value);

  Generic(ReflectionType&& _value) noexcept;

  template <class T,
            typename std::enable_if<std::is_convertible_v<T, ReflectionType>,
                                    bool>::type = true>
  Generic(const T& _value) : value_(_value) {}

  template <class T,
            typename std::enable_if<std::is_convertible_v<T, ReflectionType>,
                                    bool>::type = true>
  Generic(T&& _value) noexcept : value_(std::forward<T>(_value)) {}

  ~Generic();

  /// Returns the underlying object.
  const ReflectionType& get() const { return value_; }

  /// Assigns the underlying object.
  Generic& operator=(const ReflectionType& _value);

  /// Assigns the underlying object.
  Generic& operator=(ReflectionType&& _value) noexcept;

  /// Assigns the underlying object.
  template <class T,
            typename std::enable_if<std::is_convertible_v<T, ReflectionType>,
                                    bool>::type = true>
  auto& operator=(const T& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the underlying object.
  Generic& operator=(const Generic& _other);

  /// Assigns the underlying object.
  Generic& operator=(Generic&& _other);

  /// Returns the underlying object, necessary for the serialization to work.
  const ReflectionType& reflection() const { return value_; };

  /// Casts the underlying value to an rfl::Generic::Array or returns an
  /// rfl::Error, if the underlying value is not an rfl::Generic::Array.
  Result<Array> to_array() const noexcept;

  /// Casts the underlying value to a boolean or returns an rfl::Error, if the
  /// underlying value is not a boolean.
  Result<bool> to_bool() const noexcept;

  /// Casts the underlying value to a double or returns an rfl::Error, if the
  /// underlying value is not a double.
  Result<double> to_double() const noexcept;

  /// Casts the underlying value to an integer or returns an rfl::Error, if the
  /// underlying value is not an integer.
  Result<int> to_int() const noexcept;

  /// Casts the underlying value to an rfl::Generic::Object or returns an
  /// rfl::Error, if the underlying value is not an rfl::Generic::Object.
  Result<Object> to_object() const noexcept;

  /// Casts the underlying value to a string or returns an rfl::Error, if the
  /// underlying value is not a string.
  Result<std::string> to_string() const noexcept;

 private:
  ReflectionType value_;
};

/// Casts the underlying value to an rfl::Generic::Array or returns an
/// rfl::Error, if the underlying value is not an rfl::Generic::Array.
inline Result<Generic::Array> to_array(const Generic& _g) noexcept {
  return _g.to_array();
}

/// Casts the underlying value to a boolean or returns an rfl::Error, if the
/// underlying value is not a boolean.
inline Result<bool> to_bool(const Generic& _g) noexcept { return _g.to_bool(); }

/// Casts the underlying value to a double or returns an rfl::Error, if the
/// underlying value is not a double.
inline Result<double> to_double(const Generic& _g) noexcept {
  return _g.to_double();
}

/// Casts the underlying value to an integer or returns an rfl::Error, if the
/// underlying value is not an integer.
inline Result<int> to_int(const Generic& _g) noexcept { return _g.to_int(); }

/// Casts the underlying value to an rfl::Generic::Object or returns an
/// rfl::Error, if the underlying value is not an rfl::Generic::Object.
inline Result<Generic::Object> to_object(const Generic& _g) noexcept {
  return _g.to_object();
}

/// Casts the underlying value to a string or returns an rfl::Error, if the
/// underlying value is not a string.
inline Result<std::string> to_string(const Generic& _g) noexcept {
  return _g.to_string();
}

}  // namespace rfl

#endif
