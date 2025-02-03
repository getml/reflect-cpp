#ifndef RFL_AVRO_SCHEMA_TYPE_HPP_
#define RFL_AVRO_SCHEMA_TYPE_HPP_

#include <map>
#include <memory>
#include <optional>
#include <string>

#include "../../Literal.hpp"
#include "../../Object.hpp"
#include "../../Ref.hpp"
#include "../../Rename.hpp"
#include "../../Variant.hpp"

namespace rfl::avro::schema {

struct Type {
  struct Null {
    Literal<"null"> type;
  };

  struct Boolean {
    Literal<"boolean"> type;
  };

  struct Int {
    Literal<"int"> type;
  };

  struct Long {
    Literal<"long"> type;
  };

  struct Float {
    Literal<"float"> type;
  };

  struct Double {
    Literal<"float"> type;
  };

  struct Bytes {
    Literal<"bytes"> type;
  };

  struct String {
    Literal<"string"> type;
  };

  struct RecordField {
    std::string name;
    rfl::Ref<Type> type;
  };

  struct Record {
    Literal<"record"> type;
    std::string name;
    std::vector<RecordField> fields;
    std::optional<std::string> doc;
  };

  struct Enum {
    Literal<"enum"> type;
    std::string name;
    std::vector<std::string> symbols;
  };

  struct Array {
    Literal<"array"> type;
    rfl::Ref<Type> items;
    Rename<"default", std::vector<std::string>> default_;
  };

  struct Map {
    Literal<"map"> type;
    rfl::Ref<Type> values;
    Rename<"default", std::map<std::string, std::string>> default_;
  };

  struct Reference {
    std::string type;
  };

  using ReflectionType =
      rfl::Variant<Null, Boolean, Int, Long, Float, Double, Bytes, String,
                   Record, Enum, Array, Map, Reference, std::vector<Type>>;

  const auto& reflection() const { return value; }

  Type with_name(const std::string& _name) const;

  ReflectionType value;
};

}  // namespace rfl::avro::schema

#endif
