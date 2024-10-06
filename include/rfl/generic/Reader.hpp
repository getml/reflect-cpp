#ifndef GENERIC_READER_HPP_
#define GENERIC_READER_HPP_

#include <cstddef>
#include <exception>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Bytestring.hpp"
#include "../Generic.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl {
namespace generic {

struct Reader {
  using InputArrayType = Generic::Array;
  using InputObjectType = Generic::Object;
  using InputVarType = Generic;

  template <class T>
  static constexpr bool has_custom_constructor = false;

  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType& _arr) const noexcept {
    if (_idx >= _arr.size()) {
      return rfl::Error("Index " + std::to_string(_idx) + " of of bounds.");
    }
    return _arr[_idx];
  }

  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType& _obj) const noexcept {
    return _obj.get(_name);
  }

  bool is_empty(const InputVarType& _var) const noexcept {
    return _var.is_null();
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      return _var.to_string();
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      return _var.to_bool();
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      return static_cast<T>(_var.to_double());
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      return static_cast<T>(_var.to_int());
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    for (const auto& v : _arr) {
      const auto err = _array_reader.read(InputVarType(v));
      if (err) {
        return err;
      }
    }
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    for (const auto& [k, v] : _obj) {
      _object_reader.read(std::string_view(k), v);
    }
    return std::nullopt;
  }

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    return _var.to_array();
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    return _var.to_object();
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType _var) const noexcept {
    return rfl::Error("Not supported for generic types");
  }
};

}  // namespace generic
}  // namespace rfl

#endif
