#include "rfl/msgpack/Writer.hpp"

namespace rfl::msgpack {

Writer::Writer(msgpack_packer* _pk) : pk_(_pk) {}

Writer::~Writer() = default;

Writer::OutputArrayType Writer::array_as_root(
    const size_t _size) const noexcept {
  return new_array(_size);
}

Writer::OutputObjectType Writer::object_as_root(
    const size_t _size) const noexcept {
  return new_object(_size);
}

Writer::OutputVarType Writer::null_as_root() const noexcept {
  msgpack_pack_nil(pk_);
  return OutputVarType{};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  return new_array(_size);
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  msgpack_pack_str(pk_, _name.size());
  msgpack_pack_str_body(pk_, _name.data(), _name.size());
  return new_array(_size);
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  return new_object(_size);
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  msgpack_pack_str(pk_, _name.size());
  msgpack_pack_str_body(pk_, _name.data(), _name.size());
  return new_object(_size);
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const noexcept {
  msgpack_pack_nil(pk_);
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  msgpack_pack_str(pk_, _name.size());
  msgpack_pack_str_body(pk_, _name.data(), _name.size());
  msgpack_pack_nil(pk_);
  return OutputVarType{};
}

void Writer::end_array(OutputArrayType* _arr) const noexcept {}

void Writer::end_object(OutputObjectType* _obj) const noexcept {}

Writer::OutputArrayType Writer::new_array(const size_t _size) const noexcept {
  msgpack_pack_array(pk_, _size);
  return OutputArrayType{};
}

Writer::OutputObjectType Writer::new_object(const size_t _size) const noexcept {
  msgpack_pack_map(pk_, _size);
  return OutputObjectType{};
}

}  // namespace rfl::msgpack
