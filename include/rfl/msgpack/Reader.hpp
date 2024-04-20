#ifndef RFL_MSGPACK_READER_HPP_
#define RFL_MSGPACK_READER_HPP_

#include <msgpack.h>

#include <array>
#include <concepts>
#include <exception>
#include <map>
#include <memory>
#include <source_location>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../Box.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl {
namespace msgpack {

struct Reader {
  using InputArrayType = msgpack_object_array;
  using InputObjectType = msgpack_object_map;
  using InputVarType = msgpack_object;

  template <class T>
  static constexpr bool has_custom_constructor = (requires(InputVarType var) {
    T::from_msgpack_obj(var);
  });

  rfl::Result<InputVarType> get_field(
      const std::string& _name, const InputObjectType& _obj) const noexcept {
    for (uint32_t i = 0; i < _obj.size; ++i) {
      const auto& key = _obj.ptr[i].key;
      if (key.type != MSGPACK_OBJECT_STR) {
        return Error("Key in element " + std::to_string(i) +
                     " was not a string.");
      }
      const auto current_name =
          std::string_view(key.via.str.ptr, key.via.str.size);
      if (_name == current_name) {
        return _obj.ptr[i].val;
      }
    }
    return Error("No field named '" + _name + "' was found.");
  }

  bool is_empty(const InputVarType& _var) const noexcept {
    return _var.type == MSGPACK_OBJECT_NIL;
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    const auto type = _var.type;
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      if (type != MSGPACK_OBJECT_STR) {
        return Error("Could not cast to string.");
      }
      const auto str = _var.via.str;
      return std::string(str.ptr, str.size);
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (type != MSGPACK_OBJECT_BOOLEAN) {
        return Error("Could not cast to boolean.");
      }
      return _var.via.boolean;
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_integral<std::remove_cvref_t<T>>()) {
      if (type == MSGPACK_OBJECT_FLOAT32 || type == MSGPACK_OBJECT_FLOAT64 ||
          type == MSGPACK_OBJECT_FLOAT) {
        return static_cast<T>(_var.via.f64);
      } else if (type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
        return static_cast<T>(_var.via.u64);
      } else if (type == MSGPACK_OBJECT_NEGATIVE_INTEGER) {
        return static_cast<T>(_var.via.i64);
      }
      return rfl::Error(
          "Could not cast to numeric value. The type must be integral, float "
          "or double.");
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    if (_var.type != MSGPACK_OBJECT_ARRAY) {
      return Error("Could not cast to an array.");
    }
    return _var.via.array;
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    if (_var.type != MSGPACK_OBJECT_MAP) {
      return Error("Could not cast to a map.");
    }
    return _var.via.map;
  }

  std::vector<InputVarType> to_vec(const InputArrayType& _arr) const noexcept {
    std::vector<InputVarType> vec;
    for (uint32_t i = 0; i < _arr.size; ++i) {
      vec.push_back(_arr.ptr[i]);
    }
    return vec;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    for (uint32_t i = 0; i < _obj.size; ++i) {
      const auto& key = _obj.ptr[i].key;
      const auto& val = _obj.ptr[i].val;
      if (key.type != MSGPACK_OBJECT_STR) {
        return Error("Key in element " + std::to_string(i) +
                     " was not a string.");
      }
      const auto name = std::string_view(key.via.str.ptr, key.via.str.size);
      _object_reader.read(name, val);
    }
    return std::nullopt;
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_msgpack_obj(_var);
    } catch (std::exception& e) {
      return rfl::Error(e.what());
    }
  }
};

}  // namespace msgpack
}  // namespace rfl

#endif  // JSON_PARSER_HPP_
