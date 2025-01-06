#include "rfl/cbor/Reader.hpp"

namespace rfl::cbor {

rfl::Result<Reader::InputVarType> Reader::get_field_from_array(
    const size_t _idx, const InputArrayType& _arr) const noexcept {
  if (_idx >= _arr.val_->size()) {
    return Error("Index out of range.");
  }
  return InputVarType{&_arr.val_->at(_idx)};
}

rfl::Result<Reader::InputVarType> Reader::get_field_from_object(
    const std::string& _name, const InputObjectType& _obj) const noexcept {
  for (auto& kv : _obj.val_->object_range()) {
    if (kv.key() == _name) {
      return InputVarType{&kv.value()};
    };
  }
  return Error("Field name '" + _name + "' not found.");
}

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return _var.val_->is_null();
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  if (!_var.val_->is_array()) {
    return Error("Could not cast to an array.");
  }
  return InputArrayType{_var.val_};
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  if (!_var.val_->is_object()) {
    return Error("Could not cast to an object.");
  }
  return InputObjectType{_var.val_};
}

}  // namespace rfl::cbor
