#include "rfl/flexbuf/Reader.hpp"

namespace rfl::flexbuf {

rfl::Result<Reader::InputVarType> Reader::get_field_from_array(
    const size_t _idx, const InputArrayType& _arr) const noexcept {
  if (_idx >= _arr.size()) {
    return rfl::Error("Index " + std::to_string(_idx) + " of of bounds.");
  }
  return _arr[_idx];
}

rfl::Result<Reader::InputVarType> Reader::get_field_from_object(
    const std::string& _name, const InputObjectType& _obj) const noexcept {
  const auto keys = _obj.Keys();
  for (size_t i = 0; i < keys.size(); ++i) {
    if (_name == keys[i].AsString().c_str()) {
      return _obj.Values()[i];
    }
  }
  return rfl::Error("Map does not contain any element called '" + _name + "'.");
}

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return _var.IsNull();
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  if (!_var.IsVector()) {
    return rfl::Error("Could not cast to Vector.");
  }
  return _var.AsVector();
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  if (!_var.IsMap()) {
    return rfl::Error("Could not cast to Map!");
  }
  return _var.AsMap();
}

}  // namespace rfl::flexbuf
