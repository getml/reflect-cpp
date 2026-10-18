#ifndef RFL_NATIVE_ARROW_TYPES_HPP_
#define RFL_NATIVE_ARROW_TYPES_HPP_

#include <arrow/api.h>

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

#include "rfl/Ref.hpp"
#include "rfl/Timestamp.hpp"
#include "rfl/always_false.hpp"
#include "rfl/concepts.hpp"
#include "rfl/internal/StringLiteral.hpp"
#include "rfl/internal/ptr_cast.hpp"
#include "rfl/parsing/tabular/ArrowBridge.hpp"
#include "rfl/parsing/tabular/arrow_builder_operations.hpp"

namespace rfl::parsing::tabular {

template <class T, SerializationType _s>
struct NativeArrowTypes;

/**
 * @brief Transforms a numerical array to the desired type T.
 * @tparam T The desired type.
 * @tparam _s The serialization type.
 * @param _arr The array to transform.
 * @return The transformed array or an error.
 */
template <class T, SerializationType _s>
Result<Ref<typename NativeArrowTypes<T, _s>::ArrayType>>
native_transform_numerical_array(
    const std::shared_ptr<arrow::Array>& _arr) noexcept;

template <SerializationType _s>
struct NativeArrowTypes<bool, _s> {
  using ArrayType = arrow::BooleanArray;
  using BuilderType = arrow::BooleanBuilder;

