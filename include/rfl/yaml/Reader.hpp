#ifndef RFL_YAML_READER_HPP_
#define RFL_YAML_READER_HPP_

#include <yaml-cpp/yaml.h>

#include <array>
#include <exception>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl {
namespace yaml {

struct Reader {
  struct YAMLInputArray {
    YAMLInputArray(const YAML::Node& _node) : node_(_node) {}
    YAML::Node node_;
  };

  struct YAMLInputObject {
    YAMLInputObject(const YAML::Node& _node) : node_(_node) {}
    YAML::Node node_;
  };

  struct YAMLInputVar {
    YAMLInputVar(const YAML::Node& _node) : node_(_node) {}
    YAML::Node node_;
  };

  using InputArrayType = YAMLInputArray;
  using InputObjectType = YAMLInputObject;
  using InputVarType = YAMLInputVar;

  template <class T, class = void>
  struct has_from_json_obj : std::false_type {};

  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_yaml_obj(var); });

  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType& _arr) const noexcept;

  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType& _obj) const noexcept;

  bool is_empty(const InputVarType& _var) const noexcept;

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    try {
      if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>() ||
                    std::is_same<std::remove_cvref_t<T>, bool>() ||
                    std::is_floating_point<std::remove_cvref_t<T>>()) {
        return _var.node_.as<std::remove_cvref_t<T>>();
      } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
        return static_cast<T>(_var.node_.as<std::remove_cvref_t<int64_t>>());
      } else {
        static_assert(rfl::always_false_v<T>, "Unsupported type.");
      }
    } catch (std::exception& e) {
      return rfl::Error(e.what());
    }
  }

  rfl::Result<InputArrayType> to_array(const InputVarType& _var) const noexcept;

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    for (size_t i = 0; i < _arr.node_.size(); ++i) {
      const auto err = _array_reader.read(_arr.node_[i]);
      if (err) {
        return err;
      }
    }
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    for (const auto& p : _obj.node_) {
      try {
        const auto k = p.first.as<std::string>();
        _object_reader.read(std::string_view(k), InputVarType(p.second));
      } catch (std::exception& e) {
        continue;
      }
    }
    return std::nullopt;
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept;

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType _var) const noexcept {
    try {
      return T::from_yaml_obj(_var);
    } catch (std::exception& e) {
      return rfl::Error(e.what());
    }
  }
};

}  // namespace yaml
}  // namespace rfl

#endif
