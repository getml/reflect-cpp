#ifndef RFL_CBOR_READER_HPP_
#define RFL_CBOR_READER_HPP_

#include <cstddef>
#include <exception>
#include <jsoncons/json.hpp>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/ptr_cast.hpp"

namespace rfl::cbor {

class Reader {
 public:
  struct CBORInputArray {
    jsoncons::json* val_;
  };

  struct CBORInputObject {
    jsoncons::json* val_;
  };

  struct CBORInputVar {
    jsoncons::json* val_;
  };

  using InputArrayType = CBORInputArray;
  using InputObjectType = CBORInputObject;
  using InputVarType = CBORInputVar;

  Reader() {}

  ~Reader() = default;

  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_cbor_obj(var); });

  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType& _arr) const noexcept {
    if (_idx >= _arr.val_->size()) {
      return error("Index out of range.");
    }
    return InputVarType{&_arr.val_->at(_idx)};
  }

  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType& _obj) const noexcept {
    for (auto& kv : _obj.val_->object_range()) {
      if (kv.key() == _name) {
        return InputVarType{&kv.value()};
      };
    }
    return error("Field name '" + _name + "' not found.");
  }

  bool is_empty(const InputVarType& _var) const noexcept {
    return _var.val_->is_null();
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      if (!_var.val_->is_string()) {
        return error("Could not cast to string.");
      }
      return _var.val_->as<std::string>();

    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      if (!_var.val_->is_byte_string()) {
        return error("Could not cast to bytestring.");
      }
      const auto vec = _var.val_->as<std::vector<uint8_t>>();
      const auto data = internal::ptr_cast<const std::byte*>(vec.data());
      return rfl::Bytestring(data, data + vec.size());

    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (!_var.val_->is_bool()) {
        return error("Could not cast to boolean.");
      }
      return _var.val_->as<bool>();

    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      if (_var.val_->is_double()) {
        return static_cast<T>(_var.val_->as<double>());
      }
      return error("Could not cast to double.");

    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      if (_var.val_->is_int64()) {
        return static_cast<T>(_var.val_->as<int64_t>());
      }
      if (_var.val_->is_uint64()) {
        return static_cast<T>(_var.val_->as<uint64_t>());
      }
      return error("Could not cast to integer.");

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    if (!_var.val_->is_array()) {
      return error("Could not cast to an array.");
    }
    return InputArrayType{_var.val_};
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    if (!_var.val_->is_object()) {
      return error("Could not cast to an object.");
    }
    return InputObjectType{_var.val_};
  }

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    for (auto& val : _arr.val_->array_range()) {
      const auto err = _array_reader.read(InputVarType{&val});
      if (err) {
        return err;
      }
    }
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    for (auto& kv : _obj.val_->object_range()) {
      _object_reader.read(kv.key(), InputVarType{&kv.value()});
    }
    return std::nullopt;
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_cbor_obj(_var);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }
};

}  // namespace rfl::cbor

#endif
