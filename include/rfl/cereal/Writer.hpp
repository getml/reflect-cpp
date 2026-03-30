#ifndef RFL_CEREAL_WRITER_HPP_
#define RFL_CEREAL_WRITER_HPP_

#include <cereal/archives/portable_binary.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#include "../always_false.hpp"
#include "../common.hpp"
#include "../parsing/schemaful/IsSchemafulWriter.hpp"

namespace rfl::cereal {

class Writer {
 public:
  using CerealArchive = ::cereal::PortableBinaryOutputArchive;

  struct CerealOutputArray {};

  struct CerealOutputMap {};

  struct CerealOutputObject {};

  struct CerealOutputUnion {};

  struct CerealOutputVar {};

  using OutputArrayType = CerealOutputArray;
  using OutputObjectType = CerealOutputObject;
  using OutputMapType = CerealOutputMap;
  using OutputUnionType = CerealOutputUnion;
  using OutputVarType = CerealOutputVar;

  Writer(CerealArchive* _archive) : archive_(_archive) {}

  ~Writer() = default;

  OutputArrayType array_as_root(const size_t _size) const {
    (*archive_)(::cereal::make_size_tag(_size));
    return OutputArrayType{};
  }

  OutputMapType map_as_root(const size_t _size) const {
    (*archive_)(::cereal::make_size_tag(_size));
    return OutputMapType{};
  }

  OutputObjectType object_as_root(const size_t _size) const {
    return OutputObjectType{};
  }

  OutputUnionType union_as_root() const { return OutputUnionType{}; }

  OutputVarType null_as_root() const { return OutputVarType{}; }

  template <class T>
  OutputVarType value_as_root(const T& _var) const {
    (*archive_)(_var);
    return OutputVarType{};
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const {
    (*archive_)(::cereal::make_size_tag(_size));
    return OutputArrayType{};
  }

  OutputArrayType add_array_to_map(const std::string_view& _name,
                                   const size_t _size,
                                   OutputMapType* _parent) const {
    add_string_view(_name);
    (*archive_)(::cereal::make_size_tag(_size));
    return OutputArrayType{};
  }

  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const {
    (*archive_)(::cereal::make_size_tag(_size));
    return OutputArrayType{};
  }

  OutputArrayType add_array_to_union(const size_t _index, const size_t _size,
                                     OutputUnionType* _parent) const {
    (*archive_)(_index);
    (*archive_)(::cereal::make_size_tag(_size));
    return OutputArrayType{};
  }

  OutputMapType add_map_to_array(const size_t _size,
                                 OutputArrayType* _parent) const {
    (*archive_)(::cereal::make_size_tag(_size));
    return OutputMapType{};
  }

  OutputMapType add_map_to_map(const std::string_view& _name,
                               const size_t _size,
                               OutputMapType* _parent) const {
    (*archive_)(std::string(_name));
    (*archive_)(::cereal::make_size_tag(_size));
    return OutputMapType{};
  }

  OutputMapType add_map_to_object(const std::string_view& _name,
                                  const size_t _size,
                                  OutputObjectType* _parent) const {
    (*archive_)(::cereal::make_size_tag(_size));
    return OutputMapType{};
  }

  OutputMapType add_map_to_union(const size_t _index, const size_t _size,
                                 OutputUnionType* _parent) const {
    (*archive_)(_index);
    (*archive_)(::cereal::make_size_tag(_size));
    return OutputMapType{};
  }

  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const {
    return OutputObjectType{};
  }

  OutputObjectType add_object_to_map(const std::string_view& _name,
                                     const size_t _size,
                                     OutputMapType* _parent) const {
    add_string_view(_name);
    return OutputObjectType{};
  }

  OutputObjectType add_object_to_object(const std::string_view& _name,
                                        const size_t _size,
                                        OutputObjectType* _parent) const {
    return OutputObjectType{};
  }

  OutputObjectType add_object_to_union(const size_t _index, const size_t _size,
                                       OutputUnionType* _parent) const {
    (*archive_)(_index);
    return OutputObjectType{};
  }

  OutputUnionType add_union_to_array(OutputArrayType* _parent) const {
    return OutputUnionType{};
  }

  OutputUnionType add_union_to_map(const std::string_view& _name,
                                   OutputMapType* _parent) const {
    add_string_view(_name);
    return OutputUnionType{};
  }

  OutputUnionType add_union_to_object(const std::string_view& _name,
                                      OutputObjectType* _parent) const {
    return OutputUnionType{};
  }

  OutputUnionType add_union_to_union(const size_t _index,
                                     OutputUnionType* _parent) const {
    (*archive_)(_index);
    return OutputUnionType{};
  }

  template <class T>
  OutputVarType add_value_to_array(const T& _var, OutputArrayType*) const {
    (*archive_)(_var);
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_map(const std::string_view& _name, const T& _var,
                                 OutputMapType* _parent) const {
    add_string_view(_name);
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var, OutputObjectType*) const {
    (*archive_)(_var);
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_union(const size_t _index, const T& _var,
                                   OutputUnionType* _parent) const {
    (*archive_)(_index);
    (*archive_)(_var);
    return OutputVarType{};
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const {
    return OutputVarType{};
  }

  OutputVarType add_null_to_map(const std::string_view& _name,
                                OutputMapType* _parent) const {
    add_string_view(_name);
    return OutputVarType{};
  }

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const {
    return OutputVarType{};
  }

  OutputVarType add_null_to_union(const size_t _index,
                                  OutputUnionType* _parent) const {
    (*archive_)(_index);
    return OutputVarType{};
  }

  void end_array(OutputArrayType* _arr) const noexcept {}

  void end_map(OutputMapType* _map) const noexcept {}

  void end_object(OutputObjectType* _obj) const noexcept {}

 private:
  void add_string_view(const std::string_view& _str) const {
    (*archive_)(::cereal::make_size_tag(_str.size()));
    (*archive_)(::cereal::binary_data(_str.data(), _str.size()));
  }

 private:
  CerealArchive* archive_;
};

static_assert(parsing::schemaful::IsSchemafulWriter<Writer>);

}  // namespace rfl::cereal

#endif
