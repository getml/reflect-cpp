#ifndef RFL_CAPNPROTO_WRITER_HPP_
#define RFL_CAPNPROTO_WRITER_HPP_

#include <capnp/dynamic.h>
#include <kj/array.h>

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#include "../Bytestring.hpp"
#include "../Vectorstring.hpp"
#include "../always_false.hpp"
#include "../common.hpp"
#include "../internal/is_literal.hpp"

namespace rfl::capnproto {

class RFL_API Writer {
 public:
  struct CapnProtoOutputArray {
    capnp::DynamicList::Builder val_;
    size_t ix_ = 0;
  };

  struct CapnProtoOutputMap {
    capnp::DynamicStruct::Builder val_;
    size_t ix_ = 0;
  };

  struct CapnProtoOutputObject {
    capnp::DynamicStruct::Builder val_;
  };

  struct CapnProtoOutputUnion {
    capnp::DynamicStruct::Builder val_;
  };

  struct CapnProtoOutputVar {
    capnp::DynamicValue::Builder val_;
  };

  using OutputArrayType = CapnProtoOutputArray;
  using OutputMapType = CapnProtoOutputMap;
  using OutputObjectType = CapnProtoOutputObject;
  using OutputUnionType = CapnProtoOutputUnion;
  using OutputVarType = CapnProtoOutputVar;

  Writer(capnp::DynamicStruct::Builder* _root);

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
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      _parent->val_.set(_parent->ix_++, _var.c_str());

    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>() ||
                         std::is_same<std::remove_cvref_t<T>,
                                      rfl::Vectorstring>()) {
      const auto array_ptr = kj::ArrayPtr<const kj::byte>(
          internal::ptr_cast<const unsigned char*>(_var.data()), _var.size());
      _parent->val_.set(_parent->ix_++, capnp::Data::Reader(array_ptr));

    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_same<std::remove_cvref_t<T>, bool>()) {
      _parent->val_.set(_parent->ix_++, _var);

    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      _parent->val_.set(_parent->ix_++, static_cast<std::int64_t>(_var));

    } else if constexpr (internal::is_literal_v<T>) {
      return add_value_to_array(_var.value(), _parent);

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_map(const std::string_view& _name, const T& _var,
                                 OutputMapType* _parent) const {
    auto entries =
        OutputArrayType{_parent->val_.get("entries").as<capnp::DynamicList>()};
    auto new_entry = add_object_to_array(2, &entries);
    add_value_to_object("key", _name, &new_entry);
    return add_value_to_object("value", _var, &new_entry);
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      _parent->val_.set(to_kj_string_ptr(_name), _var.c_str());

    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      const auto array_ptr = kj::ArrayPtr<const kj::byte>(
          internal::ptr_cast<const unsigned char*>(_var.data()), _var.size());
      _parent->val_.set(to_kj_string_ptr(_name),
                        capnp::Data::Reader(array_ptr));

    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_same<std::remove_cvref_t<T>, bool>()) {
      _parent->val_.set(to_kj_string_ptr(_name), _var);

    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      _parent->val_.set(to_kj_string_ptr(_name),
                        static_cast<std::int64_t>(_var));

    } else if constexpr (internal::is_literal_v<T>) {
      return add_value_to_object(_name, _var.value(), _parent);

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_union(const size_t _index, const T& _var,
                                   OutputUnionType* _parent) const {
    const auto field = _parent->val_.getSchema().getFields()[_index];

    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      _parent->val_.set(field, _var.c_str());

    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      const auto array_ptr = kj::ArrayPtr<const kj::byte>(
          internal::ptr_cast<const unsigned char*>(_var.data()), _var.size());
      _parent->val_.set(field, capnp::Data::Reader(array_ptr));

    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_same<std::remove_cvref_t<T>, bool>()) {
      _parent->val_.set(field, _var);

    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      _parent->val_.set(field, static_cast<std::int64_t>(_var));

    } else if constexpr (internal::is_literal_v<T>) {
      return add_value_to_union(_index, _var.value(), _parent);

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

  void end_array(OutputArrayType* /*_arr*/) const {}

  void end_map(OutputMapType* /*_obj*/) const {}

  void end_object(OutputObjectType* /*_obj*/) const {}

 private:
  kj::StringPtr to_kj_string_ptr(const std::string_view& _str) const {
    return kj::StringPtr(_str.begin(), _str.end());
  }

 private:
  capnp::DynamicStruct::Builder* root_;
};

}  // namespace rfl::capnproto

#endif
