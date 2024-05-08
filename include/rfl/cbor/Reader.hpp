#ifndef RFL_CBOR_READER_HPP_
#define RFL_CBOR_READER_HPP_

#include <cbor.h>

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
namespace cbor {

/// Please refer to https://intel.github.io/tinycbor/current/index.html
struct Reader {
  struct CBORInputArray {
    CborValue* val_;
  };

  struct CBORInputObject {
    CborValue* val_;
  };

  struct CBORInputVar {
    CborValue* val_;
  };

  using InputArrayType = CBORInputArray;
  using InputObjectType = CBORInputObject;
  using InputVarType = CBORInputVar;

  template <class T>
  static constexpr bool has_custom_constructor = (requires(InputVarType var) {
    T::from_cbor_obj(var);
  });

  rfl::Result<InputVarType> get_field(
      const std::string& _name, const InputObjectType& _obj) const noexcept {
    CborValue val;
    auto buffer = std::vector<char>();
    auto err = cbor_value_enter_container(_obj.val_, &val);
    if (err != CborNoError) {
      return Error(cbor_error_string(err));
    }
    size_t length = 0;
    err = cbor_value_get_map_length(_obj.val_, &length);
    if (err != CborNoError) {
      return Error(cbor_error_string(err));
    }
    for (size_t i = 0; i < length; ++i) {
      if (!cbor_value_is_text_string(&val)) {
        return Error("Expected the key to be a string value.");
      }
      err = get_string(&val, &buffer);
      if (err != CborNoError) {
        return Error(cbor_error_string(err));
      }
      err = cbor_value_advance(&val);
      if (err != CborNoError) {
        return Error(cbor_error_string(err));
      }
      if (_name == buffer.data()) {
        return to_input_var(&val);
      }
      err = cbor_value_advance(&val);
      if (err != CborNoError) {
        return Error(cbor_error_string(err));
      }
    }
    return Error("No field named '" + _name + "' was found.");
  }

  bool is_empty(const InputVarType& _var) const noexcept {
    return cbor_value_is_null(_var.val_);
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      if (!cbor_value_is_text_string(_var.val_)) {
        return Error("Could not cast to string.");
      }
      std::vector<char> buffer;
      const auto err = get_string(_var.val_, &buffer);
      if (err != CborNoError) {
        return Error(cbor_error_string(err));
      }
      return std::string(buffer.data());
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (!cbor_value_is_boolean(_var.val_)) {
        return rfl::Error("Could not cast to boolean.");
      }
      bool result = false;
      const auto err = cbor_value_get_boolean(_var.val_, &result);
      if (err != CborNoError) {
        return Error(cbor_error_string(err));
      }
      return result;
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_integral<std::remove_cvref_t<T>>()) {
      if (cbor_value_is_integer(_var.val_)) {
        std::int64_t result = 0;
        const auto err = cbor_value_get_int64(_var.val_, &result);
        if (err != CborNoError) {
          return Error(cbor_error_string(err));
        }
        return static_cast<T>(result);
      } else if (cbor_value_is_float(_var.val_)) {
        float result = 0.0;
        const auto err = cbor_value_get_float(_var.val_, &result);
        if (err != CborNoError) {
          return Error(cbor_error_string(err));
        }
        return static_cast<T>(result);
      } else if (cbor_value_is_double(_var.val_)) {
        double result = 0.0;
        const auto err = cbor_value_get_double(_var.val_, &result);
        if (err != CborNoError) {
          return Error(cbor_error_string(err));
        }
        return static_cast<T>(result);
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
    if (!cbor_value_is_array(_var.val_)) {
      return Error("Could not cast to an array.");
    }
    return InputArrayType{_var.val_};
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    if (!cbor_value_is_map(_var.val_)) {
      return Error("Could not cast to an object.");
    }
    return InputObjectType{_var.val_};
  }

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    CborValue val;
    auto buffer = std::vector<char>();
    auto err = cbor_value_enter_container(_arr.val_, &val);
    if (err != CborNoError && err != CborErrorOutOfMemory) {
      return Error(cbor_error_string(err));
    }
    size_t length = 0;
    err = cbor_value_get_array_length(_arr.val_, &length);
    if (err != CborNoError && err != CborErrorOutOfMemory) {
      return Error(cbor_error_string(err));
    }
    for (size_t i = 0; i < length; ++i) {
      const auto err2 = _array_reader.read(to_input_var(&val));
      if (err2) {
        return err2;
      }
      err = cbor_value_advance(&val);
      if (err != CborNoError && err != CborErrorOutOfMemory) {
        return Error(cbor_error_string(err));
      }
    }
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    size_t length = 0;
    auto err = cbor_value_get_map_length(_obj.val_, &length);
    if (err != CborNoError) {
      return Error(cbor_error_string(err));
    }

    CborValue val;
    err = cbor_value_enter_container(_obj.val_, &val);
    if (err != CborNoError) {
      return Error(cbor_error_string(err));
    }

    auto buffer = std::vector<char>();

    for (size_t i = 0; i < length; ++i) {
      err = get_string(&val, &buffer);
      if (err != CborNoError) {
        return Error(cbor_error_string(err));
      }
      err = cbor_value_advance(&val);
      if (err != CborNoError) {
        return Error(cbor_error_string(err));
      }
      const auto name = std::string_view(buffer.data(), buffer.size() - 1);
      _object_reader.read(name, InputVarType{&val});
      cbor_value_advance(&val);
    }

    return std::nullopt;
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_cbor_obj(_var);
    } catch (std::exception& e) {
      return rfl::Error(e.what());
    }
  }

 private:
  CborError get_string(const CborValue* _ptr,
                       std::vector<char>* _buffer) const noexcept {
    size_t length = 0;
    auto err = cbor_value_get_string_length(_ptr, &length);
    if (err != CborNoError && err != CborErrorOutOfMemory) {
      return err;
    }
    _buffer->resize(length + 1);
    (*_buffer)[length] = '\0';
    return cbor_value_copy_text_string(_ptr, _buffer->data(), &length, NULL);
  }

  InputVarType to_input_var(CborValue* _ptr) const noexcept {
    values_->emplace_back(rfl::Box<CborValue>::make(*_ptr));
    auto* last_value = values_->back().get();
    return InputVarType{last_value};
  }

 private:
  /// Contains the values inside the object.
  rfl::Box<std::vector<rfl::Box<CborValue>>> values_;
};

}  // namespace cbor
}  // namespace rfl

#endif  // JSON_PARSER_HPP_
