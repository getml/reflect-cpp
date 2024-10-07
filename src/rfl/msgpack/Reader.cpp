#include "rfl/msgpack/Reader.hpp"

namespace rfl::msgpack {

rfl::Result<Reader::InputVarType> Reader::get_field_from_array(
    const size_t _idx, const InputArrayType _arr) const noexcept {
  if (_idx >= _arr.size) {
    return rfl::Error("Index " + std::to_string(_idx) + " of of bounds.");
  }
  return _arr.ptr[_idx];
}

rfl::Result<Reader::InputVarType> Reader::get_field_from_object(
    const std::string& _name, const InputObjectType& _obj) const noexcept {
  for (uint32_t i = 0; i < _obj.size; ++i) {
    const auto& key = _obj.ptr[i].key;
    if (key.type != MSGPACK_OBJECT_STR) {
      return Error("Key in element " + std::to_string(i) +
                   " was not a string.");
    }
    const auto current_name =
        std::string_view(key.via.str.ptr, key.via.str.size);
    if (_name == current_name) {
      return _obj.ptr[i].val;
    }
  }
  return Error("No field named '" + _name + "' was found.");
}

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return _var.type == MSGPACK_OBJECT_NIL;
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  if (_var.type != MSGPACK_OBJECT_ARRAY) {
    return Error("Could not cast to an array.");
  }
  return _var.via.array;
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  if (_var.type != MSGPACK_OBJECT_MAP) {
    return Error("Could not cast to a map.");
  }
  return _var.via.map;
}

}  // namespace rfl::msgpack
