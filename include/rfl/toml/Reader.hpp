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
#include <toml++/toml.hpp>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl::toml {

struct Reader {
  using InputArrayType = ::toml::array*;
  using InputObjectType = ::toml::table*;
  using InputVarType = ::toml::node*;

  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_toml_obj(var); });

  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType _arr) const noexcept {
    if (_idx >= _arr->size()) {
      return error("Index " + std::to_string(_idx) + " of of bounds.");
    }
    return _arr->get(_idx);
  }

  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType& _obj) const noexcept {
    auto var = (*_obj)[_name];
    if (!var) {
      return error("Object contains no field named '" + _name + "'.");
    }
    return var.node();
  }

  bool is_empty(const InputVarType& _var) const noexcept {
    return !_var && true;
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      const auto ptr = _var->as<std::string>();
      if (!ptr) {
        return error("Could not cast the node to std::string!");
      }
      return **ptr;
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      const auto ptr = _var->as<bool>();
      if (!ptr) {
        return error("Could not cast the node to bool!");
      }
      return **ptr;
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      const auto ptr = _var->as<double>();
      if (!ptr) {
        return error("Could not cast the node to double!");
      }
      return static_cast<std::remove_cvref_t<T>>(**ptr);
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      const auto ptr = _var->as<int64_t>();
      if (!ptr) {
        return error("Could not cast the node to int64_t!");
      }
      return static_cast<std::remove_cvref_t<T>>(**ptr);
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    const auto ptr = _var->as_array();
    if (!ptr) {
      return error("Could not cast to an array!");
    }
    return ptr;
  }

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    for (auto& node : *_arr) {
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
    for (auto& [k, v] : *_obj) {
      _object_reader.read(std::string_view(k), &v);
    }
    return std::nullopt;
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    const auto ptr = _var->as_table();
    if (!ptr) {
      return error("Could not cast to a table!");
    }
    return ptr;
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
