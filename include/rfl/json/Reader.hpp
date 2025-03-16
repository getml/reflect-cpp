#ifndef RFL_JSON_READER_HPP_
#define RFL_JSON_READER_HPP_

#if __has_include(<yyjson.h>)
#include <yyjson.h>
#else
#include "../thirdparty/yyjson.h"
#endif

#include <array>
#include <concepts>
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
namespace json {

struct Reader {
  struct YYJSONInputArray {
    YYJSONInputArray(yyjson_val* _val) : val_(_val) {}
    yyjson_val* val_;
  };

  struct YYJSONInputObject {
    YYJSONInputObject(yyjson_val* _val) : val_(_val) {}
    yyjson_val* val_;
  };

  struct YYJSONInputVar {
    YYJSONInputVar() : val_(nullptr) {}
    YYJSONInputVar(yyjson_val* _val) : val_(_val) {}
    yyjson_val* val_;
  };

  using InputArrayType = YYJSONInputArray;
  using InputObjectType = YYJSONInputObject;
  using InputVarType = YYJSONInputVar;

  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_json_obj(var); });

  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType _arr) const noexcept {
    const auto var = InputVarType(yyjson_arr_get(_arr.val_, _idx));
    if (!var.val_) {
      return error("Index " + std::to_string(_idx) + " of of bounds.");
    }
    return var;
  }

  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType _obj) const noexcept {
    const auto var = InputVarType(yyjson_obj_get(_obj.val_, _name.c_str()));
    if (!var.val_) {
      return error("Object contains no field named '" + _name + "'.");
    }
    return var;
  }

  bool is_empty(const InputVarType _var) const noexcept {
    return !_var.val_ || yyjson_is_null(_var.val_);
  }

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    yyjson_val* val;
    yyjson_arr_iter iter;
    yyjson_arr_iter_init(_arr.val_, &iter);
    while ((val = yyjson_arr_iter_next(&iter))) {
      const auto err = _array_reader.read(InputVarType(val));
      if (err) {
        return err;
      }
    }
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    yyjson_obj_iter iter;
    yyjson_obj_iter_init(_obj.val_, &iter);
    yyjson_val* key;
    while ((key = yyjson_obj_iter_next(&iter))) {
      const auto name = std::string_view(yyjson_get_str(key));
      _object_reader.read(name, InputVarType(yyjson_obj_iter_get_val(key)));
    }
    return std::nullopt;
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      const auto r = yyjson_get_str(_var.val_);
      if (r == NULL) {
        return error("Could not cast to string.");
      }
      return std::string(r);

    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (!yyjson_is_bool(_var.val_)) {
        return error("Could not cast to boolean.");
      }
      return yyjson_get_bool(_var.val_);

    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      if (!yyjson_is_num(_var.val_)) {
        return error("Could not cast to double.");
      }
      return static_cast<T>(yyjson_get_num(_var.val_));

    } else if constexpr (std::is_unsigned<std::remove_cvref_t<T>>()) {
      if (!yyjson_is_int(_var.val_)) {
        return error("Could not cast to int.");
      }
      return static_cast<T>(yyjson_get_uint(_var.val_));

    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      if (!yyjson_is_int(_var.val_)) {
        return error("Could not cast to int.");
      }
      return static_cast<T>(yyjson_get_sint(_var.val_));

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(const InputVarType _var) const noexcept {
    if (!yyjson_is_arr(_var.val_)) {
      return error("Could not cast to array!");
    }
    return InputArrayType(_var.val_);
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType _var) const noexcept {
    if (!yyjson_is_obj(_var.val_)) {
      return error("Could not cast to object!");
    }
    return InputObjectType(_var.val_);
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType _var) const noexcept {
    try {
      return T::from_json_obj(_var);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }
};

}  // namespace json
}  // namespace rfl

#endif
