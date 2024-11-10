#ifndef RFL_AVRO_READER_HPP_
#define RFL_AVRO_READER_HPP_

#include <avro.h>

#include <cstddef>
#include <exception>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl::avro {

/// Please refer to https://intel.github.io/tinyavro/current/index.html
struct Reader {
  struct AVROInputArray {
    const avro_value_t* val_;
  };

  struct AVROInputObject {
    const avro_value_t* val_;
  };

  struct AVROInputVar {
    const avro_value_t* val_;
  };

  using InputArrayType = AVROInputArray;
  using InputObjectType = AVROInputObject;
  using InputVarType = AVROInputVar;

  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_avro_obj(var); });

  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType& _arr) const noexcept;

  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType& _obj) const noexcept;

  bool is_empty(const InputVarType& _var) const noexcept;

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    const auto type = avro_value_get_type(_var.val_);
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      if (type != AVRO_STRING) {
        return Error("Could not cast to string.");
      }
      const char* c_str = nullptr;
      size_t size = 0;
      avro_value_get_string(_var.val_, &c_str, &size);
      return std::string(c_str, size);
      /*} else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                        rfl::Bytestring>()) {
        if (!avro_value_is_byte_string(&_var.val_)) {
          return Error("Could not cast to bytestring.");
        }
        rfl::Bytestring bstr;
        const auto err = get_bytestring(&_var.val_, &bstr);
        if (err != AvroNoError) {
          return Error(avro_error_string(err));
        }
        return bstr;*/
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (type != AVRO_BOOLEAN) {
        return rfl::Error("Could not cast to boolean.");
      }
      int result = 0;
      avro_value_get_boolean(_var.val_, &result);
      return (result != 0);
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_integral<std::remove_cvref_t<T>>()) {
      if (type == AVRO_DOUBLE) {
        double result = 0.0;
        const auto err = avro_value_get_double(_var.val_, &result);
        return static_cast<T>(result);
      } else if (type == AVRO_INT32) {
        int32_t result = 0;
        avro_value_get_int(_var.val_, &result);
        return static_cast<T>(result);
      } else if (type == AVRO_INT64) {
        int64_t result = 0;
        avro_value_get_int64(_var.val_, &result);
        return static_cast<T>(result);
      } else if (type == AVRO_FLOAT) {
        double result = 0.0;
        const auto err = avro_value_get_float(_var.val_, &result);
        return static_cast<T>(result);
      }
      return rfl::Error(
          "Could not cast to numeric value. The type must be integral, float "
          "or double.");

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(const InputVarType& _var) const noexcept;

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept;

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    size_t size = 0;
    avro_value_get_size(_arr.val_, &size);
    for (size_t ix = 0; ix < size; ++ix) {
      avro_value_t element;
      avro_value_get_by_index(_arr.val_, ix, &element, nullptr);
      const auto err = _array_reader.read(InputVarType{&element});
      if (err) {
        return err;
      }
    }
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    size_t size = 0;
    avro_value_get_size(_arr.val_, &size);
    for (size_t ix = 0; ix < size; ++ix) {
      avro_value_t element;
      const char* key = nullptr;
      avro_value_get_by_index(_obj.val_, ix, &element, &key);
      _object_reader.read(std::string_view(key), InputVarType{&element});
    }
    return std::nullopt;
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_avro_obj(_var);
    } catch (std::exception& e) {
      return rfl::Error(e.what());
    }
  }
};

}  // namespace rfl::avro

#endif
