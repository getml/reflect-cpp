#include "rfl/ubjson/Writer.hpp"

namespace rfl::ubjson {

Writer::Writer(Encoder* _encoder) : encoder_(_encoder) {}

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
  encoder_->null_value();
  return OutputVarType{};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  return new_array(_size);
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  encoder_->key(_name);
  return new_array(_size);
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  return new_object(_size);
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  encoder_->key(_name);
  return new_object(_size);
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const noexcept {
  encoder_->null_value();
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  encoder_->key(_name);
  encoder_->null_value();
  return OutputVarType{};
}

void Writer::end_array(OutputArrayType* _arr) const noexcept {
  encoder_->end_array();
}

void Writer::end_object(OutputObjectType* _obj) const noexcept {
  encoder_->end_object();
}

Writer::OutputArrayType Writer::new_array(const size_t _size) const noexcept {
  encoder_->begin_array(_size);
  return OutputArrayType{};
}

Writer::OutputObjectType Writer::new_object(const size_t _size) const noexcept {
  encoder_->begin_object(_size);
  return OutputObjectType{};
}

}  // namespace rfl::ubjson
