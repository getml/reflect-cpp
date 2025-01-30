#ifndef RFL_CAPNPROTO_SCHEMA_CAPNPROTOTYPES_HPP_
#define RFL_CAPNPROTO_SCHEMA_CAPNPROTOTYPES_HPP_

#include <map>

#include "Type.hpp"

namespace rfl::capnproto::schema {

struct CapnProtoTypes {
  bool has_maps_ = false;
  std::map<std::string, schema::Type> structs_;
  std::map<std::string, schema::Type> enums_;
  std::map<std::string, schema::Type> tuples_;
  std::map<std::string, schema::Type> unions_;
};

const char* MAP_DEFINITION = R"(
struct Map(Value) {
  entries @0 :List(Entry);
  struct Entry {
    key @0 :Text;
    value @1 :Value;
  }
})";

std::ostream& operator<<(std::ostream& _os,
                         const std::map<std::string, schema::Type>& _map) {
  for (const auto& [name, type] : _map) {
    _os << type.with_name(internal::strings::to_pascal_case(name)) << std::endl
        << std::endl;
  }
  return _os;
}

std::ostream& operator<<(std::ostream& _os, const CapnProtoTypes& _cnp_types) {
  // TODO: ID is hardcoded.
  _os << "@0xdbb9ad1f14bf0b36;" << std::endl << std::endl;
  if (_cnp_types.has_maps_) {
    _os << MAP_DEFINITION << std::endl << std::endl;
  }
  _os << _cnp_types.structs_ << _cnp_types.enums_ << _cnp_types.tuples_
      << _cnp_types.unions_;
  return _os;
}

}  // namespace rfl::capnproto::schema

#endif
