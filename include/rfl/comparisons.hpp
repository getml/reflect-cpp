#ifndef RFL_COMPARISONS_HPP_
#define RFL_COMPARISONS_HPP_

#include <type_traits>

#include "Result.hpp"
#include "parsing/schema/ValidationType.hpp"

namespace rfl {

template <auto _threshold>
struct EqualTo {
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value != threshold) {
      return error("Value expected to be equal to " +
                   std::to_string(threshold) + ", but got " +
                   std::to_string(_value) + ".");
    }
    return _value;
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    const auto value =
        std::is_floating_point_v<T>
            ? rfl::Variant<double, int>(static_cast<double>(_threshold))
            : rfl::Variant<double, int>(static_cast<int>(_threshold));
    return ValidationType{ValidationType::EqualTo{.value_ = value}};
  }
};

template <auto _threshold>
struct Minimum {
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value < threshold) {
      return error("Value expected to be greater than or equal to " +
                   std::to_string(threshold) + ", but got " +
                   std::to_string(_value) + ".");
    }
    return _value;
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    const auto value =
        std::is_floating_point_v<T>
            ? rfl::Variant<double, int>(static_cast<double>(_threshold))
            : rfl::Variant<double, int>(static_cast<int>(_threshold));
    return ValidationType{ValidationType::Minimum{.value_ = value}};
  }
};

template <auto _threshold>
struct ExclusiveMinimum {
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value <= threshold) {
      return error("Value expected to be greater than " +
                   std::to_string(threshold) + ", but got " +
                   std::to_string(_value) + ".");
    }
    return _value;
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    const auto value =
        std::is_floating_point_v<T>
            ? rfl::Variant<double, int>(static_cast<double>(_threshold))
            : rfl::Variant<double, int>(static_cast<int>(_threshold));
    return ValidationType{ValidationType::ExclusiveMinimum{.value_ = value}};
  }
};

template <auto _threshold>
struct Maximum {
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value > threshold) {
      return error("Value expected to be less than or equal to " +
                   std::to_string(threshold) + ", but got " +
                   std::to_string(_value) + ".");
    }
    return _value;
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    const auto value =
        std::is_floating_point_v<T>
            ? rfl::Variant<double, int>(static_cast<double>(_threshold))
            : rfl::Variant<double, int>(static_cast<int>(_threshold));
    return ValidationType{ValidationType::Maximum{.value_ = value}};
  }
};

template <auto _threshold>
struct ExclusiveMaximum {
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value >= threshold) {
      return error("Value expected to be less than " +
                   std::to_string(threshold) + ", but got " +
                   std::to_string(_value) + ".");
    }
    return _value;
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    const auto value =
        std::is_floating_point_v<T>
            ? rfl::Variant<double, int>(static_cast<double>(_threshold))
            : rfl::Variant<double, int>(static_cast<int>(_threshold));
    return ValidationType{ValidationType::ExclusiveMaximum{.value_ = value}};
  }
};

template <auto _threshold>
struct NotEqualTo {
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value == threshold) {
      return error("Value expected to not be equal to " +
                   std::to_string(threshold) + ", but got " +
                   std::to_string(_value) + ".");
    }
    return _value;
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    const auto value =
        std::is_floating_point_v<T>
            ? rfl::Variant<double, int>(static_cast<double>(_threshold))
            : rfl::Variant<double, int>(static_cast<int>(_threshold));
    return ValidationType{ValidationType::NotEqualTo{.value_ = value}};
  }
};

}  // namespace rfl

#endif
