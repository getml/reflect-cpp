#ifndef RFL_COMPARISONS_HPP_
#define RFL_COMPARISONS_HPP_

#include "Result.hpp"

namespace rfl {

template <auto _threshold>
struct EqualTo {
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value != threshold) {
      return Error("Value expected to be equal to " +
                   std::to_string(threshold) + ", but got " +
                   std::to_string(_value) + ".");
    }
    return _value;
  }
};

template <auto _threshold>
struct Minimum {
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value < threshold) {
      return Error("Value expected to be greater than or equal to " +
                   std::to_string(threshold) + ", but got " +
                   std::to_string(_value) + ".");
    }
    return _value;
  }
};

template <auto _threshold>
struct ExclusiveMinimum {
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value <= threshold) {
      return Error("Value expected to be greater than " +
                   std::to_string(threshold) + ", but got " +
                   std::to_string(_value) + ".");
    }
    return _value;
  }
};

template <auto _threshold>
struct Maximum {
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value > threshold) {
      return Error("Value expected to be less than or equal to " +
                   std::to_string(threshold) + ", but got " +
                   std::to_string(_value) + ".");
    }
    return _value;
  }
};

template <auto _threshold>
struct ExclusiveMaximum {
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value >= threshold) {
      return Error("Value expected to be less than " +
                   std::to_string(threshold) + ", but got " +
                   std::to_string(_value) + ".");
    }
    return _value;
  }
};

template <auto _threshold>
struct NotEqualTo {
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value == threshold) {
      return Error("Value expected to not be equal to " +
                   std::to_string(threshold) + ", but got " +
                   std::to_string(_value) + ".");
    }
    return _value;
  }
};

}  // namespace rfl

#endif
