#include "rfl/avro/Reader.hpp"

namespace rfl::avro {

rfl::Result<Reader::InputVarType> Reader::get_field_from_array(
    const size_t _idx, const InputArrayType& _arr) const noexcept {
  return Error("TODO");
}

rfl::Result<Reader::InputVarType> Reader::get_field_from_object(
    const std::string& _name, const InputObjectType& _obj) const noexcept {
  return Error("TODO");
}

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return avro_value_get_type(_val.val_) == AVRO_NULL;
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  if (avro_value_get_type(_val.val_) != AVRO_ARRAY) {
    return Error("Could not cast to an array.");
  }
  return InputArrayType{_var.val_};
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  const auto type = avro_value_get_type(_val.val_);
  if (type != AVRO_RECORD && type != AVRO_MAP) {
    return Error("Could not cast to an object.");
  }
  return InputObjectType{_var.val_};
}

}  // namespace rfl::avro
