#include "rfl/capnproto/Writer.hpp"

#include <stdexcept>

#include "rfl/parsing/schemaful/IsSchemafulWriter.hpp"

namespace rfl::capnproto {

static_assert(parsing::schemaful::IsSchemafulWriter<Writer>,
              "This must be a schemaful writer.");

Writer::Writer(capnp::DynamicStruct::Builder* _root) : root_(_root){};

Writer::~Writer() = default;

Writer::OutputObjectType Writer::object_as_root(
    const size_t _size) const noexcept {
  return OutputObjectType{root_->as<capnp::DynamicStruct>()};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  return OutputArrayType{
      _parent->val_.init(_parent->ix_++, _size).as<capnp::DynamicList>()};
}

Writer::OutputArrayType Writer::add_array_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const noexcept {
  auto entries = OutputArrayType{
      .val_ = _parent->val_.get("entries").as<capnp::DynamicList>(),
      .ix_ = _parent->ix_++};
  auto new_entry = add_object_to_array(2, &entries);
  add_value_to_object("key", std::string(_name), &new_entry);
  return add_array_to_object("value", _size, &new_entry);
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  return OutputArrayType{
      _parent->val_.init(_name.data(), _size).as<capnp::DynamicList>()};
}

Writer::OutputArrayType Writer::add_array_to_union(
    const size_t _index, const size_t _size,
    OutputUnionType* _parent) const noexcept {
  const auto field = _parent->val_.getSchema().getFields()[_index];
  return OutputArrayType{
      _parent->val_.init(field, _size).template as<capnp::DynamicList>()};
}

Writer::OutputMapType Writer::add_map_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  auto new_map = add_object_to_array(1, _parent);
  add_array_to_object("entries", _size, &new_map);
  return OutputMapType{new_map.val_};
}

Writer::OutputMapType Writer::add_map_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const noexcept {
  auto new_map = add_object_to_map(_name, 1, _parent);
  add_array_to_object("entries", _size, &new_map);
  return OutputMapType{new_map.val_};
}

Writer::OutputMapType Writer::add_map_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  auto new_map = add_object_to_object(_name, 1, _parent);
  add_array_to_object("entries", _size, &new_map);
  return OutputMapType{new_map.val_};
}

Writer::OutputMapType Writer::add_map_to_union(
    const size_t _index, const size_t _size,
    OutputUnionType* _parent) const noexcept {
  auto new_map = add_object_to_union(_index, 1, _parent);
  add_array_to_object("entries", _size, &new_map);
  return OutputMapType{new_map.val_};
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  return OutputObjectType{
      _parent->val_[_parent->ix_++].as<capnp::DynamicStruct>()};
}

Writer::OutputObjectType Writer::add_object_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const noexcept {
  auto entries = OutputArrayType{
      .val_ = _parent->val_.get("entries").as<capnp::DynamicList>(),
      .ix_ = _parent->ix_++};
  auto new_entry = add_object_to_array(2, &entries);
  add_value_to_object("key", std::string(_name), &new_entry);
  return add_object_to_object("value", _size, &new_entry);
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  return OutputObjectType{
      _parent->val_.get(_name.data()).as<capnp::DynamicStruct>()};
}

Writer::OutputObjectType Writer::add_object_to_union(
    const size_t _index, const size_t _size,
    OutputUnionType* _parent) const noexcept {
  const auto field = _parent->val_.getSchema().getFields()[_index];
  return OutputObjectType{
      _parent->val_.init(field).template as<capnp::DynamicStruct>()};
}

Writer::OutputUnionType Writer::add_union_to_array(
    OutputArrayType* _parent) const noexcept {
  return OutputUnionType{
      _parent->val_[_parent->ix_++].as<capnp::DynamicStruct>()};
}

Writer::OutputUnionType Writer::add_union_to_map(
    const std::string_view& _name, OutputMapType* _parent) const noexcept {
  auto entries = OutputArrayType{
      .val_ = _parent->val_.get("entries").as<capnp::DynamicList>(),
      .ix_ = _parent->ix_++};
  auto new_entry = add_object_to_array(2, &entries);
  add_value_to_object("key", std::string(_name), &new_entry);
  return add_union_to_object("value", &new_entry);
}

Writer::OutputUnionType Writer::add_union_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  return OutputUnionType{
      _parent->val_.get(_name.data()).as<capnp::DynamicStruct>()};
}

Writer::OutputUnionType Writer::add_union_to_union(
    const size_t _index, OutputUnionType* _parent) const noexcept {
  const auto field = _parent->val_.getSchema().getFields()[_index];
  return OutputUnionType{
      _parent->val_.init(field).template as<capnp::DynamicStruct>()};
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const noexcept {
  _parent->val_.set(_parent->ix_++, capnp::VOID);
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_map(
    const std::string_view& _name, OutputMapType* _parent) const noexcept {
  auto entries = OutputArrayType{
      .val_ = _parent->val_.get("entries").as<capnp::DynamicList>(),
      .ix_ = _parent->ix_++};
  auto new_entry = add_object_to_array(2, &entries);
  add_value_to_object("key", std::string(_name), &new_entry);
  return add_null_to_object("value", &new_entry);
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  _parent->val_.set(_name.data(), capnp::VOID);
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_union(
    const size_t _index, OutputUnionType* _parent) const noexcept {
  const auto field = _parent->val_.getSchema().getFields()[_index];
  _parent->val_.set(field, capnp::VOID);
  return OutputVarType{};
}

}  // namespace rfl::capnproto
