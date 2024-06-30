#ifndef RFL_BSON_READER_HPP_
#define RFL_BSON_READER_HPP_

#include <bson/bson.h>

#include <array>
#include <concepts>
#include <cstddef>
#include <exception>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../Box.hpp"
#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl {
namespace bson {

/// Please refer to https://mongoc.org/libbson/current/api.html
struct Reader {
  struct BSONInputArray {
    const bson_value_t* val_;
  };

  struct BSONInputObject {
    const bson_value_t* val_;
  };

  struct BSONInputVar {
    const bson_value_t* val_;
  };

  using InputArrayType = BSONInputArray;
  using InputObjectType = BSONInputObject;
  using InputVarType = BSONInputVar;

  template <class T>
  static constexpr bool has_custom_constructor = (requires(InputVarType var) {
    T::from_bson_obj(var);
  });

  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType& _arr) const noexcept {
    bson_t b;
    bson_iter_t iter;
    const auto doc = _arr.val_->value.v_doc;
    if (bson_init_static(&b, doc.data, doc.data_len)) {
      if (bson_iter_init(&iter, &b)) {
        size_t i = 0;
        while (bson_iter_next(&iter)) {
          if (i == _idx) {
            return to_input_var(&iter);
          }
          ++i;
        }
      } else {
        return Error("Could not init the array iteration.");
      }
    } else {
      return Error("Could not init array.");
    }
    return Error("Index " + std::to_string(_idx) + " of of bounds.");
  }

  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType& _obj) const noexcept {
    bson_t b;
    bson_iter_t iter;
    const auto doc = _obj.val_->value.v_doc;
    if (bson_init_static(&b, doc.data, doc.data_len)) {
      if (bson_iter_init(&iter, &b)) {
        while (bson_iter_next(&iter)) {
          auto key = std::string(bson_iter_key(&iter));
          if (key == _name) {
            return to_input_var(&iter);
          }
        }
      }
    }
    return Error("No field named '" + _name + "' was found.");
  }

  bool is_empty(const InputVarType& _var) const noexcept {
    return _var.val_->value_type == BSON_TYPE_NULL;
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    const auto btype = _var.val_->value_type;
    const auto value = _var.val_->value;
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      switch (btype) {
        case BSON_TYPE_UTF8:
          return std::string(value.v_utf8.str, value.v_utf8.len);

        case BSON_TYPE_SYMBOL:
          return std::string(value.v_symbol.symbol, value.v_symbol.len);

        default:
          return rfl::Error(
              "Could not cast to string. The type must be UTF8 or symbol.");
      }
    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      if (btype != BSON_TYPE_BINARY) {
        return rfl::Error("Could not cast to bytestring.");
      }
      if (value.v_binary.subtype != BSON_SUBTYPE_BINARY) {
        return rfl::Error(
            "The BSON subtype must be a binary in order to read into a "
            "bytestring.");
      }
      return rfl::Bytestring(
          reinterpret_cast<const std::byte*>(value.v_binary.data),
          value.v_binary.data_len);
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (btype != BSON_TYPE_BOOL) {
        return rfl::Error("Could not cast to boolean.");
      }
      return value.v_bool;
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_integral<std::remove_cvref_t<T>>()) {
      switch (btype) {
        case BSON_TYPE_DOUBLE:
          return static_cast<T>(value.v_double);

        case BSON_TYPE_INT32:
          return static_cast<T>(value.v_int32);

        case BSON_TYPE_INT64:
          return static_cast<T>(value.v_int64);

        case BSON_TYPE_DATE_TIME:
          return static_cast<T>(value.v_datetime);

        default:
          return rfl::Error(
              "Could not cast to numeric value. The type must be double, "
              "int32, int64 or date_time.");
      }
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bson_oid_t>()) {
      if (btype != BSON_TYPE_OID) {
        return rfl::Error("Could not cast to OID.");
      }
      return value.v_oid;
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    const auto btype = _var.val_->value_type;
    if (btype != BSON_TYPE_ARRAY && btype != BSON_TYPE_DOCUMENT) {
      return Error("Could not cast to an array.");
    }
    return InputArrayType{_var.val_};
  }

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    bson_t b;
    bson_iter_t iter;
    const auto doc = _arr.val_->value.v_doc;
    if (bson_init_static(&b, doc.data, doc.data_len)) {
      if (bson_iter_init(&iter, &b)) {
        while (bson_iter_next(&iter)) {
          const auto err = _array_reader.read(to_input_var(&iter));
          if (err) {
            return err;
          }
        }
      } else {
        return Error("Could not init the array iteration.");
      }
    } else {
      return Error("Could not init array.");
    }
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    bson_t b;
    bson_iter_t iter;
    const auto doc = _obj.val_->value.v_doc;
    if (bson_init_static(&b, doc.data, doc.data_len)) {
      if (bson_iter_init(&iter, &b)) {
        while (bson_iter_next(&iter)) {
          const char* k = bson_iter_key(&iter);
          _object_reader.read(std::string_view(k), to_input_var(&iter));
        }
      } else {
        return Error("Could not init the object iteration.");
      }
    } else {
      return Error("Could not init object.");
    }
    return std::nullopt;
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    const auto btype = _var.val_->value_type;
    if (btype != BSON_TYPE_DOCUMENT) {
      return Error("Could not cast to a document.");
    }
    return InputObjectType{_var.val_};
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_bson_obj(_var);
    } catch (std::exception& e) {
      return rfl::Error(e.what());
    }
  }

 private:
  InputVarType to_input_var(bson_iter_t* _iter) const noexcept {
    return InputVarType{bson_iter_value(_iter)};
  }
};

}  // namespace bson
}  // namespace rfl

#endif  // JSON_PARSER_HPP_
