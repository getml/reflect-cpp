#include "rfl/Generic.hpp"

namespace rfl {

Generic::Generic() : value_(false) {}

Generic::Generic(Generic&& _other) noexcept = default;

Generic::Generic(const Generic& _other) = default;

Generic::Generic(const ReflectionType& _value) : value_(_value) {}

Generic::Generic(ReflectionType&& _value) noexcept
    : value_(std::move(_value)) {}

Generic::~Generic() = default;

Generic& Generic::operator=(const ReflectionType& _value) {
  value_ = _value;
  return *this;
}

Generic& Generic::operator=(ReflectionType&& _value) noexcept {
  value_ = std::move(_value);
  return *this;
}

Generic& Generic::operator=(const Generic& _other) = default;

Generic& Generic::operator=(Generic&& _other) = default;

Result<Generic::Array> Generic::to_array() const noexcept {
  if (const auto* ptr = std::get_if<Array>(&value_)) {
    return *ptr;
  } else {
    return Error(
        "rfl::Generic: Could not cast the underlying value to an "
        "rfl::Generic::Array.");
  }
}

Result<bool> Generic::to_bool() const noexcept {
  if (const auto* ptr = std::get_if<bool>(&value_)) {
    return *ptr;
  } else {
    return Error(
        "rfl::Generic: Could not cast the underlying value to a boolean.");
  }
}

Result<double> Generic::to_double() const noexcept {
  if (const auto* ptr = std::get_if<double>(&value_)) {
    return *ptr;
  } else {
    return Error(
        "rfl::Generic: Could not cast the underlying value to a double.");
  }
}

Result<int> Generic::to_int() const noexcept {
  if (const auto* ptr = std::get_if<int>(&value_)) {
    return *ptr;
  } else {
    return Error(
        "rfl::Generic: Could not cast the underlying value to an integer.");
  }
}

Result<Generic::Object> Generic::to_object() const noexcept {
  if (const auto* ptr = std::get_if<Object>(&value_)) {
    return *ptr;
  } else {
    return Error(
        "rfl::Generic: Could not cast the underlying value to an "
        "rfl::Generic::Object.");
  }
}

Result<std::string> Generic::to_string() const noexcept {
  if (const auto* ptr = std::get_if<std::string>(&value_)) {
    return *ptr;
  } else {
    return Error(
        "rfl::Generic: Could not cast the underlying value to a string.");
  }
}

}  // namespace rfl
