#ifndef RFL_GENERIC_HPP_
#define RFL_GENERIC_HPP_

#include <optional>
#include <ostream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "Object.hpp"
#include "Result.hpp"
#include "Variant.hpp"

namespace rfl {

class Generic {
 public:
  constexpr static std::nullopt_t Null = std::nullopt;

  using Array = std::vector<Generic>;
  using Object = rfl::Object<Generic>;
  using VariantType = std::variant<bool, int64_t, double, std::string, Object,
                                   Array, std::nullopt_t>;
  using ReflectionType = std::optional<
      std::variant<bool, int64_t, double, std::string, Object, Array>>;

  Generic();

  Generic(Generic&& _other) noexcept;

  Generic(const Generic& _other);

  Generic(const VariantType& _value);

  Generic(VariantType&& _value) noexcept;

  Generic(const ReflectionType& _value);

  template <class T,
            typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                    bool>::type = true>
  Generic(const T& _value) {
    value_ = _value;
  }

  template <class T,
            typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                    bool>::type = true>
  Generic(T&& _value) noexcept : value_(std::forward<T>(_value)) {}

  ~Generic();

  /// Returns the underlying object.
  const VariantType& get() const { return value_; }

  /// Whether the object contains the null value.
  bool is_null() const noexcept;

  /// Assigns the underlying object.
  Generic& operator=(const VariantType& _value);

  /// Assigns the underlying object.
  Generic& operator=(VariantType&& _value) noexcept;

  /// Assigns the underlying object.
  template <class T,
            typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                    bool>::type = true>
  auto& operator=(const T& _value) {
    using Type = std::remove_cvref_t<T>;
    if constexpr (std::is_same_v<Type, bool>) {
      value_.emplace<0>(_value);
    } else if constexpr (std::is_integral_v<Type>) {
      value_.emplace<1>(static_cast<int64_t>(_value));
    } else if constexpr (std::is_floating_point_v<Type>) {
      value_.emplace<2>(static_cast<double>(_value));
    } else {
      value_ = _value;
    }
    return *this;
  }

  /// Assigns the underlying object.
  Generic& operator=(const Generic& _other);

  /// Assigns the underlying object.
  Generic& operator=(Generic&& _other);

  /// Returns the underlying object, necessary for the serialization to work.
  ReflectionType reflection() const noexcept;

  /// Casts the underlying value to an rfl::Generic::Array or returns an
  /// rfl::Error, if the underlying value is not an rfl::Generic::Array.
  Result<Array> to_array() const noexcept {
    return std::visit(
        [](auto _v) -> Result<Array> {
          using V = std::remove_cvref_t<decltype(_v)>;
          if constexpr (std::is_same_v<V, Array>) {
            return _v;
          } else {
            return error(
                "rfl::Generic: Could not cast the underlying value to an "
                "rfl::Generic::Array.");
          }
        },
        value_);
  }

  /// Casts the underlying value to a boolean or returns an rfl::Error, if the
  /// underlying value is not a boolean.
  Result<bool> to_bool() const noexcept {
    return std::visit(
        [](auto _v) -> Result<bool> {
          using V = std::remove_cvref_t<decltype(_v)>;
          if constexpr (std::is_same_v<V, bool>) {
            return _v;
          } else {
            return error(
                "rfl::Generic: Could not cast the underlying value to a "
                "boolean.");
          }
        },
        value_);
  }

  /// Casts the underlying value to a double or returns an rfl::Error, if the
  /// underlying value is not a double.
  Result<double> to_double() const noexcept {
    return std::visit(
        [](auto _v) -> Result<double> {
          using V = std::remove_cvref_t<decltype(_v)>;
          if constexpr (std::is_same_v<V, double>) {
            return _v;
          } else {
            return error(
                "rfl::Generic: Could not cast the underlying value to a "
                "double.");
          }
        },
        value_);
  }

  /// Casts the underlying value to an integer or returns an rfl::Error, if the
  /// underlying value is not an integer.
  Result<int> to_int() const noexcept {
    return std::visit(
        [](auto _v) -> Result<int> {
          using V = std::remove_cvref_t<decltype(_v)>;
          if constexpr (std::is_same_v<V, int64_t>) {
            return static_cast<int>(_v);
          } else {
            return error(
                "rfl::Generic: Could not cast the underlying value to an "
                "integer.");
          }
        },
        value_);
  }

  /// Casts the underlying value to an int64 or returns an rfl::Error, if the
  /// underlying value is not an integer.
  Result<int64_t> to_int64() const noexcept {
    return std::visit(
        [](auto _v) -> Result<int64_t> {
          using V = std::remove_cvref_t<decltype(_v)>;
          if constexpr (std::is_same_v<V, int64_t>) {
            return _v;
          } else {
            return error(
                "rfl::Generic: Could not cast the underlying value to an "
                "int64.");
          }
        },
        value_);
  }

  /// Casts the underlying value to an rfl::Generic::Object or returns an
  /// rfl::Error, if the underlying value is not an rfl::Generic::Object.
  Result<Object> to_object() const noexcept {
    return std::visit(
        [](auto _v) -> Result<Object> {
          using V = std::remove_cvref_t<decltype(_v)>;
          if constexpr (std::is_same_v<V, Object>) {
            return _v;
          } else {
            return error(
                "rfl::Generic: Could not cast the underlying value to an "
                "rfl::Generic::Object.");
          }
        },
        value_);
  }

  /// Casts the underlying value to rfl::Generic::Null or returns an
  /// rfl::Error, if the underlying value is not rfl::Generic::Null.
  Result<std::nullopt_t> to_null() const noexcept {
    return std::visit(
        [](auto _v) -> Result<std::nullopt_t> {
          using V = std::remove_cvref_t<decltype(_v)>;
          if constexpr (std::is_same_v<V, std::nullopt_t>) {
            return _v;
          } else {
            return error(
                "rfl::Generic: Could not cast the underlying value to "
                "rfl::Generic::Null.");
          }
        },
        value_);
  }

  /// Casts the underlying value to a string or returns an rfl::Error, if the
  /// underlying value is not a string.
  Result<std::string> to_string() const noexcept {
    return std::visit(
        [](auto _v) -> Result<std::string> {
          using V = std::remove_cvref_t<decltype(_v)>;
          if constexpr (std::is_same_v<V, std::string>) {
            return _v;
          } else {
            return error(
                "rfl::Generic: Could not cast the underlying value to a "
                "string.");
          }
        },
        value_);
  }

  /// Returns the underlying variant.
  VariantType& variant() noexcept { return value_; };

  /// Returns the underlying variant.
  const VariantType& variant() const noexcept { return value_; };

 private:
  static VariantType from_reflection_type(const ReflectionType& _r) noexcept;

 private:
  VariantType value_;
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

/// Casts the underlying value to an int64 or returns an rfl::Error, if the
/// underlying value is not an integer.
inline Result<long> to_int64(const Generic& _g) noexcept {
  return _g.to_int64();
}

/// Casts the underlying value to an rfl::Generic::Object or returns an
/// rfl::Error, if the underlying value is not an rfl::Generic::Object.
inline Result<Generic::Object> to_object(const Generic& _g) noexcept {
  return _g.to_object();
}

/// Casts the underlying value to a double or returns an rfl::Error, if the
/// underlying value is not a double.
inline Result<std::nullopt_t> to_null(const Generic& _g) noexcept {
  return _g.to_null();
}

/// Casts the underlying value to a string or returns an rfl::Error, if the
/// underlying value is not a string.
inline Result<std::string> to_string(const Generic& _g) noexcept {
  return _g.to_string();
}

}  // namespace rfl

#endif
