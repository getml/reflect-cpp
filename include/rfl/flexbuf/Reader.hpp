#ifndef FLEXBUF_READER_HPP_
#define FLEXBUF_READER_HPP_

#include <flatbuffers/flexbuffers.h>

#include <bit>
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
#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl {
namespace flexbuf {

struct Reader {
  using InputArrayType = flexbuffers::Vector;
  using InputObjectType = flexbuffers::Map;
  using InputVarType = flexbuffers::Reference;

  template <class T, class = void>
  struct has_from_flexbuf : std::false_type {};

  template <class T>
  struct has_from_flexbuf<
      T, std::enable_if_t<std::is_invocable_r<T, decltype(T::from_flexbuf),
                                              InputVarType>::value>>
      : std::true_type {};

  template <class T>
  struct has_from_flexbuf<
      T, std::enable_if_t<std::is_invocable_r<
             rfl::Result<T>, decltype(T::from_flexbuf), InputVarType>::value>>
      : std::true_type {};

  template <class T>
  static constexpr bool has_custom_constructor = has_from_flexbuf<T>::value;

  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType& _arr) const noexcept;

  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType& _obj) const noexcept;

  bool is_empty(const InputVarType& _var) const noexcept;

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      if (!_var.IsString()) {
        return rfl::Error("Could not cast to a string.");
      }
      return std::string(_var.AsString().c_str());
    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      if (!_var.IsBlob()) {
        return rfl::Error("Could not cast to a bytestring.");
      }
      const auto blob = _var.AsBlob();
      return rfl::Bytestring(std::bit_cast<const std::byte*>(blob.data()),
                             blob.size());
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (!_var.IsBool()) {
        return rfl::Error("Could not cast to boolean.");
      }
      return _var.AsBool();
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      if (!_var.IsNumeric()) {
        return rfl::Error("Could not cast to double.");
      }
      return static_cast<T>(_var.AsDouble());
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      if (!_var.IsNumeric()) {
        return rfl::Error("Could not cast to int.");
      }
      return static_cast<T>(_var.AsInt64());
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    const auto size = _arr.size();
    for (size_t i = 0; i < size; ++i) {
      const auto err = _array_reader.read(InputVarType(_arr[i]));
      if (err) {
        return err;
      }
    }
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    const auto keys = _obj.Keys();
    const auto values = _obj.Values();
    const auto num_values = std::min(keys.size(), values.size());

    for (size_t i = 0; i < num_values; ++i) {
      _object_reader.read(std::string_view(keys[i].AsString().c_str()),
                          values[i]);
    }

    return std::nullopt;
  }

  rfl::Result<InputArrayType> to_array(const InputVarType& _var) const noexcept;

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept;

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_flexbuf(_var);
    } catch (std::exception& e) {
      return rfl::Error(e.what());
    }
  }
};

}  // namespace flexbuf
}  // namespace rfl

#endif
