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
#include "../internal/is_literal.hpp"
#include "../parsing/schemaful/IsSchemafulReader.hpp"

namespace rfl::avro {

struct Reader {
  struct AVROInputArray {
    const avro_value_t* val_;
  };

  struct AVROInputObject {
    const avro_value_t* val_;
  };

  struct AVROInputMap {
    const avro_value_t* val_;
  };

  struct AVROInputUnion {
    const avro_value_t* val_;
  };

  struct AVROInputVar {
    const avro_value_t* val_;
  };

  using InputArrayType = AVROInputArray;
  using InputObjectType = AVROInputObject;
  using InputMapType = AVROInputMap;
  using InputUnionType = AVROInputUnion;
  using InputVarType = AVROInputVar;

  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_avro_obj(var); });

  bool is_empty(const InputVarType& _var) const noexcept {
    return avro_value_get_type(_var.val_) == AVRO_NULL;
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    const auto type = avro_value_get_type(_var.val_);
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      const char* c_str = nullptr;
      size_t size = 0;
      const auto err = avro_value_get_string(_var.val_, &c_str, &size);
      if (err) {
        return error("Could not cast to string.");
      }
      if (size == 0) {
        return std::string("");
      }
      return std::string(c_str, size - 1);
    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      const void* ptr = nullptr;
      size_t size = 0;
      const auto err = avro_value_get_bytes(_var.val_, &ptr, &size);
      if (err) {
        return error("Could not cast to bytestring.");
      }
      const auto data = internal::ptr_cast<const std::byte*>(ptr);
      return rfl::Bytestring(data, data + size);
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (type != AVRO_BOOLEAN) {
        return rfl::error("Could not cast to boolean.");
      }
      int result = 0;
      avro_value_get_boolean(_var.val_, &result);
      return (result != 0);

    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      if (type == AVRO_DOUBLE) {
        double result = 0.0;
        const auto err = avro_value_get_double(_var.val_, &result);
        if (err) {
          return error("Could not cast to double.");
        }
        return static_cast<T>(result);
      } else if (type == AVRO_FLOAT) {
        float result = 0.0;
        const auto err = avro_value_get_float(_var.val_, &result);
        if (err) {
          return error("Could not cast to float.");
        }
        return static_cast<T>(result);
      } else {
        return rfl::error(
            "Could not cast to numeric value. The type must be float "
            "or double.");
      }

    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      if (type == AVRO_INT32) {
        int32_t result = 0;
        const auto err = avro_value_get_int(_var.val_, &result);
        if (err) {
          return error("Could not cast to int32.");
        }
        return static_cast<T>(result);
      } else if (type == AVRO_INT64) {
        int64_t result = 0;
        const auto err = avro_value_get_long(_var.val_, &result);
        if (err) {
          return error("Could not cast to int64.");
        }
        return static_cast<T>(result);
      } else {
        return rfl::error(
            "Could not cast to numeric value. The type must be integral.");
      }

    } else if constexpr (internal::is_literal_v<T>) {
      int value = 0;
      const auto err = avro_value_get_enum(_var.val_, &value);
      if (err) {
        return error("Could not cast to enum.");
      }
      return std::remove_cvref_t<T>::from_value(
          static_cast<typename std::remove_cvref_t<T>::ValueType>(value));

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    if (avro_value_get_type(_var.val_) != AVRO_ARRAY) {
      return error("Could not cast to an array.");
    }
    return InputArrayType{_var.val_};
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    const auto type = avro_value_get_type(_var.val_);
    if (type != AVRO_RECORD) {
      return error("Could not cast to an object.");
    }
    return InputObjectType{_var.val_};
  }

  rfl::Result<InputMapType> to_map(const InputVarType& _var) const noexcept {
    const auto type = avro_value_get_type(_var.val_);
    if (type != AVRO_MAP) {
      return error("Could not cast to a map.");
    }
    return InputMapType{_var.val_};
  }

  rfl::Result<InputUnionType> to_union(
      const InputVarType& _var) const noexcept {
    const auto type = avro_value_get_type(_var.val_);
    if (type != AVRO_UNION) {
      return error("Could not cast to a union.");
    }
    return InputUnionType{_var.val_};
  }

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

  template <class MapReader>
  std::optional<Error> read_map(const MapReader& _map_reader,
                                const InputMapType& _map) const noexcept {
    size_t size = 0;
    avro_value_get_size(_map.val_, &size);
    for (size_t ix = 0; ix < size; ++ix) {
      avro_value_t element;
      const char* key = nullptr;
      avro_value_get_by_index(_map.val_, ix, &element, &key);
      _map_reader.read(std::string_view(key), InputVarType{&element});
    }
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    size_t size = 0;
    avro_value_get_size(_obj.val_, &size);
    for (size_t ix = 0; ix < size; ++ix) {
      avro_value_t element;
      avro_value_get_by_index(_obj.val_, ix, &element, nullptr);
      _object_reader.read(static_cast<int>(ix), InputVarType{&element});
    }
    return std::nullopt;
  }

  template <class VariantType, class UnionReaderType>
  rfl::Result<VariantType> read_union(
      const InputUnionType& _union) const noexcept {
    int disc = 0;
    auto err = avro_value_get_discriminant(_union.val_, &disc);
    if (err) {
      return error("Could not get the discriminant.");
    }
    avro_value_t value;
    err = avro_value_get_current_branch(_union.val_, &value);
    if (err) {
      return error("Could not cast the union type.");
    }
    return UnionReaderType::read(*this, static_cast<size_t>(disc),
                                 InputVarType{&value});
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_avro_obj(_var);
    } catch (std::exception& e) {
      return rfl::error(e.what());
    }
  }
};

static_assert(parsing::schemaful::IsSchemafulReader<Reader>,
              "This must be a schemaful reader.");

}  // namespace rfl::avro

#endif
