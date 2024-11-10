#include "rfl/cbor/Writer.hpp"

namespace rfl::cbor {

Writer::Writer(CborEncoder* _encoder) : encoder_(_encoder) {}

Writer::~Writer() = default;

Writer::OutputArrayType Writer::array_as_root(
    const size_t _size) const noexcept {
  return new_array(_size, encoder_);
}

Writer::OutputObjectType Writer::object_as_root(
    const size_t _size) const noexcept {
  return new_object(_size, encoder_);
}

Writer::OutputVarType Writer::null_as_root() const noexcept {
  cbor_encode_null(encoder_);
  return OutputVarType{};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  return new_array(_size, _parent->encoder_);
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  cbor_encode_text_string(_parent->encoder_, _name.data(), _name.size());
  return new_array(_size, _parent->encoder_);
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  return new_object(_size, _parent->encoder_);
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  cbor_encode_text_string(_parent->encoder_, _name.data(), _name.size());
  return new_object(_size, _parent->encoder_);
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const noexcept {
  cbor_encode_null(_parent->encoder_);
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  cbor_encode_text_string(_parent->encoder_, _name.data(), _name.size());
  cbor_encode_null(_parent->encoder_);
  return OutputVarType{};
}

void Writer::end_array(OutputArrayType* _arr) const noexcept {
  cbor_encoder_close_container(_arr->parent_, _arr->encoder_);
}

void Writer::end_object(OutputObjectType* _obj) const noexcept {
  cbor_encoder_close_container(_obj->parent_, _obj->encoder_);
}

Writer::OutputArrayType Writer::new_array(const size_t _size,
                                          CborEncoder* _parent) const noexcept {
  subencoders_->emplace_back(rfl::Box<CborEncoder>::make());
  cbor_encoder_create_array(_parent, subencoders_->back().get(), _size);
  return OutputArrayType{subencoders_->back().get(), _parent};
}

Writer::OutputObjectType Writer::new_object(
    const size_t _size, CborEncoder* _parent) const noexcept {
  subencoders_->emplace_back(rfl::Box<CborEncoder>::make());
  cbor_encoder_create_map(_parent, subencoders_->back().get(), _size);
  return OutputObjectType{subencoders_->back().get(), _parent};
}

}  // namespace rfl::cbor
