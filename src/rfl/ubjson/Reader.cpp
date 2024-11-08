#include "rfl/ubjson/Reader.hpp"

namespace rfl::ubjson {

rfl::Result<Reader::InputVarType> Reader::get_field_from_array(
    const size_t _idx, const InputArrayType& _arr) const noexcept {
  return Error("TODO");
}

rfl::Result<Reader::InputVarType> Reader::get_field_from_object(
    const std::string& _name, const InputObjectType& _obj) const noexcept {
  return Error("TODO");
}

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return _var.cursor_->current().event_type() ==
         jsoncons::staj_event_type::null_value;
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  if (_var.cursor_->current().event_type() !=
      jsoncons::staj_event_type::begin_array) {
    return Error("Could not cast to an array.");
  }
  return InputArrayType{_var.cursor_};
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  if (_var.cursor_->current().event_type() !=
      jsoncons::staj_event_type::begin_object) {
    return Error("Could not cast to an object.");
  }
  return InputObjectType{_var.cursor_};
}

}  // namespace rfl::ubjson
