#ifndef RFL_CEREAL_WRITER_HPP_
#define RFL_CEREAL_WRITER_HPP_

#include <cereal/archives/portable_binary.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#include "../Bytestring.hpp"
#include "../Vectorstring.hpp"
#include "../always_false.hpp"
#include "../common.hpp"
#include "../internal/is_literal.hpp"
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

  Writer(CerealArchive* _archive);

  ~Writer() = default;

  OutputArrayType array_as_root(const size_t _size) const;

  OutputMapType map_as_root(const size_t _size) const;

  OutputObjectType object_as_root(const size_t _size) const;

  OutputUnionType union_as_root() const;

  OutputVarType null_as_root() const;

  template <class T>
  OutputVarType value_as_root(const T& _var) const {
    (*archive_)(_var);
    return OutputVarType{};
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const;

  OutputArrayType add_array_to_map(const std::string_view& _name,
                                   const size_t _size,
                                   OutputMapType* _parent) const;

  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const;

  OutputArrayType add_array_to_union(const size_t _index, const size_t _size,
                                     OutputUnionType* _parent) const;

  OutputMapType add_map_to_array(const size_t _size,
                                 OutputArrayType* _parent) const;

  OutputMapType add_map_to_map(const std::string_view& _name,
                               const size_t _size,
                               OutputMapType* _parent) const;

  OutputMapType add_map_to_object(const std::string_view& _name,
                                  const size_t _size,
                                  OutputObjectType* _parent) const;

  OutputMapType add_map_to_union(const size_t _index, const size_t _size,
                                 OutputUnionType* _parent) const;

  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const;

  OutputObjectType add_object_to_map(const std::string_view& _name,
                                     const size_t _size,
                                     OutputMapType* _parent) const;

  OutputObjectType add_object_to_object(const std::string_view& _name,
                                        const size_t _size,
                                        OutputObjectType* _parent) const;

  OutputObjectType add_object_to_union(const size_t _index, const size_t _size,
                                       OutputUnionType* _parent) const;

  OutputUnionType add_union_to_array(OutputArrayType* _parent) const;

  OutputUnionType add_union_to_map(const std::string_view& _name,
                                   OutputMapType* _parent) const;

  OutputUnionType add_union_to_object(const std::string_view& _name,
                                      OutputObjectType* _parent) const;

  OutputUnionType add_union_to_union(const size_t _index,
                                     OutputUnionType* _parent) const;

  template <class T>
  OutputVarType add_value_to_array(const T& _var, OutputArrayType*) const {
    add_value(_var);
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_map(const std::string_view& _name, const T& _var,
                                 OutputMapType* _parent) const {
    add_string_view(_name);
    add_value(_var);
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var, OutputObjectType*) const {
    add_value(_var);
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_union(const size_t _index, const T& _var,
                                   OutputUnionType* _parent) const {
    (*archive_)(_index);
    add_value(_var);
    return OutputVarType{};
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const;

  OutputVarType add_null_to_map(const std::string_view& _name,
                                OutputMapType* _parent) const;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const;

  OutputVarType add_null_to_union(const size_t _index,
                                  OutputUnionType* _parent) const;

  void end_array(OutputArrayType* _arr) const noexcept;

  void end_map(OutputMapType* _map) const noexcept;

  void end_object(OutputObjectType* _obj) const noexcept;

 private:
  void add_string_view(const std::string_view& _str) const;

  template <class T>
  void add_value(const T& _var) const noexcept {
    using Type = std::remove_cvref_t<T>;
    if constexpr (internal::is_literal_v<Type>) {
      add_value(_var.str());

    } else if constexpr (std::is_same<Type, rfl::Bytestring>() ||
                         std::is_same<Type, rfl::Vectorstring>()) {
      (*archive_)(::cereal::make_size_tag(_var.size()));
      (*archive_)(::cereal::binary_data(_var.data(), _var.size()));

    } else {
      (*archive_)(_var);
    }
  }

 private:
  CerealArchive* archive_;
};

static_assert(parsing::schemaful::IsSchemafulWriter<Writer>);

}  // namespace rfl::cereal

#endif
