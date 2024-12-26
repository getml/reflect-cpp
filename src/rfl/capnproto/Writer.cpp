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
  // TODO
  /*avro_value_t new_array;
avro_value_append(&_parent->val_, &new_array, nullptr);
return OutputArrayType{new_array};*/
}

Writer::OutputArrayType Writer::add_array_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_array;
  avro_value_add(&_parent->val_, _name.data(), &new_array, nullptr, nullptr);
  return OutputArrayType{new_array};*/
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
  /*avro_value_t new_array;
  avro_value_set_branch(&_parent->val_, static_cast<int>(_index), &new_array);
  return OutputArrayType{new_array};*/
}

Writer::OutputMapType Writer::add_map_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_map;
avro_value_append(&_parent->val_, &new_map, nullptr);
return OutputMapType{new_map};*/
}

Writer::OutputMapType Writer::add_map_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_map;
  avro_value_add(&_parent->val_, _name.data(), &new_map, nullptr, nullptr);
  return OutputMapType{new_map};*/
}

Writer::OutputMapType Writer::add_map_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  return OutputMapType{
      _parent->val_.init(_name.data(), _size).as<capnp::DynamicList>()};
}

Writer::OutputMapType Writer::add_map_to_union(
    const size_t _index, const size_t _size,
    OutputUnionType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_map;
avro_value_set_branch(&_parent->val_, static_cast<int>(_index), &new_map);
return OutputMapType{new_map};*/
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_object;
avro_value_append(&_parent->val_, &new_object, nullptr);
return OutputObjectType{new_object};*/
}

Writer::OutputObjectType Writer::add_object_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_object;
  avro_value_add(&_parent->val_, _name.data(), &new_object, nullptr, nullptr);
  return OutputObjectType{new_object};*/
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_object;
  avro_value_get_by_name(&_parent->val_, _name.data(), &new_object, nullptr);
  return OutputObjectType{new_object};*/
}

Writer::OutputObjectType Writer::add_object_to_union(
    const size_t _index, const size_t _size,
    OutputUnionType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_object;
avro_value_set_branch(&_parent->val_, static_cast<int>(_index), &new_object);
return OutputObjectType{new_object};
*/
}

Writer::OutputUnionType Writer::add_union_to_array(
    OutputArrayType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_union;
  avro_value_append(&_parent->val_, &new_union, nullptr);
  return OutputUnionType{new_union};*/
}

Writer::OutputUnionType Writer::add_union_to_map(
    const std::string_view& _name, OutputMapType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_union;
avro_value_add(&_parent->val_, _name.data(), &new_union, nullptr, nullptr);
return OutputUnionType{new_union};*/
}

Writer::OutputUnionType Writer::add_union_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_union;
avro_value_get_by_name(&_parent->val_, _name.data(), &new_union, nullptr);
return OutputUnionType{new_union};*/
}

Writer::OutputUnionType Writer::add_union_to_union(
    const size_t _index, OutputUnionType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_union;
avro_value_set_branch(&_parent->val_, static_cast<int>(_index), &new_union);
return OutputUnionType{new_union};*/
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_null;
avro_value_append(&_parent->val_, &new_null, nullptr);
avro_value_set_null(&new_null);
return OutputVarType{new_null};*/
}

Writer::OutputVarType Writer::add_null_to_map(
    const std::string_view& _name, OutputMapType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_null;
avro_value_add(&_parent->val_, _name.data(), &new_null, nullptr, nullptr);
avro_value_set_null(&new_null);
return OutputVarType{new_null};*/
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_null;
avro_value_get_by_name(&_parent->val_, _name.data(), &new_null, nullptr);
avro_value_set_null(&new_null);
return OutputVarType{new_null};*/
}

Writer::OutputVarType Writer::add_null_to_union(
    const size_t _index, OutputUnionType* _parent) const noexcept {
  // TODO
  /*avro_value_t new_null;
avro_value_set_branch(&_parent->val_, static_cast<int>(_index), &new_null);
avro_value_set_null(&new_null);
return OutputVarType{new_null};*/
}

}  // namespace rfl::capnproto