#include "rfl/msgpack/Writer.hpp"

#include <stdexcept>

namespace rfl::msgpack {

Writer::Writer(msgpack_packer* _pk) : pk_(_pk) {}

Writer::~Writer() = default;

Writer::OutputArrayType Writer::array_as_root(const size_t _size) const {
  return new_array(_size);
}

Writer::OutputObjectType Writer::object_as_root(const size_t _size) const {
  return new_object(_size);
}

Writer::OutputVarType Writer::null_as_root() const {
  const auto err = msgpack_pack_nil(pk_);
  if (err) {
    throw std::runtime_error("Could not pack null as root.");
  }
  return OutputVarType{};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* /*_parent*/) const {
  return new_array(_size);
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* /*_parent*/) const {
  auto err = msgpack_pack_str(pk_, _name.size());
  if (err) {
    throw std::runtime_error("Could not pack field name: '" +
                             std::string(_name) + "'");
  }
  err = msgpack_pack_str_body(pk_, _name.data(), _name.size());
  if (err) {
    throw std::runtime_error("Could not pack field name body: '" +
                             std::string(_name) + "'");
  }
  return new_array(_size);
}

Writer::OutputObjectType Writer::add_object_to_array(const size_t _size,
                                                     OutputArrayType*) const {
  return new_object(_size);
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType*) const {
  auto err = msgpack_pack_str(pk_, _name.size());
  if (err) {
    throw std::runtime_error("Could not pack field name: '" +
                             std::string(_name) + "'");
  }
  err = msgpack_pack_str_body(pk_, _name.data(), _name.size());
  if (err) {
    throw std::runtime_error("Could not pack field name body: '" +
                             std::string(_name) + "'");
  }
  return new_object(_size);
}

Writer::OutputVarType Writer::add_null_to_array(OutputArrayType*) const {
  const auto err = msgpack_pack_nil(pk_);
  if (err) {
    throw std::runtime_error("Could not add nil to array.");
  }
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* /*_parent*/) const {
  auto err = msgpack_pack_str(pk_, _name.size());
  if (err) {
    throw std::runtime_error("Could not pack field name: '" +
                             std::string(_name) + "'");
  }
  err = msgpack_pack_str_body(pk_, _name.data(), _name.size());
  if (err) {
    throw std::runtime_error("Could not pack field name body: '" +
                             std::string(_name) + "'");
  }
  err = msgpack_pack_nil(pk_);
  if (err) {
    throw std::runtime_error(
        "Could not pack null value for the following field: '" +
        std::string(_name) + "'");
  }
  return OutputVarType{};
}

void Writer::end_array(OutputArrayType* /*_arr*/) const noexcept {}

void Writer::end_object(OutputObjectType* /*_obj*/) const noexcept {}

Writer::OutputArrayType Writer::new_array(const size_t _size) const {
  const auto err = msgpack_pack_array(pk_, _size);
  if (err) {
    throw std::runtime_error("Could not pack array.");
  }
  return OutputArrayType{};
}

Writer::OutputObjectType Writer::new_object(const size_t _size) const {
  const auto err = msgpack_pack_map(pk_, _size);
  if (err) {
    throw std::runtime_error("Could not pack array.");
  }
  return OutputObjectType{};
}

}  // namespace rfl::msgpack
