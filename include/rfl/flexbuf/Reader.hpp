#ifndef FLEXBUF_READER_HPP_
#define FLEXBUF_READER_HPP_

#include <flatbuffers/flexbuffers.h>

#include <exception>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

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

  rfl::Result<InputVarType> get_field(
      const std::string& _name, const InputObjectType& _obj) const noexcept {
    const auto keys = _obj.Keys();
    for (size_t i = 0; i < keys.size(); ++i) {
      if (_name == keys[i].AsString().c_str()) {
        return _obj.Values()[i];
      }
    }
    return rfl::Error("Map does not contain any element called '" + _name +
                      "'.");
  }

  bool is_empty(const InputVarType& _var) const noexcept {
    return _var.IsNull();
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      if (!_var.IsString()) {
        return rfl::Error("Could not cast to string.");
      }
      return std::string(_var.AsString().c_str());
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

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    if (!_var.IsVector()) {
      return rfl::Error("Could not cast to Vector.");
    }
    return _var.AsVector();
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    if (!_var.IsMap()) {
      return rfl::Error("Could not cast to Map!");
    }
    return _var.AsMap();
  }

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
