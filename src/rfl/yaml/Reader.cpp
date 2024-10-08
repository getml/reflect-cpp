#include "rfl/yaml/Reader.hpp"

namespace rfl::yaml {

rfl::Result<Reader::InputVarType> Reader::get_field_from_array(
    const size_t _idx, const InputArrayType& _arr) const noexcept {
  if (_idx >= _arr.node_.size()) {
    return rfl::Error("Index " + std::to_string(_idx) + " of of bounds.");
  }
  return InputVarType(_arr.node_[_idx]);
}

rfl::Result<Reader::InputVarType> Reader::get_field_from_object(
    const std::string& _name, const InputObjectType& _obj) const noexcept {
  auto var = InputVarType(_obj.node_[_name]);
  if (!var.node_) {
    return rfl::Error("Object contains no field named '" + _name + "'.");
  }
  return var;
}

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return !_var.node_ && true;
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  if (!_var.node_.IsSequence()) {
    return rfl::Error("Could not cast to sequence!");
  }
  return InputArrayType(_var.node_);
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  if (!_var.node_.IsMap()) {
    return rfl::Error("Could not cast to map!");
  }
  return InputObjectType(_var.node_);
}

}  // namespace rfl::yaml
