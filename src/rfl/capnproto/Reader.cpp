#include "rfl/capnproto/Reader.hpp"

#include "rfl/parsing/schemaful/IsSchemafulReader.hpp"

namespace rfl::capnproto {

static_assert(parsing::schemaful::IsSchemafulReader<Reader>,
              "This must be a schemaful reader.");

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  // TODO: Is this correct?
  return _var.val_.getType() == capnp::DynamicValue::VOID;
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  if (_var.val_.getType() != capnp::DynamicValue::LIST) {
    return Error("Could not cast to a list.");
  }
  return InputArrayType{_var.val_.as<capnp::DynamicList>()};
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  if (_var.val_.getType() != capnp::DynamicValue::STRUCT) {
    return Error("Could not cast to a struct.");
  }
  return InputObjectType{_var.val_.as<capnp::DynamicStruct>()};
}

rfl::Result<Reader::InputMapType> Reader::to_map(
    const InputVarType& _var) const noexcept {
  if (_var.val_.getType() != capnp::DynamicValue::LIST) {
    return Error("Could not cast to a list.");
  }
  return InputMapType{_var.val_.as<capnp::DynamicList>()};
}

rfl::Result<Reader::InputUnionType> Reader::to_union(
    const InputVarType& _var) const noexcept {
  if (_var.val_.getType() != capnp::DynamicValue::STRUCT) {
    return Error("Could not cast to a struct.");
  }
  return InputUnionType{_var.val_.as<capnp::DynamicStruct>()};
}

}  // namespace rfl::capnproto
