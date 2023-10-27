#ifndef RFL_VALIDATOR_HPP_
#define RFL_VALIDATOR_HPP_

#include <concepts>
#include <optional>
#include <regex>
#include <string>
#include <type_traits>

#include "internal/StringLiteral.hpp"
#include "rfl/Result.hpp"

namespace rfl {

template <class Class, typename Type>
concept HasValidation = requires(Class obj, Type value) {
  { Class::validate(value) } -> std::same_as<std::optional<rfl::Error>>;
};

template <typename _Type>
concept HasDescription = requires(_Type obj) {
  { obj.description } -> std::same_as<std::optional<rfl::Error>>;
};


template <typename _Type, class Class>
requires HasValidation<Class, _Type>
struct Annotate {
 public:
  using ReflectionType = _Type;
  ReflectionType value;

  template <class T, 
  std::enable_if_t<std::is_convertible_v<T, _Type>, bool> = true > 
  Annotate(const T& _value) : value(_value) {
    std::optional<rfl::Error> validate_check = Class::validate(value);
    if (validate_check.has_value()) {
      throw std::runtime_error(validate_check.value().what());
    }
  };

  template <class T, 
  std::enable_if_t<std::is_convertible_v<T, _Type>, bool> = true > 
  Annotate(T&& _value) : value(std::forward<T>(_value)) {
    std::optional<rfl::Error> validate_check = Class::validate(value);
    if (validate_check.has_value()) {
      throw std::runtime_error(validate_check.value().what());
    }
  }

  Annotate(ReflectionType _value) : value(_value) {
    std::optional<rfl::Error> validate_check = Class::validate(value);
    if (validate_check.has_value()) {
      throw std::runtime_error(validate_check.value().what());
    }
  }
  
};

template <typename T>
std::string to_string(const T& _value) {
  std::ostringstream oss;
  oss << _value;
  return oss.str();
}

template <typename T>
concept HasSize = requires(T obj) {
  { obj.size() } -> std::same_as<size_t>;
};

template <typename T>
concept HasIntLength = HasSize<T> || std::is_integral_v<T>;

template <size_t _min_length, size_t _max_length>
struct Length {
  static constexpr const auto min_length = _min_length;
  static constexpr const auto max_length = _max_length;

  template <HasIntLength T>
  static std::optional<rfl::Error> validate(T _value) {
    uint16_t size;
    if constexpr (HasSize<T>) {
      size = _value.size();
    } else if (std::is_integral_v<T>) {
      size = _value;
    }
    if (size < _min_length) {
      return rfl::Error("'" + to_string(_value) +
                        "' is less than the min length requirement of '" +
                        std::to_string(_min_length) + "'");
    }
    if (size > _max_length) {
      return rfl::Error("'" + to_string(_value) +
                        "' exceeds max length requirement of '" +
                        std::to_string(_max_length) + "'");
    }
    return std::nullopt;
  }
};

template <internal::StringLiteral _pattern>
struct Pattern {
  static constexpr const auto pattern = _pattern;

  static std::optional<rfl::Error> validate(std::string _value) {
    std::regex regex_pattern(_pattern.str());
    if (!std::regex_match(_value, regex_pattern)) {
      return rfl::Error("String '" + _value + "' did not match format '" +
                        _pattern.str() + "'");
    }
    return std::nullopt;
  }
};

}  // namespace rfl
//
#endif
