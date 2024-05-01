#ifndef RFL_JSON_TOSCHEMA_HPP_
#define RFL_JSON_TOSCHEMA_HPP_

#include <yyjson.h>

#include <map>
#include <string>
#include <type_traits>
#include <variant>

#include "../Literal.hpp"
#include "../Processors.hpp"
#include "../parsing/schema/Type.hpp"
#include "../parsing/schema/ValidationType.hpp"
#include "../parsing/schema/make.hpp"
#include "Reader.hpp"
#include "Writer.hpp"
#include "schema/JSONSchema.hpp"
#include "schema/Type.hpp"
#include "write.hpp"

namespace rfl::json {

inline schema::Type type_to_json_schema_type(
    const parsing::schema::Type& _type);

inline bool is_optional(const parsing::schema::Type& _t) {
  const auto handle = [](const auto& _v) -> bool {
    using T = std::remove_cvref_t<decltype(_v)>;
    return std::is_same<T, parsing::schema::Type::Optional>();
  };
  return std::visit(handle, _t.variant_);
}

inline std::string numeric_type_to_string(const parsing::schema::Type& _type) {
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
  return std::visit(handle_variant, _type.variant_);
}

inline schema::Type handle_validation_type(
    const parsing::schema::Type& _type,
    const parsing::schema::ValidationType& _validation_type) {
  const auto handle_variant = [&](const auto& _v) -> schema::Type {
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
      return type_to_json_schema_type(_type);

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

  return std::visit(handle_variant, _validation_type.variant_);
}

inline schema::Type type_to_json_schema_type(
    const parsing::schema::Type& _type) {
  const auto handle_variant = [](const auto& _t) -> schema::Type {
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
      return std::visit(update_prediction, res.value);

    } else if constexpr (std::is_same<T, Type::FixedSizeTypedArray>()) {
      return schema::Type{.value = schema::Type::FixedSizeTypedArray{
                              .items = Ref<schema::Type>::make(
                                  type_to_json_schema_type(*_t.type_)),
                              .minContains = _t.size_,
                              .maxContains = _t.size_}};

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
      return schema::Type{.value = schema::Type::Object{
                              .properties = properties, .required = required}};

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

  return std::visit(handle_variant, _type.variant_);
}

template <class T>
struct TypeHelper {};

template <class... Ts>
struct TypeHelper<std::variant<Ts...>> {
  using JSONSchemaType = std::variant<schema::JSONSchema<Ts>...>;
};

/// Returns the JSON schema for a class.
template <class T, class... Ps>
std::string to_schema(const yyjson_write_flag _flag = 0) {
  const auto internal_schema =
      parsing::schema::make<Reader, Writer, T, Processors<Ps...>>();
  auto definitions = std::map<std::string, schema::Type>();
  for (const auto& [k, v] : internal_schema.definitions_) {
    definitions[k] = type_to_json_schema_type(v);
  }
  using JSONSchemaType =
      typename TypeHelper<schema::Type::ReflectionType>::JSONSchemaType;
  const auto to_schema = [&](auto&& _root) -> JSONSchemaType {
    using U = std::decay_t<decltype(_root)>;
    return schema::JSONSchema<U>{.root = std::move(_root),
                                 .definitions = definitions};
  };
  auto root = type_to_json_schema_type(internal_schema.root_);
  const auto json_schema = std::visit(to_schema, std::move(root.value));
  return write(json_schema, _flag);
}
}  // namespace rfl::json

#endif
