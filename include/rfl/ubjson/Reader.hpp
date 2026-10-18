#ifndef RFL_UBJSON_READER_HPP_
#define RFL_UBJSON_READER_HPP_

#include <cstddef>
#include <cstdint>
#include <exception>
#include <string_view>
#ifdef RFL_JSONCONS_MODULE_INTERFACE
#include "../internal/jsoncons_module_fwd.hpp"
#else
#include <jsoncons/json.hpp>
#endif
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "../Vectorstring.hpp"
#include "../always_false.hpp"
#include "../common.hpp"
#include "../internal/ptr_cast.hpp"

namespace rfl::ubjson {

class RFL_API Reader {
 public:
  struct UBJSONInputArray {
    jsoncons::json* val_;
  };

  struct UBJSONInputObject {
    jsoncons::json* val_;
  };

  struct UBJSONInputVar {
    jsoncons::json* val_;
  };

  using InputArrayType = UBJSONInputArray;
  using InputObjectType = UBJSONInputObject;
  using InputVarType = UBJSONInputVar;

  Reader() {}

  ~Reader() = default;

  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_ubjson_obj(var); });

  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType& _arr) const noexcept;

  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType& _obj) const noexcept;

  bool is_empty(const InputVarType& _var) const noexcept;

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    using Type = std::remove_cvref_t<T>;
    if constexpr (std::is_same_v<Type, std::string>) {
      return read_string(_var);
    } else if constexpr (std::is_same_v<Type, rfl::Bytestring> ||
                         std::is_same_v<Type, rfl::Vectorstring>) {
      return read_bytes(_var, std::is_same_v<Type, rfl::Vectorstring>)
          .transform([](const auto& bytes) {
            using Value = typename Type::value_type;
            const auto begin = internal::ptr_cast<const Value*>(bytes.data());
            return Type(begin, begin + bytes.size());
          });
    } else if constexpr (std::is_same_v<Type, bool>) {
      return read_bool(_var);
    } else if constexpr (std::is_floating_point_v<Type>) {
      return read_double(_var).transform(
          [](double value) { return static_cast<T>(value); });
    } else if constexpr (std::is_integral_v<Type>) {
      if (is_int64(_var)) return static_cast<T>(read_int64(_var));
      if (is_uint64(_var)) return static_cast<T>(read_uint64(_var));
      return error("Could not cast to integer.");
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
    for (size_t i = 0; i < size(_arr.val_); ++i) {
      const auto err = _array_reader.read(InputVarType{array_at(_arr.val_, i)});
      if (err) {
        return err;
      }
    }
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    for (size_t i = 0; i < size(_obj.val_); ++i) {
      _object_reader.read(object_key(_obj.val_, i),
                          InputVarType{object_value(_obj.val_, i)});
    }
    return std::nullopt;
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_ubjson_obj(_var);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

 private:
  Result<std::string> read_string(const InputVarType& _var) const;
  Result<std::vector<uint8_t>> read_bytes(const InputVarType& _var,
                                          bool _vectorstring) const;
  Result<bool> read_bool(const InputVarType& _var) const;
  Result<double> read_double(const InputVarType& _var) const;
  bool is_int64(const InputVarType& _var) const;
  bool is_uint64(const InputVarType& _var) const;
  int64_t read_int64(const InputVarType& _var) const;
  uint64_t read_uint64(const InputVarType& _var) const;
  size_t size(jsoncons::json* _value) const;
  jsoncons::json* array_at(jsoncons::json* _value, size_t _index) const;
  std::string_view object_key(jsoncons::json* _value, size_t _index) const;
  jsoncons::json* object_value(jsoncons::json* _value, size_t _index) const;
};

}  // namespace rfl::ubjson

#endif
