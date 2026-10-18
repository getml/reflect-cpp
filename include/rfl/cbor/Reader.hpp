#ifndef RFL_CBOR_READER_HPP_
#define RFL_CBOR_READER_HPP_

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
#include <type_traits>
#include <vector>

#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "../Vectorstring.hpp"
#include "../always_false.hpp"
#include "../common.hpp"
#include "../internal/ptr_cast.hpp"

namespace rfl::cbor {

/// Reader class for deserializing CBOR (Concise Binary Object Representation) data.
/// This class provides the interface for parsing CBOR format into C++ objects.
/// CBOR is a binary data serialization format that is more compact than JSON while
/// maintaining similar data model capabilities.
class RFL_API Reader {
 public:
  /// Represents a CBOR array during deserialization.
  /// Wraps a pointer to the underlying jsoncons JSON value representing the array.
  struct CBORInputArray {
    jsoncons::json* val_;
  };

  /// Represents a CBOR object during deserialization.
  /// Wraps a pointer to the underlying jsoncons JSON value representing the object.
  struct CBORInputObject {
    jsoncons::json* val_;
  };

  /// Represents a variant CBOR value during deserialization.
  /// Can hold any CBOR type (array, object, primitive, etc.).
  /// Wraps a pointer to the underlying jsoncons JSON value.
  struct CBORInputVar {
    jsoncons::json* val_;
  };

  using InputArrayType = CBORInputArray;
  using InputObjectType = CBORInputObject;
  using InputVarType = CBORInputVar;

  /// Constructs a new CBOR Reader.
  Reader() {}

  ~Reader() = default;

  /// Compile-time flag indicating whether type T has a custom constructor from CBOR.
  /// If true, the type provides a static from_cbor_obj() method for custom deserialization.
  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_cbor_obj(var); });

  /// Retrieves a field from a CBOR array by index.
  /// @param _idx The zero-based index of the element to retrieve
  /// @param _arr The CBOR array to read from
  /// @return Result containing the field value, or an error if index is out of bounds
  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType& _arr) const noexcept;

  /// Retrieves a field from a CBOR object by name.
  /// @param _name The name of the field to retrieve
  /// @param _obj The CBOR object to read from
  /// @return Result containing the field value, or an error if field is not found
  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType& _obj) const noexcept;

  /// Checks if a CBOR value is empty or null.
  /// @param _var The CBOR value to check
  /// @return true if the value is null, false otherwise
  bool is_empty(const InputVarType& _var) const noexcept;

  /// Converts a CBOR value to a basic C++ type.
  /// Supports strings, bytesstrings, booleans, floating-point numbers, and integers.
  /// @tparam T The target C++ type
  /// @param _var The CBOR value to convert
  /// @return Result containing the converted value, or an error if conversion fails
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

  /// Converts a CBOR value to an array.
  /// @param _var The CBOR value to convert
  /// @return Result containing the array, or an error if value is not an array
  rfl::Result<InputArrayType> to_array(const InputVarType& _var) const noexcept;

  /// Converts a CBOR value to an object.
  /// @param _var The CBOR value to convert
  /// @return Result containing the object, or an error if value is not an object
  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept;

  /// Reads all elements from a CBOR array using a provided array reader.
  /// The array reader's read() method is called for each element in the array.
  /// @tparam ArrayReader Type that provides a read() method for processing elements
  /// @param _array_reader The reader object used to process each array element
  /// @param _arr The CBOR array to read from
  /// @return std::nullopt on success, or an Error if reading fails
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

  /// Reads all key-value pairs from a CBOR object using a provided object reader.
  /// The object reader's read() method is called for each key-value pair.
  /// @tparam ObjectReader Type that provides a read() method for processing key-value pairs
  /// @param _object_reader The reader object used to process each field
  /// @param _obj The CBOR object to read from
  /// @return std::nullopt on success, or an Error if reading fails
  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    for (size_t i = 0; i < size(_obj.val_); ++i) {
      _object_reader.read(object_key(_obj.val_, i),
                          InputVarType{object_value(_obj.val_, i)});
    }
    return std::nullopt;
  }

  /// Uses a type's custom constructor to deserialize from CBOR.
  /// Calls the type's static from_cbor_obj() method for custom deserialization logic.
  /// @tparam T The type to construct, must have a from_cbor_obj() static method
  /// @param _var The CBOR value to deserialize from
  /// @return Result containing the constructed object, or an error if construction fails
  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_cbor_obj(_var);
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

}  // namespace rfl::cbor

#endif
