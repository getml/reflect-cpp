/*

MIT License

Copyright (c) 2023-2024 Code17 GmbH

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "rfl/Generic.hpp"

namespace rfl {

Generic::Generic() : value_(false) {}

Generic::Generic(Generic&& _other) noexcept = default;

Generic::Generic(const Generic& _other) = default;

Generic::Generic(const VariantType& _value) : value_(_value) {}

Generic::Generic(VariantType&& _value) noexcept : value_(std::move(_value)) {}

Generic::Generic(const ReflectionType& _r) : value_(from_reflection_type(_r)) {}

Generic::~Generic() = default;

Generic::VariantType Generic::from_reflection_type(
    const ReflectionType& _r) noexcept {
  if (!_r) {
    return std::nullopt;
  } else {
    return std::visit([](const auto& _v) -> VariantType { return _v; }, *_r);
  }
}

bool Generic::is_null() const noexcept {
  return std::get_if<std::nullopt_t>(&value_) && true;
}

Generic& Generic::operator=(const VariantType& _value) {
  value_ = _value;
  return *this;
}

Generic& Generic::operator=(VariantType&& _value) noexcept {
  value_ = std::move(_value);
  return *this;
}

Generic& Generic::operator=(const Generic& _other) = default;

Generic& Generic::operator=(Generic&& _other) = default;

Generic::ReflectionType Generic::reflection() const noexcept {
  return std::visit([](const auto& _v) -> ReflectionType { return _v; },
                    value_);
}

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
  if (const bool* ptr = std::get_if<bool>(&value_)) {
    return *ptr;
  } else {
    return Error(
        "rfl::Generic: Could not cast the underlying value to a boolean.");
  }
}

Result<double> Generic::to_double() const noexcept {
  if (const double* ptr = std::get_if<double>(&value_)) {
    return *ptr;
  } else {
    return Error(
        "rfl::Generic: Could not cast the underlying value to a double.");
  }
}

Result<int> Generic::to_int() const noexcept {
  if (const int* ptr = std::get_if<int>(&value_)) {
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

Result<std::nullopt_t> Generic::to_null() const noexcept {
  if (const auto* ptr = std::get_if<std::nullopt_t>(&value_)) {
    return *ptr;
  } else {
    return Error(
        "rfl::Generic: Could not cast the underlying value to "
        "rfl::Generic::Null.");
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
