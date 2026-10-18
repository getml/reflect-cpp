#include "rfl/capnproto/Reader.hpp"

#include "rfl/parsing/schemaful/IsSchemafulReader.hpp"

namespace rfl::capnproto {
static_assert(parsing::schemaful::IsSchemafulReader<Reader>);
bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return _var.val_.native().getType() == capnp::DynamicValue::VOID;
}
Result<std::string> Reader::read_string(
    const InputVarType& _var) const noexcept {
  const auto& value = _var.val_.native();
  if (value.getType() != capnp::DynamicValue::TEXT)
    return error("Could not cast to string.");
  return std::string(value.as<capnp::Text>().cStr());
}
Result<std::vector<char>> Reader::read_bytes(
    const InputVarType& _var, bool _vectorstring) const noexcept {
  const auto& value = _var.val_.native();
  if (value.getType() != capnp::DynamicValue::DATA)
    return error(_vectorstring ? "Could not cast to vectorstring."
                               : "Could not cast to bytestring.");
  const auto bytes = value.as<capnp::Data>();
  const auto begin = internal::ptr_cast<const char*>(bytes.begin());
  return std::vector<char>(begin, begin + bytes.size());
}
Result<bool> Reader::read_bool(const InputVarType& _var) const noexcept {
  const auto& value = _var.val_.native();
  if (value.getType() != capnp::DynamicValue::BOOL)
    return error("Could not cast to boolean.");
  return value.as<bool>();
}
Result<double> Reader::read_double(const InputVarType& _var) const noexcept {
  const auto& value = _var.val_.native();
  if (value.getType() != capnp::DynamicValue::FLOAT)
    return error("Could not cast to float.");
  return value.as<double>();
}
bool Reader::is_unsigned(const InputVarType& _var) const noexcept {
  return _var.val_.native().getType() == capnp::DynamicValue::UINT;
}
Result<int64_t> Reader::read_int64(const InputVarType& _var) const noexcept {
  const auto& value = _var.val_.native();
  if (value.getType() != capnp::DynamicValue::INT)
    return error("Could not cast to an integer.");
  return value.as<int64_t>();
}
Result<uint64_t> Reader::read_uint64(const InputVarType& _var) const noexcept {
  const auto& value = _var.val_.native();
  if (value.getType() != capnp::DynamicValue::UINT)
    return error("Could not cast to an integer.");
  return value.as<uint64_t>();
}
Result<uint16_t> Reader::read_enum(const InputVarType& _var) const noexcept {
  const auto& value = _var.val_.native();
  if (value.getType() != capnp::DynamicValue::ENUM)
    return error("Could not cast to an enum.");
  return value.as<capnp::DynamicEnum>().getRaw();
}
Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  const auto& value = _var.val_.native();
  if (value.getType() != capnp::DynamicValue::LIST)
    return error("Could not cast to a list.");
  return InputArrayType{value.as<capnp::DynamicList>()};
}
Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  const auto& value = _var.val_.native();
  if (value.getType() != capnp::DynamicValue::STRUCT)
    return error("Could not cast to a struct.");
  return InputObjectType{value.as<capnp::DynamicStruct>()};
}
Result<Reader::InputMapType> Reader::to_map(
    const InputVarType& _var) const noexcept {
  const auto& value = _var.val_.native();
  if (value.getType() != capnp::DynamicValue::STRUCT)
    return error("Could not cast to a map.");
  return InputMapType{value.as<capnp::DynamicStruct>()};
}
Result<Reader::InputUnionType> Reader::to_union(
    const InputVarType& _var) const noexcept {
  const auto& value = _var.val_.native();
  if (value.getType() != capnp::DynamicValue::STRUCT)
    return error("Could not cast to a struct.");
  return InputUnionType{value.as<capnp::DynamicStruct>()};
}
size_t Reader::array_size(const InputArrayType& _arr) const {
  return _arr.val_.native().size();
}
Reader::InputVarType Reader::array_at(const InputArrayType& _arr,
                                      size_t _ix) const {
  return {_arr.val_.native()[static_cast<unsigned>(_ix)]};
}
size_t Reader::object_size(const InputObjectType& _obj) const {
  return _obj.val_.native().getSchema().getFields().size();
}
Reader::InputVarType Reader::object_at(const InputObjectType& _obj,
                                       size_t _ix) const {
  const auto& value = _obj.val_.native();
  return {value.get(value.getSchema().getFields()[static_cast<unsigned>(_ix)])};
}
Reader::InputArrayType Reader::map_entries(const InputMapType& _map) const {
  return {_map.val_.native().get("entries").as<capnp::DynamicList>()};
}
std::string_view Reader::map_key(const InputVarType& _entry) const {
  const auto key = _entry.val_.native()
                       .as<capnp::DynamicStruct>()
                       .get("key")
                       .as<capnp::Text>();
  return {key.cStr(), key.size()};
}
Reader::InputVarType Reader::map_value(const InputVarType& _entry) const {
  return {_entry.val_.native().as<capnp::DynamicStruct>().get("value")};
}
std::optional<std::pair<Reader::InputVarType, size_t>>
Reader::identify_discriminant(const InputUnionType& _union) const noexcept {
  const auto& value = _union.val_.native();
  size_t ix = 0;
  for (auto field : value.getSchema().getFields()) {
    if (value.has(field))
      return std::make_pair(InputVarType{value.get(field)}, ix);
    ++ix;
  }
  return std::nullopt;
}
}  // namespace rfl::capnproto
