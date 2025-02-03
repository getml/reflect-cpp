#ifndef RFL_CAPNPROTO_SCHEMA_TYPE_HPP_
#define RFL_CAPNPROTO_SCHEMA_TYPE_HPP_

#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "../../Literal.hpp"
#include "../../Object.hpp"
#include "../../Ref.hpp"
#include "../../Rename.hpp"
#include "../../Variant.hpp"

namespace rfl::capnproto::schema {

struct Type {
  struct Void {};

  struct Bool {};

  struct Int8 {};

  struct Int16 {};

  struct Int32 {};

  struct Int64 {};

  struct UInt8 {};

  struct UInt16 {};

  struct UInt32 {};

  struct UInt64 {};

  struct Float32 {};

  struct Float64 {};

  struct Data {};

  struct Text {};

  struct Enum {
    std::string name;
    std::vector<std::string> fields;
  };

  struct List {
    rfl::Ref<Type> type;
  };

  struct Map {
    rfl::Ref<Type> type;
  };

  struct Reference {
    std::string type_name;
  };

  struct Struct {
    std::string name;
    std::vector<std::pair<std::string, Type>> fields;
  };

  struct Union {
    std::string name;
    std::vector<std::pair<std::string, Type>> fields;
  };

  using ReflectionType =
      rfl::Variant<Void, Bool, Int8, Int16, Int32, Int64, UInt8, UInt16, UInt32,
                   UInt64, Float32, Float64, Data, Enum, List, Map, Reference,
                   Struct, Text, Union>;

  const auto& reflection() const { return value; }

  Type with_name(const std::string& _name) const;

  ReflectionType value;
};

std::ostream& operator<<(std::ostream& os, const Type& _t);

}  // namespace rfl::capnproto::schema
#endif
