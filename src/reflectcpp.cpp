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

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

#include "rfl/json/to_schema.hpp"

namespace rfl::json {

schema::Type type_to_json_schema_type(const parsing::schema::Type& _type);

bool is_optional(const parsing::schema::Type& _t) {
  const auto handle = [](const auto& _v) -> bool {
    using T = std::remove_cvref_t<decltype(_v)>;
    return std::is_same<T, parsing::schema::Type::Optional>();
  };
  return rfl::visit(handle, _t.variant_);
}

std::string numeric_type_to_string(const parsing::schema::Type& _type) {
  const auto handle_variant = [](const auto& _t) -> std::string {
    using T = std::remove_cvref_t<decltype(_t)>;
    using Type = parsing::schema::Type;
    if constexpr (std::is_same<T, Type::Int32>() ||
                  std::is_same<T, Type::Int64>() ||
                  std::is_same<T, Type::UInt32>() ||
                  std::is_same<T, Type::UInt64>() ||
                  std::is_same<T, Type::Integer>()) {
      return schema::Type::Integer{}.type.str();
    } else {
      return schema::Type::Number{}.type.str();
    }
  };
  return rfl::visit(handle_variant, _type.variant_);
}

schema::Type handle_validation_type(
    const parsing::schema::Type& _type,
    const parsing::schema::ValidationType& _validation_type) {
  auto handle_variant = [&](const auto& _v) -> schema::Type {
    using T = std::remove_cvref_t<decltype(_v)>;
    using ValidationType = parsing::schema::ValidationType;
    if constexpr (std::is_same<T, ValidationType::AllOf>()) {
      auto all_of = std::vector<schema::Type>();
      for (const auto& t : _v.types_) {
        all_of.emplace_back(handle_validation_type(_type, t));
      }
      return schema::Type{.value = schema::Type::AllOf{.allOf = all_of}};

    } else if constexpr (std::is_same<T, ValidationType::AnyOf>()) {
      auto any_of = std::vector<schema::Type>();
      for (const auto& t : _v.types_) {
        any_of.emplace_back(handle_validation_type(_type, t));
      }
      return schema::Type{.value = schema::Type::AnyOf{.anyOf = any_of}};

    } else if constexpr (std::is_same<T, ValidationType::OneOf>()) {
      auto one_of = std::vector<schema::Type>();
      for (const auto& t : _v.types_) {
        one_of.emplace_back(handle_validation_type(_type, t));
      }
      return schema::Type{.value = schema::Type::OneOf{.oneOf = one_of}};

    } else if constexpr (std::is_same<T, ValidationType::Regex>()) {
      return schema::Type{.value = schema::Type::Regex{.pattern = _v.pattern_}};

    } else if constexpr (std::is_same<T, ValidationType::Size>()) {
      auto t = type_to_json_schema_type(_type);
      const auto to_size = [](const auto _v) {
        return static_cast<size_t>(_v);
      };
      auto handle_size_variant = [&](auto& _t, const auto& _size_limit) {
        using U = std::remove_cvref_t<decltype(_t)>;
        using V = std::remove_cvref_t<decltype(_size_limit)>;
        if constexpr (std::is_same<U, schema::Type::TypedArray>() ||
                      std::is_same<U, schema::Type::String>()) {
          if constexpr (std::is_same<V, ValidationType::Minimum>()) {
            _t.minSize = _size_limit.value_.visit(to_size);
            return t;

          } else if constexpr (std::is_same<V, ValidationType::Maximum>()) {
            _t.maxSize = _size_limit.value_.visit(to_size);
            return t;

          } else if constexpr (std::is_same<V, ValidationType::EqualTo>()) {
            _t.minSize = _size_limit.value_.visit(to_size);
            _t.maxSize = _size_limit.value_.visit(to_size);
            return t;

          } else if constexpr (std::is_same<V, ValidationType::AnyOf>() ||
                               std::is_same<V, ValidationType::AllOf>()) {
            V v;
            for (const auto& limiter : _size_limit.types_) {
              v.types_.push_back(ValidationType{ValidationType::Size{
                  .size_limit_ = rfl::Ref<ValidationType>::make(limiter)}});
            }
            return handle_validation_type(_type, ValidationType{.variant_ = v});
          }
        }
        return t;
      };

      return rfl::visit(handle_size_variant, t.value, _v.size_limit_->variant_);

    } else if constexpr (std::is_same<T, ValidationType::ExclusiveMaximum>()) {
      return schema::Type{.value = schema::Type::ExclusiveMaximum{
                              .exclusiveMaximum = _v.value_,
                              .type = numeric_type_to_string(_type)}};

    } else if constexpr (std::is_same<T, ValidationType::ExclusiveMinimum>()) {
      return schema::Type{.value = schema::Type::ExclusiveMinimum{
                              .exclusiveMinimum = _v.value_,
                              .type = numeric_type_to_string(_type)}};

    } else if constexpr (std::is_same<T, ValidationType::Maximum>()) {
      return schema::Type{
          .value = schema::Type::Maximum{
              .maximum = _v.value_, .type = numeric_type_to_string(_type)}};

    } else if constexpr (std::is_same<T, ValidationType::Minimum>()) {
      return schema::Type{
          .value = schema::Type::Minimum{
              .minimum = _v.value_, .type = numeric_type_to_string(_type)}};

    } else if constexpr (std::is_same<T, ValidationType::EqualTo>()) {
      const auto maximum = schema::Type{
          .value = schema::Type::Maximum{
              .maximum = _v.value_, .type = numeric_type_to_string(_type)}};
      const auto minimum = schema::Type{
          .value = schema::Type::Minimum{
              .minimum = _v.value_, .type = numeric_type_to_string(_type)}};
      return schema::Type{.value =
                              schema::Type::AllOf{.allOf = {maximum, minimum}}};

    } else if constexpr (std::is_same<T, ValidationType::NotEqualTo>()) {
      const auto excl_maximum =
          schema::Type{.value = schema::Type::ExclusiveMaximum{
                           .exclusiveMaximum = _v.value_,
                           .type = numeric_type_to_string(_type)}};
      const auto excl_minimum =
          schema::Type{.value = schema::Type::ExclusiveMinimum{
                           .exclusiveMinimum = _v.value_,
                           .type = numeric_type_to_string(_type)}};
      return schema::Type{
          .value = schema::Type::AnyOf{.anyOf = {excl_maximum, excl_minimum}}};

    } else {
      static_assert(rfl::always_false_v<T>, "Not all cases were covered.");
    }
  };

  return rfl::visit(handle_variant, _validation_type.variant_);
}

schema::Type type_to_json_schema_type(const parsing::schema::Type& _type) {
  auto handle_variant = [](const auto& _t) -> schema::Type {
    using T = std::remove_cvref_t<decltype(_t)>;
    using Type = parsing::schema::Type;
    if constexpr (std::is_same<T, Type::Boolean>()) {
      return schema::Type{.value = schema::Type::Boolean{}};

    } else if constexpr (std::is_same<T, Type::Int32>() ||
                         std::is_same<T, Type::Int64>() ||
                         std::is_same<T, Type::UInt32>() ||
                         std::is_same<T, Type::UInt64>() ||
                         std::is_same<T, Type::Integer>()) {
      return schema::Type{.value = schema::Type::Integer{}};

    } else if constexpr (std::is_same<T, Type::Float>() ||
                         std::is_same<T, Type::Double>()) {
      return schema::Type{.value = schema::Type::Number{}};

    } else if constexpr (std::is_same<T, Type::String>()) {
      return schema::Type{.value = schema::Type::String{}};

    } else if constexpr (std::is_same<T, Type::AnyOf>()) {
      auto any_of = std::vector<schema::Type>();
      for (const auto& t : _t.types_) {
        any_of.emplace_back(type_to_json_schema_type(t));
      }
      return schema::Type{.value = schema::Type::AnyOf{.anyOf = any_of}};

    } else if constexpr (std::is_same<T, Type::Description>()) {
      auto res = type_to_json_schema_type(*_t.type_);
      const auto update_prediction = [&](auto _v) -> schema::Type {
        _v.description = _t.description_;
        return schema::Type{_v};
      };
      return rfl::visit(update_prediction, res.value);

    } else if constexpr (std::is_same<T, Type::FixedSizeTypedArray>()) {
      return schema::Type{.value = schema::Type::FixedSizeTypedArray{
                              .items = Ref<schema::Type>::make(
                                  type_to_json_schema_type(*_t.type_)),
                              .minItems = _t.size_,
                              .maxItems = _t.size_}};

    } else if constexpr (std::is_same<T, Type::Literal>()) {
      return schema::Type{.value =
                              schema::Type::StringEnum{.values = _t.values_}};

    } else if constexpr (std::is_same<T, Type::Object>()) {
      auto properties = std::map<std::string, schema::Type>();
      auto required = std::vector<std::string>();
      for (const auto& [k, v] : _t.types_) {
        properties[k] = type_to_json_schema_type(v);
        if (!is_optional(v)) {
          required.push_back(k);
        }
      }
      auto additional_properties =
          _t.additional_properties_
              ? std::make_shared<schema::Type>(
                    type_to_json_schema_type(*_t.additional_properties_))
              : std::shared_ptr<schema::Type>();
      return schema::Type{.value = schema::Type::Object{
                              .properties = properties,
                              .required = required,
                              .additionalProperties = additional_properties}};

    } else if constexpr (std::is_same<T, Type::Optional>()) {
      return schema::Type{.value = schema::Type::AnyOf{
                              .anyOf = {type_to_json_schema_type(*_t.type_),
                                        schema::Type{schema::Type::Null{}}}}};

    } else if constexpr (std::is_same<T, Type::Reference>()) {
      return schema::Type{
          .value = schema::Type::Reference{.ref = "#/definitions/" + _t.name_}};

    } else if constexpr (std::is_same<T, Type::StringMap>()) {
      return schema::Type{.value = schema::Type::StringMap{
                              .additionalProperties = Ref<schema::Type>::make(
                                  type_to_json_schema_type(*_t.value_type_))}};

    } else if constexpr (std::is_same<T, Type::Tuple>()) {
      auto items = std::vector<schema::Type>();
      for (const auto& t : _t.types_) {
        items.emplace_back(type_to_json_schema_type(t));
      }
      return schema::Type{.value = schema::Type::Tuple{.prefixItems = items}};

    } else if constexpr (std::is_same<T, Type::TypedArray>()) {
      return schema::Type{.value = schema::Type::TypedArray{
                              .items = Ref<schema::Type>::make(
                                  type_to_json_schema_type(*_t.type_))}};

    } else if constexpr (std::is_same<T, Type::Validated>()) {
      return handle_validation_type(*_t.type_, _t.validation_);

    } else {
      static_assert(rfl::always_false_v<T>, "Not all cases were covered.");
    }
  };

  return rfl::visit(handle_variant, _type.variant_);
}

}  // namespace rfl::json

// ----------------------------------------------------------------------------

#include "rfl/parsing/schema/Type.hpp"

namespace rfl::parsing::schema {

/// Requires a lot of template instantiation, so we do not want this to be
/// inlined.

Type::Type() : variant_() {}

Type::Type(const VariantType& _variant) : variant_(_variant) {}

Type::~Type() = default;

}  // namespace rfl::parsing::schema

// ----------------------------------------------------------------------------
