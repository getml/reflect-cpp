#include "rfl/avro/Writer.hpp"

#include "rfl/parsing/schemaful/IsSchemafulWriter.hpp"

namespace rfl::avro {

static_assert(parsing::schemaful::IsSchemafulWriter<Writer>,
              "This must be a schemaful writer.");

Writer::Writer(avro_value_t* _root) : root_(_root){};

Writer::~Writer() = default;

Writer::OutputArrayType Writer::array_as_root(
    const size_t _size) const noexcept {
  return OutputArrayType{*root_};
}

Writer::OutputObjectType Writer::object_as_root(
    const size_t _size) const noexcept {
  return OutputObjectType{*root_};
}

Writer::OutputVarType Writer::null_as_root() const noexcept {
  avro_value_set_null(root_);
  return OutputVarType{*root_};
}

Writer::OutputUnionType Writer::union_as_root() const noexcept {
  return OutputUnionType{*root_};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  avro_value_t new_array;
  avro_value_append(&_parent->val_, &new_array, nullptr);
  return OutputArrayType{new_array};
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  avro_value_t new_array;
  avro_value_get_by_name(&_parent->val_, _name.data(), &new_array, nullptr);
  return OutputArrayType{new_array};
}

Writer::OutputArrayType Writer::add_array_to_union(
    const size_t _index, const size_t _size,
    OutputUnionType* _parent) const noexcept {
  avro_value_t new_array;
  avro_value_set_branch(&_parent->val_, static_cast<int>(_index), &new_array);
  return OutputArrayType{new_array};
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  avro_value_t new_object;
  avro_value_append(&_parent->val_, &new_object, nullptr);
  return OutputObjectType{new_object};
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  avro_value_t new_object;
  avro_value_get_by_name(&_parent->val_, _name.data(), &new_object, nullptr);
  return OutputObjectType{new_object};
}

Writer::OutputObjectType Writer::add_object_to_union(
    const size_t _index, const size_t _size,
    OutputUnionType* _parent) const noexcept {
  avro_value_t new_object;
  avro_value_set_branch(&_parent->val_, static_cast<int>(_index), &new_object);
  return OutputObjectType{new_object};
}

Writer::OutputUnionType Writer::add_union_to_array(
    OutputArrayType* _parent) const noexcept {
  avro_value_t new_union;
  avro_value_append(&_parent->val_, &new_union, nullptr);
  return OutputUnionType{new_union};
}

Writer::OutputUnionType Writer::add_union_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  avro_value_t new_union;
  avro_value_get_by_name(&_parent->val_, _name.data(), &new_union, nullptr);
  return OutputUnionType{new_union};
}

Writer::OutputUnionType Writer::add_union_to_union(
    const size_t _index, OutputUnionType* _parent) const noexcept {
  avro_value_t new_union;
  avro_value_set_branch(&_parent->val_, static_cast<int>(_index), &new_union);
  return OutputUnionType{new_union};
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const noexcept {
  avro_value_t new_null;
  avro_value_append(&_parent->val_, &new_null, nullptr);
  avro_value_set_null(&new_null);
  return OutputVarType{new_null};
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  avro_value_t new_null;
  avro_value_get_by_name(&_parent->val_, _name.data(), &new_null, nullptr);
  avro_value_set_null(&new_null);
  return OutputVarType{new_null};
}

Writer::OutputVarType Writer::add_null_to_union(
    const size_t _index, OutputUnionType* _parent) const noexcept {
  avro_value_t new_null;
  avro_value_set_branch(&_parent->val_, static_cast<int>(_index), &new_null);
  avro_value_set_null(&new_null);
  return OutputVarType{new_null};
}

void Writer::end_array(OutputArrayType* _arr) const noexcept {}

void Writer::end_object(OutputObjectType* _obj) const noexcept {}

}  // namespace rfl::avro
