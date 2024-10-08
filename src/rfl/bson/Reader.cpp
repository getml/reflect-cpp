#include "rfl/bson/Reader.hpp"

namespace rfl::bson {

rfl::Result<Reader::InputVarType> Reader::get_field_from_array(
    const size_t _idx, const InputArrayType& _arr) const noexcept {
  bson_t b;
  bson_iter_t iter;
  const auto doc = _arr.val_->value.v_doc;
  if (bson_init_static(&b, doc.data, doc.data_len)) {
    if (bson_iter_init(&iter, &b)) {
      size_t i = 0;
      while (bson_iter_next(&iter)) {
        if (i == _idx) {
          return to_input_var(&iter);
        }
        ++i;
      }
    } else {
      return Error("Could not init the array iteration.");
    }
  } else {
    return Error("Could not init array.");
  }
  return Error("Index " + std::to_string(_idx) + " of of bounds.");
}

rfl::Result<Reader::InputVarType> Reader::get_field_from_object(
    const std::string& _name, const InputObjectType& _obj) const noexcept {
  bson_t b;
  bson_iter_t iter;
  const auto doc = _obj.val_->value.v_doc;
  if (bson_init_static(&b, doc.data, doc.data_len)) {
    if (bson_iter_init(&iter, &b)) {
      while (bson_iter_next(&iter)) {
        auto key = std::string(bson_iter_key(&iter));
        if (key == _name) {
          return to_input_var(&iter);
        }
      }
    }
  }
  return Error("No field named '" + _name + "' was found.");
}

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return _var.val_->value_type == BSON_TYPE_NULL;
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  const auto btype = _var.val_->value_type;
  if (btype != BSON_TYPE_ARRAY && btype != BSON_TYPE_DOCUMENT) {
    return Error("Could not cast to an array.");
  }
  return InputArrayType{_var.val_};
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  const auto btype = _var.val_->value_type;
  if (btype != BSON_TYPE_DOCUMENT) {
    return Error("Could not cast to a document.");
  }
  return InputObjectType{_var.val_};
}

Reader::InputVarType Reader::to_input_var(bson_iter_t* _iter) const noexcept {
  return InputVarType{bson_iter_value(_iter)};
}

}  // namespace rfl::bson
