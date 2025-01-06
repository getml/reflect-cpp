#ifndef RFL_CAPNPROTO_WRITER_HPP_
#define RFL_CAPNPROTO_WRITER_HPP_

#include <capnp/dynamic.h>
#include <kj/array.h>

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
#include "../internal/ptr_cast.hpp"

namespace rfl::capnproto {

class Writer {
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
  OutputArrayType array_as_root(const T _size) const noexcept {
    static_assert(always_false_v<T>,
                  "In Cap'n Proto, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
  }

  template <class T>
  OutputMapType map_as_root(const T _size) const noexcept {
    static_assert(always_false_v<T>,
                  "In Cap'n Proto, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
  }

  Writer::OutputObjectType object_as_root(const size_t _size) const noexcept;

  template <class T = int>
  OutputVarType null_as_root() const noexcept {
    static_assert(always_false_v<T>,
                  "In Cap'n Proto, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
  }

  template <class T = int>
  OutputUnionType union_as_root() const noexcept {
    static_assert(always_false_v<T>,
                  "In Cap'n Proto, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
  }

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    static_assert(always_false_v<T>,
                  "In Cap'n Proto, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
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
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      _parent->val_.set(_parent->ix_++, _var.c_str());

    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
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
                                 OutputMapType* _parent) const noexcept {
    auto entries =
        OutputArrayType{_parent->val_.get("entries").as<capnp::DynamicList>()};
    auto new_entry = add_object_to_array(2, &entries);
    add_value_to_object("key", std::string(_name), &new_entry);
    return add_value_to_object("value", _var, &new_entry);
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      _parent->val_.set(_name.data(), _var.c_str());

    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      const auto array_ptr = kj::ArrayPtr<const kj::byte>(
          internal::ptr_cast<const unsigned char*>(_var.data()), _var.size());
      _parent->val_.set(_name.data(), capnp::Data::Reader(array_ptr));

    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_same<std::remove_cvref_t<T>, bool>()) {
      _parent->val_.set(_name.data(), _var);

    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      _parent->val_.set(_name.data(), static_cast<std::int64_t>(_var));

    } else if constexpr (internal::is_literal_v<T>) {
      return add_value_to_object(_name, _var.value(), _parent);

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_union(const size_t _index, const T& _var,
                                   OutputUnionType* _parent) const noexcept {
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

  void end_array(OutputArrayType* _arr) const noexcept {}

  void end_map(OutputMapType* _obj) const noexcept {}

  void end_object(OutputObjectType* _obj) const noexcept {}

 private:
  capnp::DynamicStruct::Builder* root_;
};

}  // namespace rfl::capnproto

#endif
