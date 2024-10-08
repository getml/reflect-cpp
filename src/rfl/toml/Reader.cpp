#include "rfl/toml/Reader.hpp"

namespace rfl::toml {

rfl::Result<Reader::InputVarType> Reader::get_field_from_array(
    const size_t _idx, const InputArrayType _arr) const noexcept {
  if (_idx >= _arr->size()) {
    return rfl::Error("Index " + std::to_string(_idx) + " of of bounds.");
  }
  return _arr->get(_idx);
}

rfl::Result<Reader::InputVarType> Reader::get_field_from_object(
    const std::string& _name, const InputObjectType& _obj) const noexcept {
  auto var = (*_obj)[_name];
  if (!var) {
    return rfl::Error("Object contains no field named '" + _name + "'.");
  }
  return var.node();
}

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return !_var && true;
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  const auto ptr = _var->as_array();
  if (!ptr) {
    return rfl::Error("Could not cast to an array!");
  }
  return ptr;
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  const auto ptr = _var->as_table();
  if (!ptr) {
    return rfl::Error("Could not cast to a table!");
  }
  return ptr;
}

}  // namespace rfl::toml
