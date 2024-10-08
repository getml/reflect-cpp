#include "rfl/flexbuf/Writer.hpp"

namespace rfl::flexbuf {

Writer::Writer(const Ref<flexbuffers::Builder>& _fbb) : fbb_(_fbb) {}

Writer::~Writer() = default;

Writer::OutputArrayType Writer::array_as_root(
    const size_t _size) const noexcept {
  return new_array();
}

Writer::OutputObjectType Writer::object_as_root(
    const size_t _size) const noexcept {
  return new_object();
}

Writer::OutputVarType Writer::null_as_root() const noexcept {
  fbb_->Null();
  return OutputVarType{};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  return new_array();
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  return new_array(_name);
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  return new_object();
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  return new_object(_name);
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const noexcept {
  fbb_->Null();
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  fbb_->Null(_name.data());
  return OutputVarType{};
}

void Writer::end_array(OutputArrayType* _arr) const noexcept {
  fbb_->EndVector(_arr->start_, false, false);
}

void Writer::end_object(OutputObjectType* _obj) const noexcept {
  fbb_->EndMap(_obj->start_);
}

Writer::OutputArrayType Writer::new_array(
    const std::string_view& _name) const noexcept {
  const auto start = fbb_->StartVector(_name.data());
  return OutputArrayType{start};
}

Writer::OutputArrayType Writer::new_array() const noexcept {
  const auto start = fbb_->StartVector();
  return OutputArrayType{start};
}

Writer::OutputObjectType Writer::new_object(
    const std::string_view& _name) const noexcept {
  const auto start = fbb_->StartMap(_name.data());
  return OutputObjectType{start};
}

Writer::OutputObjectType Writer::new_object() const noexcept {
  const auto start = fbb_->StartMap();
  return OutputObjectType{start};
}

}  // namespace rfl::flexbuf
