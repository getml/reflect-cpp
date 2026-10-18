#include "rfl/ubjson/Reader.hpp"

namespace rfl::ubjson {

rfl::Result<Reader::InputVarType> Reader::get_field_from_array(
    const size_t _idx, const InputArrayType& _arr) const noexcept {
  if (_idx >= _arr.val_->size()) {
    return error("Index out of range.");
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
  return error("Field name '" + _name + "' not found.");
}

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return _var.val_->is_null();
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  if (!_var.val_->is_array()) {
    return error("Could not cast to an array.");
  }
  return InputArrayType{_var.val_};
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  if (!_var.val_->is_object()) {
    return error("Could not cast to an object.");
  }
  return InputObjectType{_var.val_};
}

Result<std::string> Reader::read_string(const InputVarType& _var) const {
  if (!_var.val_->is_string()) return error("Could not cast to string.");
  return _var.val_->as<std::string>();
}

Result<std::vector<uint8_t>> Reader::read_bytes(const InputVarType& _var,
                                                bool _vectorstring) const {
  if (!_var.val_->is<std::vector<uint8_t>>())
    return error(_vectorstring ? "Could not cast to vectorstring."
                               : "Could not cast to bytestring.");
  return _var.val_->as<std::vector<uint8_t>>();
}

Result<bool> Reader::read_bool(const InputVarType& _var) const {
  if (!_var.val_->is_bool()) return error("Could not cast to boolean.");
  return _var.val_->as<bool>();
}

Result<double> Reader::read_double(const InputVarType& _var) const {
  if (!_var.val_->is_double())
    return error("Could not cast to floating point value.");
  return _var.val_->as<double>();
}

bool Reader::is_int64(const InputVarType& _var) const {
  return _var.val_->is_int64();
}
int64_t Reader::read_int64(const InputVarType& _var) const {
  return _var.val_->as<int64_t>();
}
bool Reader::is_uint64(const InputVarType& _var) const {
  return _var.val_->is_uint64();
}
uint64_t Reader::read_uint64(const InputVarType& _var) const {
  return _var.val_->as<uint64_t>();
}
size_t Reader::size(jsoncons::json* _value) const { return _value->size(); }
jsoncons::json* Reader::array_at(jsoncons::json* _value, size_t _index) const {
  return &_value->at(_index);
}
std::string_view Reader::object_key(jsoncons::json* _value,
                                    size_t _index) const {
  return (_value->object_range().begin() + _index)->key();
}
jsoncons::json* Reader::object_value(jsoncons::json* _value,
                                     size_t _index) const {
  return &(_value->object_range().begin() + _index)->value();
}

}  // namespace rfl::ubjson
