#ifndef RFL_JSON_SCHEMA_TYPE_HPP_
#define RFL_JSON_SCHEMA_TYPE_HPP_

#include <memory>
#include <optional>
#include <string>

#include "../../Literal.hpp"
#include "../../Object.hpp"
#include "../../Rename.hpp"
#include "../../Variant.hpp"

namespace rfl::json::schema {

/// The JSON representation of internal::schema::Type.
struct Type {
  struct Boolean {
    std::optional<std::string> description = std::optional<std::string>{};
    Literal<"boolean"> type = Literal<"boolean">{};
  };

  struct Integer {
    Literal<"integer"> type = Literal<"integer">{};
    std::optional<std::string> description = std::optional<std::string>{};
  };

  struct Number {
    Literal<"number"> type = Literal<"number">{};
    std::optional<std::string> description = std::optional<std::string>{};
  };

  struct String {
    Literal<"string"> type = Literal<"string">{};
    std::optional<std::string> description = std::optional<std::string>{};
    rfl::Rename<"minLength", std::optional<size_t>> minSize;
    rfl::Rename<"maxLength", std::optional<size_t>> maxSize;
  };

  using NumericType = rfl::Variant<Integer, Number>;

  struct AllOf {
    std::optional<std::string> description = std::optional<std::string>{};
    std::vector<Type> allOf;
  };

  struct AnyOf {
    std::optional<std::string> description = std::optional<std::string>{};
    std::vector<Type> anyOf;
  };

  struct ExclusiveMaximum {
    std::optional<std::string> description = std::optional<std::string>{};
    rfl::Variant<double, int> exclusiveMaximum;
    std::string type;
  };

  struct ExclusiveMinimum {
    std::optional<std::string> description = std::optional<std::string>{};
    rfl::Variant<double, int> exclusiveMinimum;
    std::string type;
  };

  struct FixedSizeTypedArray {
    Literal<"array"> type = Literal<"array">{};
    std::optional<std::string> description = std::optional<std::string>{};
    rfl::Ref<Type> items;
    size_t minItems;
    size_t maxItems;
  };

  struct Maximum {
    std::optional<std::string> description = std::optional<std::string>{};
    rfl::Variant<double, int> maximum;
    std::string type;
  };

  struct Minimum {
    std::optional<std::string> description = std::optional<std::string>{};
    rfl::Variant<double, int> minimum;
    std::string type;
  };

  struct Null {
    Literal<"null"> type = Literal<"null">{};
    std::optional<std::string> description = std::optional<std::string>{};
  };

  struct Object {
    Literal<"object"> type = Literal<"object">{};
    std::optional<std::string> description = std::optional<std::string>{};
    rfl::Object<Type> properties;
    std::vector<std::string> required;
    std::shared_ptr<Type> additionalProperties;
  };

  struct OneOf {
    std::optional<std::string> description = std::optional<std::string>{};
    std::vector<Type> oneOf;
  };

  struct Reference {
    Rename<"$ref", std::optional<std::string>> ref;
    std::optional<std::string> description = std::optional<std::string>{};
  };

  struct Regex {
    Literal<"string"> type = Literal<"string">{};
    std::optional<std::string> description = std::optional<std::string>{};
    std::string pattern;
  };

  struct StringEnum {
    Literal<"string"> type = Literal<"string">{};
    std::optional<std::string> description = std::optional<std::string>{};
    rfl::Rename<"enum", std::vector<std::string>> values;
  };

  struct StringMap {
    Literal<"object"> type = Literal<"object">{};
    std::optional<std::string> description = std::optional<std::string>{};
    rfl::Ref<Type> additionalProperties;
  };

  struct Tuple {
    Literal<"array"> type = Literal<"array">{};
    std::optional<std::string> description = std::optional<std::string>{};
    std::vector<Type> prefixItems;
    bool items = false;
  };

  struct TypedArray {
    Literal<"array"> type =Literal<"array">{};
    std::optional<std::string> description = std::optional<std::string>{};
    rfl::Ref<Type> items;
    rfl::Rename<"minItems", std::optional<size_t>> minSize = rfl::Rename<"minItems", std::optional<size_t>>{};
    rfl::Rename<"maxItems", std::optional<size_t>> maxSize = std::optional<size_t>{};
  };

  using ReflectionType =
      rfl::Variant<AllOf, AnyOf, Boolean, ExclusiveMaximum, ExclusiveMinimum,
                   FixedSizeTypedArray, Integer, Maximum, Minimum, Number, Null,
                   Object, OneOf, Reference, Regex, String, StringEnum,
                   StringMap, Tuple, TypedArray>;

  const auto& reflection() const { return value; }

  ReflectionType value;
};

}  // namespace rfl::json::schema

#endif
