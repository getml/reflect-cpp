#include "rfl/avro/Writer.hpp"

#include <string_view>

#include "rfl/parsing/schemaful/IsSchemafulWriter.hpp"

namespace rfl::avro {

static_assert(parsing::schemaful::IsSchemafulWriter<Writer>,
              "This must be a schemaful writer.");

Writer::Writer(avro_value_t* _root) : root_(_root){};

Writer::~Writer() = default;

Writer::OutputArrayType Writer::array_as_root(const size_t /*_size*/) const {
  return OutputArrayType{*root_};
}

Writer::OutputMapType Writer::map_as_root(const size_t /*_size*/) const {
  return OutputMapType{*root_};
}

Writer::OutputObjectType Writer::object_as_root(const size_t /*_size*/) const {
  return OutputObjectType{*root_};
}

Writer::OutputVarType Writer::null_as_root() const {
  int result = avro_value_set_null(root_);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputVarType{*root_};
}

Writer::OutputUnionType Writer::union_as_root() const {
  return OutputUnionType{*root_};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t /*_size*/, OutputArrayType* _parent) const {
  avro_value_t new_array;
  int result = avro_value_append(&_parent->val_, &new_array, nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputArrayType{new_array};
}

Writer::OutputArrayType Writer::add_array_to_map(const std::string_view& _name,
                                                 const size_t /*_size*/,
                                                 OutputMapType* _parent) const {
  avro_value_t new_array;
  int result = avro_value_add(&_parent->val_, _name.data(), &new_array, nullptr,
                              nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputArrayType{new_array};
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t /*_size*/,
    OutputObjectType* _parent) const {
  avro_value_t new_array;
  int result =
      avro_value_get_by_name(&_parent->val_, _name.data(), &new_array, nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputArrayType{new_array};
}

Writer::OutputArrayType Writer::add_array_to_union(
    const size_t _index, const size_t /*_size*/,
    OutputUnionType* _parent) const {
  if (_index > static_cast<size_t>(INT_MAX)) {
    throw std::runtime_error(std::string(__FUNCTION__) + " index error");
  }
  avro_value_t new_array;
  int result = avro_value_set_branch(&_parent->val_, static_cast<int>(_index),
                                     &new_array);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputArrayType{new_array};
}

Writer::OutputMapType Writer::add_map_to_array(const size_t /*_size*/,
                                               OutputArrayType* _parent) const {
  avro_value_t new_map;
  int result = avro_value_append(&_parent->val_, &new_map, nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputMapType{new_map};
}

Writer::OutputMapType Writer::add_map_to_map(const std::string_view& _name,
                                             const size_t /*_size*/,
                                             OutputMapType* _parent) const {
  avro_value_t new_map;
  int result =
      avro_value_add(&_parent->val_, _name.data(), &new_map, nullptr, nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputMapType{new_map};
}

Writer::OutputMapType Writer::add_map_to_object(
    const std::string_view& _name, const size_t /*_size*/,
    OutputObjectType* _parent) const {
  avro_value_t new_map;
  int result =
      avro_value_get_by_name(&_parent->val_, _name.data(), &new_map, nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputMapType{new_map};
}

Writer::OutputMapType Writer::add_map_to_union(const size_t _index,
                                               const size_t /*_size*/,
                                               OutputUnionType* _parent) const {
  if (_index > static_cast<size_t>(INT_MAX)) {
    throw std::runtime_error(std::string(__FUNCTION__) + " index error");
  }
  avro_value_t new_map;
  int result =
      avro_value_set_branch(&_parent->val_, static_cast<int>(_index), &new_map);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputMapType{new_map};
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t /*_size*/, OutputArrayType* _parent) const {
  avro_value_t new_object;
  int result = avro_value_append(&_parent->val_, &new_object, nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputObjectType{new_object};
}

Writer::OutputObjectType Writer::add_object_to_map(
    const std::string_view& _name, const size_t /*_size*/,
    OutputMapType* _parent) const {
  avro_value_t new_object;
  int result = avro_value_add(&_parent->val_, _name.data(), &new_object,
                              nullptr, nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputObjectType{new_object};
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t /*_size*/,
    OutputObjectType* _parent) const {
  avro_value_t new_object;
  int result = avro_value_get_by_name(&_parent->val_, _name.data(), &new_object,
                                      nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputObjectType{new_object};
}

Writer::OutputObjectType Writer::add_object_to_union(
    const size_t _index, const size_t /*_size*/,
    OutputUnionType* _parent) const {
  if (_index > static_cast<size_t>(INT_MAX)) {
    throw std::runtime_error(std::string(__FUNCTION__) + " index error");
  }
  avro_value_t new_object;
  int result = avro_value_set_branch(&_parent->val_, static_cast<int>(_index),
                                     &new_object);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputObjectType{new_object};
}

Writer::OutputUnionType Writer::add_union_to_array(
    OutputArrayType* _parent) const {
  avro_value_t new_union;
  int result = avro_value_append(&_parent->val_, &new_union, nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputUnionType{new_union};
}

Writer::OutputUnionType Writer::add_union_to_map(const std::string_view& _name,
                                                 OutputMapType* _parent) const {
  avro_value_t new_union;
  int result = avro_value_add(&_parent->val_, _name.data(), &new_union, nullptr,
                              nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputUnionType{new_union};
}

Writer::OutputUnionType Writer::add_union_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const {
  avro_value_t new_union;
  int result =
      avro_value_get_by_name(&_parent->val_, _name.data(), &new_union, nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputUnionType{new_union};
}

Writer::OutputUnionType Writer::add_union_to_union(
    const size_t _index, OutputUnionType* _parent) const {
  if (_index > static_cast<size_t>(INT_MAX)) {
    throw std::runtime_error(std::string(__FUNCTION__) + " index error");
  }
  avro_value_t new_union;
  int result = avro_value_set_branch(&_parent->val_, static_cast<int>(_index),
                                     &new_union);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputUnionType{new_union};
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const {
  avro_value_t new_null;
  int result = avro_value_append(&_parent->val_, &new_null, nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  result = avro_value_set_null(&new_null);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputVarType{new_null};
}

Writer::OutputVarType Writer::add_null_to_map(const std::string_view& _name,
                                              OutputMapType* _parent) const {
  avro_value_t new_null;
  int result =
      avro_value_add(&_parent->val_, _name.data(), &new_null, nullptr, nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  result = avro_value_set_null(&new_null);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputVarType{new_null};
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const {
  avro_value_t new_null;
  int result =
      avro_value_get_by_name(&_parent->val_, _name.data(), &new_null, nullptr);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  result = avro_value_set_null(&new_null);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputVarType{new_null};
}

Writer::OutputVarType Writer::add_null_to_union(
    const size_t _index, OutputUnionType* _parent) const {
  if (_index > static_cast<size_t>(INT_MAX)) {
    throw std::runtime_error(std::string(__FUNCTION__) + " index error");
  }
  avro_value_t new_null;
  int result = avro_value_set_branch(&_parent->val_, static_cast<int>(_index),
                                     &new_null);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  result = avro_value_set_null(&new_null);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  return OutputVarType{new_null};
}

}  // namespace rfl::avro
