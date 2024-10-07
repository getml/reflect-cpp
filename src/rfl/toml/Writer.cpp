#include "rfl/toml/Writer.hpp"

namespace rfl::toml {

Writer::Writer(::toml::table* _root) : root_(_root) {}

Writer::~Writer() = default;

Writer::OutputObjectType Writer::object_as_root(
    const size_t _size) const noexcept {
  return OutputObjectType{root_};
}

Writer::OutputVarType Writer::null_as_root() const noexcept {
  return OutputVarType{};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  const auto i = _parent->val_->size();
  _parent->val_->push_back(::toml::array());
  return OutputArrayType{_parent->val_->at(i).as_array()};
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  _parent->val_->emplace(_name, ::toml::array());
  return OutputArrayType{_parent->val_->at_path(_name).as_array()};
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  const auto i = _parent->val_->size();
  _parent->val_->push_back(::toml::table());
  return OutputObjectType{_parent->val_->at(i).as_table()};
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  _parent->val_->emplace(_name, ::toml::table());
  return OutputObjectType{_parent->val_->at_path(_name).as_table()};
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const noexcept {
  _parent->val_->push_back(std::string(""));
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  _parent->val_->emplace(_name, ::toml::value(std::string("")));
  return OutputVarType{};
}

void Writer::end_array(OutputArrayType* _arr) const noexcept {}

void Writer::end_object(OutputObjectType* _obj) const noexcept {}

}  // namespace rfl::toml
