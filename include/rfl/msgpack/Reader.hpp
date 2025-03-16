#ifndef RFL_MSGPACK_READER_HPP_
#define RFL_MSGPACK_READER_HPP_

#include <msgpack.h>

#include <cstddef>
#include <exception>
#include <string>
#include <string_view>
#include <type_traits>

#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/ptr_cast.hpp"

namespace rfl {
namespace msgpack {

struct Reader {
  using InputArrayType = msgpack_object_array;
  using InputObjectType = msgpack_object_map;
  using InputVarType = msgpack_object;

  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_msgpack_obj(var); });

  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType _arr) const noexcept {
    if (_idx >= _arr.size) {
      return error("Index " + std::to_string(_idx) + " of of bounds.");
    }
    return _arr.ptr[_idx];
  }

  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType& _obj) const noexcept {
    for (uint32_t i = 0; i < _obj.size; ++i) {
      const auto& key = _obj.ptr[i].key;
      if (key.type != MSGPACK_OBJECT_STR) {
        return error("Key in element " + std::to_string(i) +
                     " was not a string.");
      }
      const auto current_name =
          std::string_view(key.via.str.ptr, key.via.str.size);
      if (_name == current_name) {
        return _obj.ptr[i].val;
      }
    }
    return error("No field named '" + _name + "' was found.");
  }

  bool is_empty(const InputVarType& _var) const noexcept {
    return _var.type == MSGPACK_OBJECT_NIL;
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    const auto type = _var.type;
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      if (type != MSGPACK_OBJECT_STR) {
        return error("Could not cast to string.");
      }
      const auto str = _var.via.str;
      return std::string(str.ptr, str.size);

    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      if (type != MSGPACK_OBJECT_BIN) {
        return error("Could not cast to a bytestring.");
      }
      const auto bin = _var.via.bin;
      const auto data = internal::ptr_cast<const std::byte*>(bin.ptr);
      return rfl::Bytestring(data, data + bin.size);

    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (type != MSGPACK_OBJECT_BOOLEAN) {
        return error("Could not cast to boolean.");
      }
      return _var.via.boolean;

    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      if (type == MSGPACK_OBJECT_FLOAT32 || type == MSGPACK_OBJECT_FLOAT64 ||
          type == MSGPACK_OBJECT_FLOAT) {
        return static_cast<T>(_var.via.f64);
      }
      return error(
          "Could not cast to numeric value. The type must be float "
          "or double.");

    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      if (type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
        return static_cast<T>(_var.via.u64);
      } else if (type == MSGPACK_OBJECT_NEGATIVE_INTEGER) {
        return static_cast<T>(_var.via.i64);
      }
      return error(
          "Could not cast to numeric value. The type must be integral, float "
          "or double.");
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    if (_var.type != MSGPACK_OBJECT_ARRAY) {
      return error("Could not cast to an array.");
    }
    return _var.via.array;
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    if (_var.type != MSGPACK_OBJECT_MAP) {
      return error("Could not cast to a map.");
    }
    return _var.via.map;
  }

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    for (uint32_t i = 0; i < _arr.size; ++i) {
      const auto err = _array_reader.read(_arr.ptr[i]);
      if (err) {
        return err;
      }
    }
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    for (uint32_t i = 0; i < _obj.size; ++i) {
      const auto& key = _obj.ptr[i].key;
      const auto& val = _obj.ptr[i].val;
      if (key.type != MSGPACK_OBJECT_STR) {
        return rfl::Error("Key in element " + std::to_string(i) +
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
      return error(e.what());
    }
  }
};

}  // namespace msgpack
}  // namespace rfl

#endif
