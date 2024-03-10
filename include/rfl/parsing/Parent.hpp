#ifndef RFL_PARSING_PARENT_HPP_
#define RFL_PARSING_PARENT_HPP_

#include <string>
#include <string_view>
#include <type_traits>

#include "../always_false.hpp"
#include "supports_attributes.hpp"

namespace rfl {
namespace parsing {

template <class W>
struct Parent {
  using OutputArrayType = typename W::OutputArrayType;
  using OutputObjectType = typename W::OutputObjectType;
  using OutputVarType = typename W::OutputVarType;

  struct Array {
    OutputArrayType* arr_;
  };

  struct Object {
    std::string_view name_;
    OutputObjectType* obj_;
    bool is_attribute_ = false;
    Object as_attribute() const { return Object{name_, obj_, true}; }
  };

  struct Root {};

  template <class ParentType>
  static OutputArrayType add_array(const W& _w, const size_t _size,
                                   const ParentType& _parent) {
    using Type = std::remove_cvref_t<ParentType>;
    if constexpr (std::is_same<Type, Array>()) {
      return _w.add_array_to_array(_size, _parent.arr_);
    } else if constexpr (std::is_same<Type, Object>()) {
      return _w.add_array_to_object(_parent.name_, _size, _parent.obj_);
    } else if constexpr (std::is_same<Type, Root>()) {
      return _w.array_as_root(_size);
    } else {
      static_assert(always_false_v<Type>, "Unsupported option.");
    }
  }

  template <class ParentType>
  static OutputObjectType add_object(const W& _w, const size_t _size,
                                     const ParentType& _parent) {
    using Type = std::remove_cvref_t<ParentType>;
    if constexpr (std::is_same<Type, Array>()) {
      return _w.add_object_to_array(_size, _parent.arr_);
    } else if constexpr (std::is_same<Type, Object>()) {
      return _w.add_object_to_object(_parent.name_, _size, _parent.obj_);
    } else if constexpr (std::is_same<Type, Root>()) {
      return _w.object_as_root(_size);
    } else {
      static_assert(always_false_v<Type>, "Unsupported option.");
    }
  }

  template <class ParentType>
  static OutputVarType add_null(const W& _w, const ParentType& _parent) {
    using Type = std::remove_cvref_t<ParentType>;
    if constexpr (std::is_same<Type, Array>()) {
      return _w.add_null_to_array(_parent.arr_);
    } else if constexpr (std::is_same<Type, Object>()) {
      if constexpr (supports_attributes<std::remove_cvref_t<W>>) {
        return _w.add_null_to_object(_parent.name_, _parent.obj_,
                                     _parent.is_attribute_);
      } else {
        return _w.add_null_to_object(_parent.name_, _parent.obj_);
      }
    } else if constexpr (std::is_same<Type, Root>()) {
      return _w.null_as_root();
    } else {
      static_assert(always_false_v<Type>, "Unsupported option.");
    }
  }

  template <class ParentType, class T>
  static OutputVarType add_value(const W& _w, const T& _var,
                                 const ParentType& _parent) {
    using Type = std::remove_cvref_t<ParentType>;
    if constexpr (std::is_same<Type, Array>()) {
      return _w.add_value_to_array(_var, _parent.arr_);
    } else if constexpr (std::is_same<Type, Object>()) {
      if constexpr (supports_attributes<std::remove_cvref_t<W>>) {
        return _w.add_value_to_object(_parent.name_, _var, _parent.obj_,
                                      _parent.is_attribute_);
      } else {
        return _w.add_value_to_object(_parent.name_, _var, _parent.obj_);
      }
    } else if constexpr (std::is_same<Type, Root>()) {
      return _w.value_as_root(_var);
    } else {
      static_assert(always_false_v<Type>, "Unsupported option.");
    }
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
