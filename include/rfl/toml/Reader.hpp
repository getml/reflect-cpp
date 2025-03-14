#ifndef RFL_TOML_READER_HPP_
#define RFL_TOML_READER_HPP_

#include <array>
#include <exception>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <toml.hpp>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl::toml {

struct Reader {
  using InputArrayType = const ::toml::array*;
  using InputObjectType = const ::toml::table*;
  using InputVarType = const ::toml::value*;

  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_toml_obj(var); });

  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType _arr) const noexcept {
    if (_idx >= _arr->size()) {
      return error("Index " + std::to_string(_idx) + " of of bounds.");
    }
    return &(*_arr)[_idx];
  }

  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType _obj) const noexcept {
    const auto it = _obj->find(_name);
    if (it == _obj->end()) {
      return error("Object contains no field named '" + _name + "'.");
    }
    return &it->second;
  }

  bool is_empty(const InputVarType _var) const noexcept {
    return _var->is_empty();
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      if (!_var->is_string()) {
        return error("Could not cast to std::string!");
      }
      return _var->as_string();
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (!_var->is_boolean()) {
        return error("Could not cast to bool!");
      }
      return _var->as_boolean();
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      if (!_var->is_floating()) {
        return error("Could not cast to double!");
      }
      return static_cast<std::remove_cvref_t<T>>(_var->as_floating());
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      if (!_var->is_integer()) {
        return error("Could not cast to int64_t!");
      }
      return static_cast<std::remove_cvref_t<T>>(_var->as_integer());
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(const InputVarType _var) const noexcept {
    if (!_var->is_array()) {
      return error("Could not cast to an array!");
    }
    return &_var->as_array();
  }

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType _arr) const noexcept {
    for (const auto& node : *_arr) {
      const auto err = _array_reader.read(&node);
      if (err) {
        return err;
      }
    }
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   InputObjectType _obj) const noexcept {
    for (const auto& [k, v] : *_obj) {
      _object_reader.read(std::string_view(k), &v);
    }
    return std::nullopt;
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType _var) const noexcept {
    if (!_var->is_table()) {
      return error("Could not cast to a table!");
    }
    return &_var->as_table();
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType _var) const noexcept {
    try {
      return T::from_toml_obj(_var);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }
};

}  // namespace rfl::toml

#endif
