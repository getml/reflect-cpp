#ifndef RFL_CAPNPROTO_WRITER_HPP_
#define RFL_CAPNPROTO_WRITER_HPP_

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

#include "../Bytestring.hpp"
#include "../Vectorstring.hpp"
#include "../always_false.hpp"
#include "../common.hpp"
#include "../internal/is_literal.hpp"
#include "../internal/ptr_cast.hpp"
#include "NativeHandle.hpp"

namespace rfl::capnproto {

class RFL_API Writer {
 public:
  struct CapnProtoOutputArray {
    detail::ListBuilder val_;
    size_t ix_ = 0;
  };

  struct CapnProtoOutputMap {
    detail::StructBuilder val_;
    size_t ix_ = 0;
  };

  struct CapnProtoOutputObject {
    detail::StructBuilder val_;
  };

  struct CapnProtoOutputUnion {
    detail::StructBuilder val_;
  };

  struct CapnProtoOutputVar {
    detail::ValueBuilder val_;
  };

  using OutputArrayType = CapnProtoOutputArray;
  using OutputMapType = CapnProtoOutputMap;
  using OutputObjectType = CapnProtoOutputObject;
  using OutputUnionType = CapnProtoOutputUnion;
  using OutputVarType = CapnProtoOutputVar;

  template <class T>
    requires std::is_same_v<typename T::Builds, capnp::DynamicStruct>
  Writer(T* _root) : root_(_root) {}

  ~Writer();

  template <class T>
  OutputArrayType array_as_root(const T /*_size*/) const {
    static_assert(always_false_v<T>,
                  "In Cap'n Proto, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
  }

  template <class T>
  OutputMapType map_as_root(const T /*_size*/) const {
    static_assert(always_false_v<T>,
                  "In Cap'n Proto, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
  }

  Writer::OutputObjectType object_as_root(const size_t _size) const;

  template <class T = int>
  OutputVarType null_as_root() const {
    static_assert(always_false_v<T>,
                  "In Cap'n Proto, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
  }

  template <class T = int>
  OutputUnionType union_as_root() const {
    static_assert(always_false_v<T>,
                  "In Cap'n Proto, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
  }

  template <class T>
  OutputVarType value_as_root(const T& /*_var*/) const {
    static_assert(always_false_v<T>,
                  "In Cap'n Proto, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
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

  OutputVarType add_null_to_array(OutputArrayType* _parent) const;

  OutputVarType add_null_to_map(const std::string_view& _name,
                                OutputMapType* _parent) const;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const;

  OutputVarType add_null_to_union(const size_t _index,
                                  OutputUnionType* _parent) const;

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const {
    return set_array(to_value(_var), _parent);
  }
  template <class T>
  OutputVarType add_value_to_map(const std::string_view& _name, const T& _var,
                                 OutputMapType* _parent) const {
    auto entry = map_entry(_name, _parent);
    return add_value_to_object("value", _var, &entry);
  }
  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const {
    return set_object(_name, to_value(_var), _parent);
  }
  template <class T>
  OutputVarType add_value_to_union(size_t _index, const T& _var,
                                   OutputUnionType* _parent) const {
    return set_union(_index, to_value(_var), _parent);
  }

  void end_array(OutputArrayType* /*_arr*/) const {}

  void end_map(OutputMapType* /*_obj*/) const {}

  void end_object(OutputObjectType* /*_obj*/) const {}

 private:
  using BasicValue = std::variant<std::string, std::vector<char>, bool, double,
                                  uint64_t, int64_t>;
  template <class T>
  static BasicValue to_value(const T& _var) {
    using U = std::remove_cvref_t<T>;
    if constexpr (std::is_same_v<U, std::string> || std::is_same_v<U, bool>)
      return _var;
    else if constexpr (std::is_same_v<U, Bytestring> ||
                       std::is_same_v<U, Vectorstring>) {
      const auto begin = internal::ptr_cast<const char*>(_var.data());
      return std::vector<char>(begin, begin + _var.size());
    } else if constexpr (std::is_floating_point_v<U>)
      return static_cast<double>(_var);
    else if constexpr (std::is_unsigned_v<U>)
      return static_cast<uint64_t>(_var);
    else if constexpr (std::is_integral_v<U>)
      return static_cast<int64_t>(_var);
    else if constexpr (internal::is_literal_v<T>)
      return to_value(_var.value());
    else
      static_assert(always_false_v<T>, "Unsupported type.");
  }
  OutputObjectType map_entry(const std::string_view&, OutputMapType*) const;
  OutputVarType set_array(const BasicValue&, OutputArrayType*) const;
  OutputVarType set_object(const std::string_view&, const BasicValue&,
                           OutputObjectType*) const;
  OutputVarType set_union(size_t, const BasicValue&, OutputUnionType*) const;
  void* root_;
};

}  // namespace rfl::capnproto
#endif