  /**
   * @brief Returns the Arrow data type for boolean.
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::boolean(); }

  /**
   * @brief Adds a boolean value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const bool _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the boolean array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The boolean array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    if (_arr->type()->Equals(data_type())) {
      return Ref<ArrayType>::make(std::static_pointer_cast<ArrayType>(_arr));
    } else {
      return error("Expected boolean array, got " + _arr->type()->ToString() +
                   ".");
    }
  }

  /**
   * @brief Returns the boolean value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The boolean value.
   */
  static Result<bool> get_value(const Ref<ArrayType>& _chunk,
                                const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  /**
   * @brief Creates a builder for boolean arrays.
   * @return The builder.
   */
  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct NativeArrowTypes<uint8_t, _s> {
  using ArrayType = arrow::UInt8Array;
  using BuilderType = arrow::UInt8Builder;
  using T = uint8_t;

  /**
   * @brief Returns the Arrow data type for uint8.
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::uint8(); }

  /**
   * @brief Adds a uint8 value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the uint8 array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The uint8 array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return native_transform_numerical_array<T, _s>(_arr);
  }

  /**
   * @brief Returns the uint8 value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The uint8 value.
   */
  static Result<uint8_t> get_value(const Ref<ArrayType>& _chunk,
                                   const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  /**
   * @brief Creates a builder for uint8 arrays.
   * @return The builder.
   */
  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct NativeArrowTypes<uint16_t, _s> {
  using ArrayType = arrow::UInt16Array;
  using BuilderType = arrow::UInt16Builder;
  using T = uint16_t;

  /**
   * @brief Returns the Arrow data type for uint16.
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::uint16(); }

  /**
   * @brief Adds a uint16 value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the uint16 array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The uint16 array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return native_transform_numerical_array<T, _s>(_arr);
  }

  /**
   * @brief Returns the uint16 value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The uint16 value.
   */
  static Result<uint16_t> get_value(const Ref<ArrayType>& _chunk,
                                    const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  /**
   * @brief Creates a builder for uint16 arrays.
   * @return The builder.
   */
  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct NativeArrowTypes<uint32_t, _s> {
  using ArrayType = arrow::UInt32Array;
  using BuilderType = arrow::UInt32Builder;
  using T = uint32_t;

  /**
   * @brief Returns the Arrow data type for uint32.
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::uint32(); }

  /**
   * @brief Adds a uint32 value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the uint32 array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The uint32 array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return native_transform_numerical_array<T, _s>(_arr);
  }

  /**
   * @brief Returns the uint32 value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The uint32 value.
   */
  static Result<uint32_t> get_value(const Ref<ArrayType>& _chunk,
                                    const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  /**
   * @brief Creates a builder for uint32 arrays.
   * @return The builder.
   */
  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct NativeArrowTypes<uint64_t, _s> {
  using ArrayType = arrow::UInt64Array;
  using BuilderType = arrow::UInt64Builder;
  using T = uint64_t;

  /**
   * @brief Returns the Arrow data type for uint64.
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::uint64(); }

  /**
   * @brief Adds a uint64 value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the uint64 array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The uint64 array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return native_transform_numerical_array<T, _s>(_arr);
  }

  /**
   * @brief Returns the uint64 value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The uint64 value.
   */
  static Result<uint64_t> get_value(const Ref<ArrayType>& _chunk,
                                    const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  /**
   * @brief Creates a builder for uint64 arrays.
   * @return The builder.
   */
  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct NativeArrowTypes<int8_t, _s> {
  using ArrayType = arrow::Int8Array;
  using BuilderType = arrow::Int8Builder;
  using T = int8_t;

  /**
   * @brief Returns the Arrow data type for int8.
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::int8(); }

  /**
   * @brief Adds an int8 value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the int8 array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The int8 array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return native_transform_numerical_array<T, _s>(_arr);
  }

  /**
   * @brief Returns the int8 value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The int8 value.
   */
  static Result<int8_t> get_value(const Ref<ArrayType>& _chunk,
                                  const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  /**
   * @brief Creates a builder for int8 arrays.
   * @return The builder.
   */
  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct NativeArrowTypes<int16_t, _s> {
  using ArrayType = arrow::Int16Array;
  using BuilderType = arrow::Int16Builder;
  using T = int16_t;

  /**
   * @brief Returns the Arrow data type for int16.
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::int16(); }

  /**
   * @brief Adds an int16 value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the int16 array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The int16 array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return native_transform_numerical_array<T, _s>(_arr);
  }

  /**
   * @brief Returns the int16 value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The int16 value.
   */
  static Result<int16_t> get_value(const Ref<ArrayType>& _chunk,
                                   const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  /**
   * @brief Creates a builder for int16 arrays.
   * @return The builder.
   */
  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct NativeArrowTypes<int32_t, _s> {
  using ArrayType = arrow::Int32Array;
  using BuilderType = arrow::Int32Builder;
  using T = int32_t;

  /**
   * @brief Returns the Arrow data type for int32.
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::int32(); }

  /**
   * @brief Adds an int32 value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the int32 array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The int32 array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return native_transform_numerical_array<T, _s>(_arr);
  }

  /**
   * @brief Returns the int32 value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The int32 value.
   */
  static Result<int32_t> get_value(const Ref<ArrayType>& _chunk,
                                   const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  /**
   * @brief Creates a builder for int32 arrays.
   * @return The builder.
   */
  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct NativeArrowTypes<int64_t, _s> {
  using ArrayType = arrow::Int64Array;
  using BuilderType = arrow::Int64Builder;
  using T = int64_t;

  /**
   * @brief Returns the Arrow data type for int64.
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::int64(); }

  /**
   * @brief Adds an int64 value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the int64 array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The int64 array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return native_transform_numerical_array<T, _s>(_arr);
  }

  /**
   * @brief Returns the int64 value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The int64 value.
   */
  static Result<int64_t> get_value(const Ref<ArrayType>& _chunk,
                                   const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  /**
   * @brief Creates a builder for int64 arrays.
   * @return The builder.
   */
  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct NativeArrowTypes<float, _s> {
  using ArrayType = arrow::FloatArray;
  using BuilderType = arrow::FloatBuilder;
  using T = float;

  /**
   * @brief Returns the Arrow data type for float32.
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::float32(); }

  /**
   * @brief Adds a float value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the float array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The float array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return native_transform_numerical_array<T, _s>(_arr);
  }

  /**
   * @brief Returns the float value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The float value.
   */
  static Result<float> get_value(const Ref<ArrayType>& _chunk,
                                 const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  /**
   * @brief Creates a builder for float arrays.
   * @return The builder.
   */
  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct NativeArrowTypes<double, _s> {
  using ArrayType = arrow::DoubleArray;
  using BuilderType = arrow::DoubleBuilder;
  using T = double;

  /**
   * @brief Returns the Arrow data type for float64.
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::float64(); }

  /**
   * @brief Adds a double value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the double array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The double array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return native_transform_numerical_array<T, _s>(_arr);
  }

  /**
   * @brief Returns the double value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The double value.
   */
  static Result<double> get_value(const Ref<ArrayType>& _chunk,
                                  const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  /**
   * @brief Creates a builder for double arrays.
   * @return The builder.
   */
  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct NativeArrowTypes<std::string, _s> {
  using ArrayType = arrow::StringArray;
  using BuilderType = arrow::StringBuilder;

  /**
   * @brief Returns the Arrow data type for string (utf8).
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::utf8(); }

  /**
   * @brief Adds a string value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    const auto status = append_string(_builder, _val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the string array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The string array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    if (_arr->type()->Equals(data_type())) {
      return Ref<ArrayType>::make(std::static_pointer_cast<ArrayType>(_arr));
    } else {
      return error("Expected string array, got " + _arr->type()->ToString() +
                   ".");
    }
  }

  /**
   * @brief Returns the string value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The string value.
   */
  static Result<std::string> get_value(const Ref<ArrayType>& _chunk,
                                       const int64_t _ix) {
    return std::string(_chunk->Value(_ix));
  }

  /**
   * @brief Creates a builder for string arrays.
   * @return The builder.
   */
  static auto make_builder() { return BuilderType(); }
};

template <class T, SerializationType _s>
  requires concepts::ContiguousByteContainer<T>
struct NativeArrowTypes<T, _s> {
  using ArrayType = arrow::BinaryArray;
  using BuilderType = arrow::BinaryBuilder;

  /**
   * @brief Returns the Arrow data type for binary.
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::binary(); }

  /**
   * @brief Adds a binary value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    const auto status = _builder->Append(
        internal::ptr_cast<const uint8_t*>(_val.data()), _val.size());
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the binary array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The binary array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    if (_arr->type()->Equals(data_type())) {
      return Ref<ArrayType>::make(std::static_pointer_cast<ArrayType>(_arr));

    } else if (_arr->type()->Equals(arrow::utf8())) {
      return transform_string(
          std::static_pointer_cast<arrow::StringArray>(_arr));

    } else {
      return error("Expected binary or string array, got " +
                   _arr->type()->ToString() + ".");
    }
  }

  /**
   * @brief Returns the binary value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The binary value.
   */
  static Result<T> get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    const auto begin = internal::ptr_cast<const typename T::value_type*>(
        _chunk->Value(_ix).data());
    return T(begin, begin + _chunk->Value(_ix).size());
  }

  /**
   * @brief Creates a builder for binary arrays.
   * @return The builder.
   */
  static auto make_builder() { return BuilderType(); }

  /**
   * @brief Transforms a string array to a binary array.
   * @param _arr The string array.
   * @return The binary array or an error.
   */
  static Result<Ref<arrow::BinaryArray>> transform_string(
      const std::shared_ptr<arrow::StringArray>& _arr) noexcept {
    if (!_arr) {
      return error(
          "transform_string: std::shared_ptr not set. This is a "
          "bug, please report.");
    }

    auto builder = arrow::BinaryBuilder();

    for (int64_t i = 0; i < _arr->length(); ++i) {
      if (_arr->IsNull(i)) {
        const auto status = builder.AppendNull();
        if (!status.ok()) {
          return error(status.message());
        }
      } else {
        const std::string_view s = _arr->Value(i);
        const auto status = builder.Append(
            internal::ptr_cast<const uint8_t*>(s.data()), s.size());
        if (!status.ok()) {
          return error(status.message());
        }
      }
    }

    std::shared_ptr<arrow::Array> res;
    const auto status = builder.Finish(&res);
    return Ref<arrow::BinaryArray>::make(
        std::static_pointer_cast<arrow::BinaryArray>(res));
  }
};

template <internal::StringLiteral _format, SerializationType _s>
struct NativeArrowTypes<Timestamp<_format>, _s> {
  enum class TimeUnit { day, second, milli, micro, nano, string };

  using ArrayType = arrow::TimestampArray;
  using BuilderType = arrow::TimestampBuilder;

  /**
   * @brief Returns the Arrow data type for timestamps (milli).
   * @return The Arrow data type.
   */
  static auto data_type() { return arrow::timestamp(arrow::TimeUnit::MILLI); }

  /**
   * @brief Adds a timestamp value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val.to_time_t() * 1000);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  /**
   * @brief Returns the timestamp array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The timestamp array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    if (_arr->type()->Equals(data_type())) {
      return Ref<ArrayType>::make(std::static_pointer_cast<ArrayType>(_arr));

    } else if (_arr->type()->Equals(
                   arrow::timestamp(arrow::TimeUnit::SECOND))) {
      return transform_time_stamp<TimeUnit::second>(
          std::static_pointer_cast<arrow::TimestampArray>(_arr));

    } else if (_arr->type()->Equals(arrow::timestamp(arrow::TimeUnit::MICRO))) {
      return transform_time_stamp<TimeUnit::micro>(
          std::static_pointer_cast<arrow::TimestampArray>(_arr));

    } else if (_arr->type()->Equals(arrow::timestamp(arrow::TimeUnit::NANO))) {
      return transform_time_stamp<TimeUnit::nano>(
          std::static_pointer_cast<arrow::TimestampArray>(_arr));

    } else if (_arr->type()->Equals(arrow::date32())) {
      return transform_time_stamp<TimeUnit::day>(
          std::static_pointer_cast<arrow::Date32Array>(_arr));

    } else if (_arr->type()->Equals(arrow::date64())) {
      return transform_time_stamp<TimeUnit::milli>(
          std::static_pointer_cast<arrow::Date64Array>(_arr));

    } else if (_arr->type()->Equals(arrow::utf8())) {
      return transform_time_stamp<TimeUnit::string>(
          std::static_pointer_cast<arrow::StringArray>(_arr));

    } else {
      return error("Expected timestamp, date32, date64 or string array, got " +
                   _arr->type()->ToString() + ".");
    }
  }

  /**
   * @brief Returns the timestamp value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The timestamp value.
   */
  static Result<Timestamp<_format>> get_value(const Ref<ArrayType>& _chunk,
                                              const int64_t _ix) {
    return Timestamp<_format>(_chunk->Value(_ix) / 1000);
  }

  /**
   * @brief Creates a builder for timestamp arrays.
   * @return The builder.
   */
  static auto make_builder() {
    return BuilderType(data_type(), arrow::default_memory_pool());
  }

  /**
   * @brief Transforms an array with different time units to a timestamp array.
   * @tparam _unit The time unit of the source array.
   * @tparam SourceArrayType The type of the source array.
   * @param _arr The source array.
   * @return The timestamp array or an error.
   */
  template <TimeUnit _unit, class SourceArrayType>
  static Result<Ref<arrow::TimestampArray>> transform_time_stamp(
      const std::shared_ptr<SourceArrayType>& _arr) noexcept {
    if (!_arr) {
      return error(
          "transform_time_stamp: std::shared_ptr not set. This is a "
          "bug, please report.");
    }

    auto builder =
        arrow::TimestampBuilder(data_type(), arrow::default_memory_pool());

    for (int64_t i = 0; i < _arr->length(); ++i) {
      if (_arr->IsNull(i)) {
        const auto status = builder.AppendNull();
        if (!status.ok()) {
          return error(status.message());
        }
      } else {
        if constexpr (_unit == TimeUnit::day) {
          const auto status = builder.Append(
              static_cast<int64_t>(_arr->Value(i)) * 1000 * 24 * 60 * 60);
          if (!status.ok()) {
            return error(status.message());
          }
        } else if constexpr (_unit == TimeUnit::second) {
          const auto status =
              builder.Append(static_cast<int64_t>(_arr->Value(i) * 1000));
          if (!status.ok()) {
            return error(status.message());
          }
        } else if constexpr (_unit == TimeUnit::milli) {
          const auto status =
              builder.Append(static_cast<int64_t>(_arr->Value(i)));
          if (!status.ok()) {
            return error(status.message());
          }
        } else if constexpr (_unit == TimeUnit::micro) {
          const auto status =
              builder.Append(static_cast<int64_t>(_arr->Value(i) / 1000));
          if (!status.ok()) {
            return error(status.message());
          }
        } else if constexpr (_unit == TimeUnit::nano) {
          const auto status =
              builder.Append(static_cast<int64_t>(_arr->Value(i) / 1000000));
          if (!status.ok()) {
            return error(status.message());
          }
        } else if constexpr (_unit == TimeUnit::string) {
          const auto ts = Timestamp<_format>::make(std::string(_arr->Value(i)));
          if (!ts) {
            return error(ts.error().what());
          }
          const auto status = builder.Append(ts->to_time_t() * 1000);
          if (!status.ok()) {
            return error(status.message());
          }
        } else {
          static_assert(rfl::always_false_v<SourceArrayType>,
                        "Unsupported time unit.");
        }
      }
    }

    std::shared_ptr<arrow::Array> res;
    const auto status = builder.Finish(&res);
    return Ref<arrow::TimestampArray>::make(
        std::static_pointer_cast<arrow::TimestampArray>(res));
  }
};

/**
 * @brief Implementation of numerical array transformation.
 * @tparam T The target type.
 * @tparam _s The serialization type.
 * @tparam SourceArrayType The type of the source array.
 * @param _arr The source array.
 * @return The transformed array or an error.
 */
template <class T, SerializationType _s, class SourceArrayType>
Result<Ref<typename NativeArrowTypes<T, _s>::ArrayType>>
native_transform_numerical_array_impl(
    const std::shared_ptr<SourceArrayType>& _arr) noexcept {
  if (!_arr) {
    return error(
        "native_transform_numerical_array_impl: std::shared_ptr not set. This "
        "is a "
        "bug, please report.");
  }

  auto builder = NativeArrowTypes<T, _s>::make_builder();

  for (int64_t i = 0; i < _arr->length(); ++i) {
    if (_arr->IsNull(i)) {
      const auto status = builder.AppendNull();
      if (!status.ok()) {
        return error(status.message());
      }
    } else {
      const auto status = builder.Append(static_cast<T>(_arr->Value(i)));
      if (!status.ok()) {
        return error(status.message());
      }
    }
  }

  using TargetArrayType = typename NativeArrowTypes<T, _s>::ArrayType;

  std::shared_ptr<arrow::Array> res;
  const auto status = builder.Finish(&res);
  return Ref<TargetArrayType>::make(
      std::static_pointer_cast<TargetArrayType>(res));
}

/**
 * @brief Transforms a numerical array to the desired type T.
 * @tparam T The desired type.
 * @tparam _s The serialization type.
 * @param _arr The array to transform.
 * @return The transformed array or an error.
 */
template <class T, SerializationType _s>
Result<Ref<typename NativeArrowTypes<T, _s>::ArrayType>>
native_transform_numerical_array(
    const std::shared_ptr<arrow::Array>& _arr) noexcept {
  if (!_arr) {
    return error(
        "Could not transform the numerical array. std::shared_ptr not set.");
  }

  using ArrayType = typename NativeArrowTypes<T, _s>::ArrayType;

  if (_arr->type()->Equals(NativeArrowTypes<T, _s>::data_type())) {
    return Ref<ArrayType>::make(std::static_pointer_cast<ArrayType>(_arr));

  } else if (_arr->type()->Equals(NativeArrowTypes<uint8_t, _s>::data_type())) {
    return native_transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<
            typename NativeArrowTypes<uint8_t, _s>::ArrayType>(_arr));

  } else if (_arr->type()->Equals(
                 NativeArrowTypes<uint16_t, _s>::data_type())) {
    return native_transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<
            typename NativeArrowTypes<uint16_t, _s>::ArrayType>(_arr));

  } else if (_arr->type()->Equals(
                 NativeArrowTypes<uint32_t, _s>::data_type())) {
    return native_transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<
            typename NativeArrowTypes<uint32_t, _s>::ArrayType>(_arr));

  } else if (_arr->type()->Equals(
                 NativeArrowTypes<uint64_t, _s>::data_type())) {
    return native_transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<
            typename NativeArrowTypes<uint64_t, _s>::ArrayType>(_arr));

  } else if (_arr->type()->Equals(NativeArrowTypes<int8_t, _s>::data_type())) {
    return native_transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<
            typename NativeArrowTypes<int8_t, _s>::ArrayType>(_arr));

  } else if (_arr->type()->Equals(NativeArrowTypes<int16_t, _s>::data_type())) {
    return native_transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<
            typename NativeArrowTypes<int16_t, _s>::ArrayType>(_arr));

  } else if (_arr->type()->Equals(NativeArrowTypes<int32_t, _s>::data_type())) {
    return native_transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<
            typename NativeArrowTypes<int32_t, _s>::ArrayType>(_arr));

  } else if (_arr->type()->Equals(NativeArrowTypes<int64_t, _s>::data_type())) {
    return native_transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<
            typename NativeArrowTypes<int64_t, _s>::ArrayType>(_arr));

  } else if (_arr->type()->Equals(NativeArrowTypes<float, _s>::data_type())) {
    return native_transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<
            typename NativeArrowTypes<float, _s>::ArrayType>(_arr));

  } else if (_arr->type()->Equals(NativeArrowTypes<double, _s>::data_type())) {
    return native_transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<
            typename NativeArrowTypes<double, _s>::ArrayType>(_arr));

  } else {
    return error("Expected numerical array, got " + _arr->type()->ToString() +
                 ".");
  }
}

}  // namespace rfl::parsing::tabular

#endif
