#ifndef RFL_AVRO_WRITER_HPP_
#define RFL_AVRO_WRITER_HPP_

#include <avro.h>

#include <bit>
#include <cstdint>
#include <exception>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

#include "../Box.hpp"
#include "../Bytestring.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/is_literal.hpp"

namespace rfl::avro {

class Writer {
 public:
  struct AVROOutputArray {
    avro_value_t val_;
  };

  struct AVROOutputMap {
    avro_value_t val_;
  };

  struct AVROOutputObject {
    avro_value_t val_;
  };

  struct AVROOutputUnion {
    avro_value_t val_;
  };

  struct AVROOutputVar {
    avro_value_t val_;
  };

  using OutputArrayType = AVROOutputArray;
  using OutputMapType = AVROOutputMap;
  using OutputObjectType = AVROOutputObject;
  using OutputUnionType = AVROOutputUnion;
  using OutputVarType = AVROOutputVar;

  Writer(avro_value_t* _root);

  ~Writer();

  OutputArrayType array_as_root(const size_t _size) const noexcept;

  OutputMapType map_as_root(const size_t _size) const noexcept;

  OutputObjectType object_as_root(const size_t _size) const noexcept;

  OutputVarType null_as_root() const noexcept;

  OutputUnionType union_as_root() const noexcept;

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    set_value(_var, root_);
    return OutputVarType{*root_};
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept;

  OutputArrayType add_array_to_map(const std::string_view& _name,
                                   const size_t _size,
                                   OutputMapType* _parent) const noexcept;

  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const noexcept;

  OutputArrayType add_array_to_union(const size_t _index, const size_t _size,
                                     OutputUnionType* _parent) const noexcept;

  OutputMapType add_map_to_array(const size_t _size,
                                 OutputArrayType* _parent) const noexcept;

  OutputMapType add_map_to_map(const std::string_view& _name,
                               const size_t _size,
                               OutputMapType* _parent) const noexcept;

  OutputMapType add_map_to_object(const std::string_view& _name,
                                  const size_t _size,
                                  OutputObjectType* _parent) const noexcept;

  OutputMapType add_map_to_union(const size_t _index, const size_t _size,
                                 OutputUnionType* _parent) const noexcept;

  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const noexcept;

  OutputObjectType add_object_to_map(const std::string_view& _name,
                                     const size_t _size,
                                     OutputMapType* _parent) const noexcept;

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept;

  OutputObjectType add_object_to_union(const size_t _index, const size_t _size,
                                       OutputUnionType* _parent) const noexcept;

  OutputUnionType add_union_to_array(OutputArrayType* _parent) const noexcept;

  OutputUnionType add_union_to_map(const std::string_view& _name,
                                   OutputMapType* _parent) const noexcept;

  OutputUnionType add_union_to_object(const std::string_view& _name,
                                      OutputObjectType* _parent) const noexcept;

  OutputUnionType add_union_to_union(const size_t _index,
                                     OutputUnionType* _parent) const noexcept;

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept;

  OutputVarType add_null_to_map(const std::string_view& _name,
                                OutputMapType* _parent) const noexcept;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept;

  OutputVarType add_null_to_union(const size_t _index,
                                  OutputUnionType* _parent) const noexcept;

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const noexcept {
    avro_value_t new_value;
    avro_value_append(&_parent->val_, &new_value, nullptr);
    set_value(_var, &new_value);
    return OutputVarType{new_value};
  }

  template <class T>
  OutputVarType add_value_to_map(const std::string_view& _name, const T& _var,
                                 OutputMapType* _parent) const noexcept {
    avro_value_t new_value;
    avro_value_add(&_parent->val_, _name.data(), &new_value, nullptr, nullptr);
    set_value(_var, &new_value);
    return OutputVarType{new_value};
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    avro_value_t new_value;
    avro_value_get_by_name(&_parent->val_, _name.data(), &new_value, nullptr);
    set_value(_var, &new_value);
    return OutputVarType{new_value};
  }

  template <class T>
  OutputVarType add_value_to_union(const size_t _index, const T& _var,
                                   OutputUnionType* _parent) const noexcept {
    avro_value_t new_value;
    avro_value_set_branch(&_parent->val_, static_cast<int>(_index), &new_value);
    set_value(_var, &new_value);
    return OutputVarType{new_value};
  }

  void end_array(OutputArrayType* _arr) const noexcept {}

  void end_map(OutputMapType* _obj) const noexcept {}

  void end_object(OutputObjectType* _obj) const noexcept {}

 private:
  template <class T>
  void set_value(const T& _var, avro_value_t* _val) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      avro_value_set_string_len(_val, _var.c_str(), _var.size() + 1);

    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      auto var = _var;
      avro_value_set_bytes(_val, var.data(), var.size());

    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      avro_value_set_boolean(_val, _var);

    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      avro_value_set_double(_val, static_cast<double>(_var));

    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      avro_value_set_long(_val, static_cast<std::int64_t>(_var));

    } else if constexpr (internal::is_literal_v<T>) {
      avro_value_set_enum(_val, static_cast<int>(_var.value()));

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

 private:
  avro_value_t* root_;
};

}  // namespace rfl::avro

#endif
