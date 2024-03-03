#ifndef RFL_JSON_SCHEMA_TYPE_HPP_
#define RFL_JSON_SCHEMA_TYPE_HPP_

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>

#include "../../Literal.hpp"
#include "../../Rename.hpp"

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
  };

  using NumericType = std::variant<Integer, Number>;

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
    std::variant<double, int> exclusiveMaximum;
    std::string type;
  };

  struct ExclusiveMinimum {
    std::optional<std::string> description;
    std::variant<double, int> exclusiveMinimum;
    std::string type;
  };

  struct FixedSizeTypedArray {
    Literal<"array"> type;
    std::optional<std::string> description;
    rfl::Ref<Type> items;
    size_t minContains;
    size_t maxContains;
  };

  struct Maximum {
    std::optional<std::string> description;
    std::variant<double, int> maximum;
    std::string type;
  };

  struct Minimum {
    std::optional<std::string> description;
    std::variant<double, int> minimum;
    std::string type;
  };

  struct Null {
    Literal<"null"> type;
    std::optional<std::string> description;
  };

  struct Object {
    Literal<"object"> type;
    std::optional<std::string> description;
    std::map<std::string, Type> properties;
    std::vector<std::string> required;
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
  };

  using ReflectionType =
      std::variant<AllOf, AnyOf, Boolean, ExclusiveMaximum, ExclusiveMinimum,
                   FixedSizeTypedArray, Integer, Maximum, Minimum, Number, Null,
                   Object, OneOf, Reference, Regex, String, StringEnum,
                   StringMap, Tuple, TypedArray>;

  const auto& reflection() const { return value; }

  ReflectionType value;
};

}  // namespace rfl::json::schema

#endif
