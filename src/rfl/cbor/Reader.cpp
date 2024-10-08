#include "rfl/cbor/Reader.hpp"

namespace rfl::cbor {

rfl::Result<Reader::InputVarType> Reader::get_field_from_array(
    const size_t _idx, const InputArrayType& _arr) const noexcept {
  InputVarType var;
  auto err = cbor_value_enter_container(&_arr.val_, &var.val_);
  if (err != CborNoError && err != CborErrorOutOfMemory) {
    return Error(cbor_error_string(err));
  }
  size_t length = 0;
  err = cbor_value_get_array_length(&_arr.val_, &length);
  if (err != CborNoError && err != CborErrorOutOfMemory) {
    return Error(cbor_error_string(err));
  }
  if (_idx >= length) {
    return Error("Index " + std::to_string(_idx) + " of of bounds.");
  }
  for (size_t i = 0; i < _idx; ++i) {
    err = cbor_value_advance(&var.val_);
    if (err != CborNoError && err != CborErrorOutOfMemory) {
      return Error(cbor_error_string(err));
    }
  }
  return var;
}

rfl::Result<Reader::InputVarType> Reader::get_field_from_object(
    const std::string& _name, const InputObjectType& _obj) const noexcept {
  InputVarType var;
  auto buffer = std::string();
  auto err = cbor_value_enter_container(&_obj.val_, &var.val_);
  if (err != CborNoError) {
    return Error(cbor_error_string(err));
  }
  size_t length = 0;
  err = cbor_value_get_map_length(&_obj.val_, &length);
  if (err != CborNoError) {
    return Error(cbor_error_string(err));
  }
  for (size_t i = 0; i < length; ++i) {
    if (!cbor_value_is_text_string(&var.val_)) {
      return Error("Expected the key to be a string value.");
    }
    err = get_string(&var.val_, &buffer);
    if (err != CborNoError) {
      return Error(cbor_error_string(err));
    }
    err = cbor_value_advance(&var.val_);
    if (err != CborNoError) {
      return Error(cbor_error_string(err));
    }
    if (_name == buffer) {
      return var;
    }
    err = cbor_value_advance(&var.val_);
    if (err != CborNoError) {
      return Error(cbor_error_string(err));
    }
  }
  return Error("No field named '" + _name + "' was found.");
}

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return cbor_value_is_null(&_var.val_);
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  if (!cbor_value_is_array(&_var.val_)) {
    return Error("Could not cast to an array.");
  }
  return InputArrayType{_var.val_};
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  if (!cbor_value_is_map(&_var.val_)) {
    return Error("Could not cast to an object.");
  }
  return InputObjectType{_var.val_};
}

CborError Reader::get_bytestring(const CborValue* _ptr,
                                 rfl::Bytestring* _str) const noexcept {
  size_t length = 0;
  auto err = cbor_value_get_string_length(_ptr, &length);
  if (err != CborNoError && err != CborErrorOutOfMemory) {
    return err;
  }
  _str->resize(length);
  if (length > 0) {
    return cbor_value_copy_byte_string(
        _ptr, reinterpret_cast<uint8_t*>(&((*_str)[0])), &length, NULL);
  } else {
    return CborNoError;
  }
}

CborError Reader::get_string(const CborValue* _ptr,
                             std::string* _str) const noexcept {
  size_t length = 0;
  auto err = cbor_value_get_string_length(_ptr, &length);
  if (err != CborNoError && err != CborErrorOutOfMemory) {
    return err;
  }
  _str->resize(length);
  if (length > 0) {
    return cbor_value_copy_text_string(_ptr, &((*_str)[0]), &length, NULL);
  } else {
    return CborNoError;
  }
}

}  // namespace rfl::cbor
