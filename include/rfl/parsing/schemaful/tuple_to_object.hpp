#ifndef RFL_PARSING_SCHEMAFUL_TUPLETOOBJECT_HPP_
#define RFL_PARSING_SCHEMAFUL_TUPLETOOBJECT_HPP_

#include <string>

#include "../schema/Type.hpp"

namespace rfl::parsing::schemaful {

std::string to_field_name(const size_t _i) {
  return std::string({'f', static_cast<char>('0' + ((_i / 100) % 10)),
                      static_cast<char>('0' + ((_i / 10) % 10)),
                      static_cast<char>('0' + (_i % 10))});
}

/// Schemaful formats often don't have an explicit tuple representation.
/// This is the required workaround.
schema::Type::Object tuple_to_object(const schema::Type::Tuple& _tup) {
  auto obj = schema::Type::Object{};
  for (size_t i = 0; i < _tup.types_.size(); ++i) {
    obj.types_[to_field_name(i)] = _tup.types_[i];
  }
  return obj;
}

}  // namespace rfl::parsing::schemaful

#endif
