#include "rfl/avro/Reader.hpp"

#include "rfl/parsing/schemaful/IsSchemafulReader.hpp"

namespace rfl::avro {

static_assert(parsing::schemaful::IsSchemafulReader<Reader>,
              "This must be a schemaful reader.");

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return avro_value_get_type(_var.val_) == AVRO_NULL;
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  if (avro_value_get_type(_var.val_) != AVRO_ARRAY) {
    return Error("Could not cast to an array.");
  }
  return InputArrayType{_var.val_};
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  const auto type = avro_value_get_type(_var.val_);
  if (type != AVRO_RECORD) {
    return Error("Could not cast to an object.");
  }
  return InputObjectType{_var.val_};
}

rfl::Result<Reader::InputMapType> Reader::to_map(
    const InputVarType& _var) const noexcept {
  const auto type = avro_value_get_type(_var.val_);
  if (type != AVRO_MAP) {
    return Error("Could not cast to a map.");
  }
  return InputMapType{_var.val_};
}

rfl::Result<Reader::InputUnionType> Reader::to_union(
    const InputVarType& _var) const noexcept {
  const auto type = avro_value_get_type(_var.val_);
  if (type != AVRO_UNION) {
    return Error("Could not cast to a union.");
  }
  return InputUnionType{_var.val_};
}

}  // namespace rfl::avro
