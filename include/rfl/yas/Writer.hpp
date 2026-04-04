#ifndef RFL_YAS_WRITER_HPP_
#define RFL_YAS_WRITER_HPP_

#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <yas/binary_oarchive.hpp>
#include <yas/mem_streams.hpp>
#include <yas/std_types.hpp>

#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "../Vectorstring.hpp"
#include "../always_false.hpp"
#include "../internal/is_literal.hpp"
#include "../parsing/schemaful/IsSchemafulWriter.hpp"

namespace rfl::yas {

struct Writer {
  using OArchive = ::yas::binary_oarchive<::yas::mem_ostream>;

  struct OutputArrayType {
    OArchive* ar;
  };

  struct OutputMapType {
    OArchive* ar;
  };

  struct OutputObjectType {
    OArchive* ar;
  };

  struct OutputUnionType {
    OArchive* ar;
  };

  struct OutputVarType {
    OArchive* ar;
  };

  Writer(OArchive* _ar) : ar_(_ar) {}

  template <class T>
  static constexpr bool has_custom_constructor = false;

  OutputArrayType array_as_root(const size_t _size) const noexcept {
    (*ar_) & _size;
    return OutputArrayType{ar_};
  }

  OutputMapType map_as_root(const size_t _size) const noexcept {
    (*ar_) & _size;
    return OutputMapType{ar_};
  }

  OutputObjectType object_as_root(const size_t /*_size*/) const noexcept {
    return OutputObjectType{ar_};
  }

  OutputUnionType union_as_root() const noexcept {
    return OutputUnionType{ar_};
  }

  OutputVarType null_as_root() const noexcept { return OutputVarType{ar_}; }

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    add_value(_var);
    return OutputVarType{ar_};
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept {
    (*_parent->ar) & _size;
    return OutputArrayType{_parent->ar};
  }

  OutputArrayType add_array_to_map(const std::string_view& _name,
                                   const size_t _size,
                                   OutputMapType* _parent) const noexcept;

  OutputArrayType add_array_to_object(
      const std::string_view& /*_name*/, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    (*_parent->ar) & _size;
    return OutputArrayType{_parent->ar};
  }

  OutputArrayType add_array_to_union(const size_t _index, const size_t _size,
                                     OutputUnionType* _parent) const noexcept;

  OutputMapType add_map_to_array(const size_t _size,
                                 OutputArrayType* _parent) const noexcept {
    (*_parent->ar) & _size;
    return OutputMapType{_parent->ar};
  }

  OutputMapType add_map_to_object(const std::string_view& /*_name*/,
                                  const size_t _size,
                                  OutputObjectType* _parent) const noexcept {
    (*_parent->ar) & _size;
    return OutputMapType{_parent->ar};
  }

  OutputMapType add_map_to_map(const std::string_view& _name,
                               const size_t _size,
                               OutputMapType* _parent) const noexcept;

  OutputMapType add_map_to_union(const size_t _index, const size_t _size,
                                 OutputUnionType* _parent) const noexcept;

  OutputObjectType add_object_to_array(
      const size_t /*_size*/, OutputArrayType* _parent) const noexcept {
    return OutputObjectType{_parent->ar};
  }

  OutputObjectType add_object_to_map(const std::string_view& _name,
                                     const size_t _size,
                                     OutputMapType* _parent) const noexcept;

  OutputObjectType add_object_to_object(
      const std::string_view& /*_name*/, const size_t /*_size*/,
      OutputObjectType* _parent) const noexcept {
    return OutputObjectType{_parent->ar};
  }

  OutputObjectType add_object_to_union(const size_t _index, const size_t _size,
                                       OutputUnionType* _parent) const noexcept;

  OutputUnionType add_union_to_array(OutputArrayType* _parent) const noexcept {
    return OutputUnionType{_parent->ar};
  }

  OutputUnionType add_union_to_map(const std::string_view& _name,
                                   OutputMapType* _parent) const noexcept;

  OutputUnionType add_union_to_object(
      const std::string_view& /*_name*/,
      OutputObjectType* _parent) const noexcept {
    return OutputUnionType{_parent->ar};
  }

  OutputUnionType add_union_to_union(const size_t _index,
                                     OutputUnionType* _parent) const noexcept;

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
    (*ar_) & _index;
    add_value(_var);
    return OutputVarType{};
  }

  OutputVarType add_null_to_array(OutputArrayType* /*_parent*/) const {
    return OutputVarType{};
  }

  OutputVarType add_null_to_map(const std::string_view& /*_name*/,
                                OutputMapType* /*_parent*/) const {
    return OutputVarType{};
  }

  OutputVarType add_null_to_object(const std::string_view& /*_name*/,
                                   OutputObjectType* /*_parent*/) const {
    return OutputVarType{};
  }

  OutputVarType add_null_to_union(const size_t _index,
                                  OutputUnionType* _parent) const noexcept;

  void end_array(OutputArrayType* /*_arr*/) const noexcept {}

  void end_map(OutputMapType* /*_map*/) const noexcept {}

  void end_object(OutputObjectType* /*_obj*/) const noexcept {}

  void add_string_view(const std::string_view& _str) const {
    ar_->write_seq_size(_str.length());
    ar_->write(_str.data(), _str.length());
  }

  template <class T>
  void add_value(const T& _var) const {
    if constexpr (internal::is_literal_v<T>) {
      (*ar_) & _var.str();
    } else {
      (*ar_) & _var;
    }
  }

  OArchive* ar_;
};

static_assert(parsing::schemaful::IsSchemafulWriter<Writer>);

}  // namespace rfl::yas

#endif
