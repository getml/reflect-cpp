#ifndef RFL_JSON_SCHEMA_TYPE_HPP_
#define RFL_JSON_SCHEMA_TYPE_HPP_

#include <map>
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
    std::optional<std::string> description;
    Literal<"boolean"> type;
  };

  struct Integer {
    Literal<"integer"> type;
    std::optional<std::string> description;
  };

  struct Number {
    Literal<"number"> type;
    std::optional<std::string> description;
  };

  struct String {
    Literal<"string"> type;
    std::optional<std::string> description;
    rfl::Rename<"minLength", std::optional<size_t>> minSize;
    rfl::Rename<"maxLength", std::optional<size_t>> maxSize;
  };

  using NumericType = rfl::Variant<Integer, Number>;

  struct AllOf {
    std::optional<std::string> description;
    std::vector<Type> allOf;
  };

  struct AnyOf {
    std::optional<std::string> description;
    std::vector<Type> anyOf;
  };

  struct ExclusiveMaximum {
    std::optional<std::string> description;
    rfl::Variant<double, int> exclusiveMaximum;
    std::string type;
  };

  struct ExclusiveMinimum {
    std::optional<std::string> description;
    rfl::Variant<double, int> exclusiveMinimum;
    std::string type;
  };

  struct FixedSizeTypedArray {
    Literal<"array"> type;
    std::optional<std::string> description;
    rfl::Ref<Type> items;
    size_t minItems;
    size_t maxItems;
  };

  struct Maximum {
    std::optional<std::string> description;
    rfl::Variant<double, int> maximum;
    std::string type;
  };

  struct Minimum {
    std::optional<std::string> description;
    rfl::Variant<double, int> minimum;
    std::string type;
  };

  struct Null {
    Literal<"null"> type;
    std::optional<std::string> description;
  };

  struct Object {
    Literal<"object"> type;
    std::optional<std::string> description;
    rfl::Object<Type> properties;
    std::vector<std::string> required;
    std::shared_ptr<Type> additionalProperties;
  };

  struct OneOf {
    std::optional<std::string> description;
    std::vector<Type> oneOf;
  };

  struct Reference {
    Rename<"$ref", std::optional<std::string>> ref;
    std::optional<std::string> description;
  };

  struct Regex {
    Literal<"string"> type;
    std::optional<std::string> description;
    std::string pattern;
  };

  struct StringEnum {
    Literal<"string"> type;
    std::optional<std::string> description;
    rfl::Rename<"enum", std::vector<std::string>> values;
  };

  struct StringMap {
    Literal<"object"> type;
    std::optional<std::string> description;
    rfl::Ref<Type> additionalProperties;
  };

  struct Tuple {
    Literal<"array"> type;
    std::optional<std::string> description;
    std::vector<Type> prefixItems;
    bool items = false;
  };

  struct TypedArray {
    Literal<"array"> type;
    std::optional<std::string> description;
    rfl::Ref<Type> items;
    rfl::Rename<"minItems", std::optional<size_t>> minSize;
    rfl::Rename<"maxItems", std::optional<size_t>> maxSize;
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
