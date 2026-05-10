#ifndef RFL_COMPARISONS_HPP_
#define RFL_COMPARISONS_HPP_

#include <sstream>
#include <type_traits>

#include "Result.hpp"
#include "parsing/schema/ValidationType.hpp"

namespace rfl {

template <auto _threshold>
struct EqualTo {
  /// @brief Validates that the given value is equal to the threshold.
  /// @tparam T The type of the value to validate.
  /// @param _value The value to validate.
  /// @return Result<T> containing the value if valid, or an error if not.
  /// @details This function checks if the input value is exactly equal to the
  /// compile-time threshold. If not, it returns an error with a descriptive
  /// message.
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value != threshold) {
      std::stringstream stream;
      stream << "Value expected to be equal to " << threshold << ", but got "
             << _value << ".";
      return error(stream.str());
    }
    return _value;
  }

  /// @brief Converts the validation rule to a schema representation.
  /// @tparam T The type for which the schema is generated.
  /// @return parsing::schema::ValidationType representing the EqualTo
  /// constraint.
  /// @details This function creates a schema object that encodes the equality
  /// constraint for serialization or documentation purposes.
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
  /// @brief Validates that the given value is greater than or equal to the
  /// threshold.
  /// @tparam T The type of the value to validate.
  /// @param _value The value to validate.
  /// @return Result<T> containing the value if valid, or an error if not.
  /// @details This function checks if the input value is at least the
  /// compile-time threshold. If not, it returns an error with a descriptive
  /// message.
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value < threshold) {
      std::stringstream stream;
      stream << "Value expected to be greater than or equal to " << threshold
             << ", but got " << _value << ".";
      return error(stream.str());
    }
    return _value;
  }

  /// @brief Converts the validation rule to a schema representation.
  /// @tparam T The type for which the schema is generated.
  /// @return parsing::schema::ValidationType representing the Minimum
  /// constraint.
  /// @details This function creates a schema object that encodes the minimum
  /// value constraint for serialization or documentation purposes.
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
  /// @brief Validates that the given value is strictly greater than the
  /// threshold.
  /// @tparam T The type of the value to validate.
  /// @param _value The value to validate.
  /// @return Result<T> containing the value if valid, or an error if not.
  /// @details This function checks if the input value is strictly greater than
  /// the compile-time threshold. If not, it returns an error with a descriptive
  /// message.
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value <= threshold) {
      std::stringstream stream;
      stream << "Value expected to be greater than " << threshold
             << ", but got " << _value << ".";
      return error(stream.str());
    }
    return _value;
  }

  /// @brief Converts the validation rule to a schema representation.
  /// @tparam T The type for which the schema is generated.
  /// @return parsing::schema::ValidationType representing the ExclusiveMinimum
  /// constraint.
  /// @details This function creates a schema object that encodes the exclusive
  /// minimum value constraint for serialization or documentation purposes.
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
  /// @brief Validates that the given value is less than or equal to the
  /// threshold.
  /// @tparam T The type of the value to validate.
  /// @param _value The value to validate.
  /// @return Result<T> containing the value if valid, or an error if not.
  /// @details This function checks if the input value is at most the
  /// compile-time threshold. If not, it returns an error with a descriptive
  /// message.
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value > threshold) {
      std::stringstream stream;
      stream << "Value expected to be less than or equal to " << threshold
             << ", but got " << _value << ".";
      return error(stream.str());
    }
    return _value;
  }

  /// @brief Converts the validation rule to a schema representation.
  /// @tparam T The type for which the schema is generated.
  /// @return parsing::schema::ValidationType representing the Maximum
  /// constraint.
  /// @details This function creates a schema object that encodes the maximum
  /// value constraint for serialization or documentation purposes.
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
  /// @brief Validates that the given value is strictly less than the threshold.
  /// @tparam T The type of the value to validate.
  /// @param _value The value to validate.
  /// @return Result<T> containing the value if valid, or an error if not.
  /// @details This function checks if the input value is strictly less than the
  /// compile-time threshold. If not, it returns an error with a descriptive
  /// message.
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value >= threshold) {
      std::stringstream stream;
      stream << "Value expected to be less than " << threshold << ", but got "
             << _value << ".";
      return error(stream.str());
    }
    return _value;
  }

  /// @brief Converts the validation rule to a schema representation.
  /// @tparam T The type for which the schema is generated.
  /// @return parsing::schema::ValidationType representing the ExclusiveMaximum
  /// constraint.
  /// @details This function creates a schema object that encodes the exclusive
  /// maximum value constraint for serialization or documentation purposes.
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
  /// @brief Validates that the given value is not equal to the threshold.
  /// @tparam T The type of the value to validate.
  /// @param _value The value to validate.
  /// @return Result<T> containing the value if valid, or an error if not.
  /// @details This function checks if the input value is not equal to the
  /// compile-time threshold. If it is, it returns an error with a descriptive
  /// message.
  template <class T>
  static Result<T> validate(T _value) noexcept {
    constexpr auto threshold = static_cast<T>(_threshold);
    if (_value == threshold) {
      std::stringstream stream;
      stream << "Value expected not to be equal to " << threshold
             << ", but got " << _value << ".";
      return error(stream.str());
    }
    return _value;
  }

  /// @brief Converts the validation rule to a schema representation.
  /// @tparam T The type for which the schema is generated.
  /// @return parsing::schema::ValidationType representing the NotEqualTo
  /// constraint.
  /// @details This function creates a schema object that encodes the
  /// not-equal-to constraint for serialization or documentation purposes.
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
