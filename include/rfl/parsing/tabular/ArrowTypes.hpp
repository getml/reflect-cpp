#ifndef RFL_PARSING_TABULAR_ARROWTYPES_HPP_
#define RFL_PARSING_TABULAR_ARROWTYPES_HPP_

#include <arrow/api.h>

#include <array>
#include <cinttypes>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "../../Box.hpp"
#include "../../NamedTuple.hpp"
#include "../../Ref.hpp"
#include "../../Rename.hpp"
#include "../../Timestamp.hpp"
#include "../../Tuple.hpp"
#include "../../concepts.hpp"
#include "../../enums.hpp"
#include "../../internal/StringLiteral.hpp"
#include "../../internal/has_reflection_type_v.hpp"
#include "../../internal/ptr_cast.hpp"
#include "../../named_tuple_t.hpp"

namespace rfl::parsing::tabular {

enum class SerializationType { csv, parquet };

template <class T, SerializationType _s>
struct ArrowTypes;

template <class T, SerializationType _s>
Result<Ref<typename ArrowTypes<T, _s>::ArrayType>> transform_numerical_array(
    const std::shared_ptr<arrow::Array>& _arr) noexcept;

template <SerializationType _s>
struct ArrowTypes<bool, _s> {
  using ArrayType = arrow::BooleanArray;
  using BuilderType = arrow::BooleanBuilder;

  static auto data_type() { return arrow::boolean(); }

  static void add_to_builder(const bool _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    if (_arr->type()->Equals(data_type())) {
      return Ref<ArrayType>::make(std::static_pointer_cast<ArrayType>(_arr));
    } else {
      return error("Expected boolean array, got " + _arr->type()->ToString() +
                   ".");
    }
  }

  static Result<bool> get_value(const Ref<ArrayType>& _chunk,
                                const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct ArrowTypes<uint8_t, _s> {
  using ArrayType = arrow::UInt8Array;
  using BuilderType = arrow::UInt8Builder;
  using T = uint8_t;

  static auto data_type() { return arrow::uint8(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return transform_numerical_array<T, _s>(_arr);
  }

  static Result<uint8_t> get_value(const Ref<ArrayType>& _chunk,
                                   const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct ArrowTypes<uint16_t, _s> {
  using ArrayType = arrow::UInt16Array;
  using BuilderType = arrow::UInt16Builder;
  using T = uint16_t;

  static auto data_type() { return arrow::uint16(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return transform_numerical_array<T, _s>(_arr);
  }

  static Result<uint16_t> get_value(const Ref<ArrayType>& _chunk,
                                    const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct ArrowTypes<uint32_t, _s> {
  using ArrayType = arrow::UInt32Array;
  using BuilderType = arrow::UInt32Builder;
  using T = uint32_t;

  static auto data_type() { return arrow::uint32(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return transform_numerical_array<T, _s>(_arr);
  }

  static Result<uint32_t> get_value(const Ref<ArrayType>& _chunk,
                                    const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct ArrowTypes<uint64_t, _s> {
  using ArrayType = arrow::UInt64Array;
  using BuilderType = arrow::UInt64Builder;
  using T = uint64_t;

  static auto data_type() { return arrow::uint64(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return transform_numerical_array<T, _s>(_arr);
  }

  static Result<uint64_t> get_value(const Ref<ArrayType>& _chunk,
                                    const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct ArrowTypes<int8_t, _s> {
  using ArrayType = arrow::Int8Array;
  using BuilderType = arrow::Int8Builder;
  using T = int8_t;

  static auto data_type() { return arrow::int8(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return transform_numerical_array<T, _s>(_arr);
  }

  static Result<int8_t> get_value(const Ref<ArrayType>& _chunk,
                                  const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct ArrowTypes<int16_t, _s> {
  using ArrayType = arrow::Int16Array;
  using BuilderType = arrow::Int16Builder;
  using T = int16_t;

  static auto data_type() { return arrow::int16(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return transform_numerical_array<T, _s>(_arr);
  }

  static Result<int16_t> get_value(const Ref<ArrayType>& _chunk,
                                   const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct ArrowTypes<int32_t, _s> {
  using ArrayType = arrow::Int32Array;
  using BuilderType = arrow::Int32Builder;
  using T = int32_t;

  static auto data_type() { return arrow::int32(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return transform_numerical_array<T, _s>(_arr);
  }

  static Result<int32_t> get_value(const Ref<ArrayType>& _chunk,
                                   const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct ArrowTypes<int64_t, _s> {
  using ArrayType = arrow::Int64Array;
  using BuilderType = arrow::Int64Builder;
  using T = int64_t;

  static auto data_type() { return arrow::int64(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return transform_numerical_array<T, _s>(_arr);
  }

  static Result<int64_t> get_value(const Ref<ArrayType>& _chunk,
                                   const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct ArrowTypes<float, _s> {
  using ArrayType = arrow::FloatArray;
  using BuilderType = arrow::FloatBuilder;
  using T = float;

  static auto data_type() { return arrow::float32(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return transform_numerical_array<T, _s>(_arr);
  }

  static Result<float> get_value(const Ref<ArrayType>& _chunk,
                                 const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct ArrowTypes<double, _s> {
  using ArrayType = arrow::DoubleArray;
  using BuilderType = arrow::DoubleBuilder;
  using T = double;

  static auto data_type() { return arrow::float64(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return transform_numerical_array<T, _s>(_arr);
  }

  static Result<double> get_value(const Ref<ArrayType>& _chunk,
                                  const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <SerializationType _s>
struct ArrowTypes<std::string, _s> {
  using ArrayType = arrow::StringArray;
  using BuilderType = arrow::StringBuilder;

  static auto data_type() { return arrow::utf8(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    if (_arr->type()->Equals(data_type())) {
      return Ref<ArrayType>::make(std::static_pointer_cast<ArrayType>(_arr));
    } else {
      return error("Expected string array, got " + _arr->type()->ToString() +
                   ".");
    }
  }

  static Result<std::string> get_value(const Ref<ArrayType>& _chunk,
                                       const int64_t _ix) {
    return std::string(_chunk->Value(_ix));
  }

  static auto make_builder() { return BuilderType(); }
};

template <class T, SerializationType _s>
  requires enchantum::Enum<T>
struct ArrowTypes<T, _s> {
  using ArrayType = arrow::StringArray;
  using BuilderType = arrow::StringBuilder;

  static auto data_type() { return arrow::utf8(); }

  static void add_to_builder(const T _val, BuilderType* _builder) {
    const auto status = _builder->Append(enum_to_string(_val));
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<std::string, _s>::get_array(_arr);
  }

  static Result<T> get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return string_to_enum<T>(std::string(_chunk->Value(_ix)));
  }

  static auto make_builder() { return BuilderType(); }
};

template <class T, SerializationType _s>
  requires concepts::ContiguousByteContainer<T>
struct ArrowTypes<T, _s> {
  using ArrayType = arrow::BinaryArray;
  using BuilderType = arrow::BinaryBuilder;

  static auto data_type() { return arrow::binary(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    const auto status = _builder->Append(
        internal::ptr_cast<const uint8_t*>(_val.data()), _val.size());
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

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

  static Result<T> get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    const auto begin = internal::ptr_cast<const typename T::value_type*>(
        _chunk->Value(_ix).data());
    return T(begin, begin + _chunk->Value(_ix).size());
  }

  static auto make_builder() { return BuilderType(); }

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
struct ArrowTypes<Timestamp<_format>, _s> {
  enum class TimeUnit { day, second, milli, micro, nano, string };

  using ArrayType = arrow::TimestampArray;
  using BuilderType = arrow::TimestampBuilder;

  static auto data_type() { return arrow::timestamp(arrow::TimeUnit::MILLI); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val.to_time_t() * 1000);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

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

  static Result<Timestamp<_format>> get_value(const Ref<ArrayType>& _chunk,
                                              const int64_t _ix) {
    return Timestamp<_format>(_chunk->Value(_ix) / 1000);
  }

  static auto make_builder() {
    return BuilderType(data_type(), arrow::default_memory_pool());
  }

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

template <internal::StringLiteral _format>
struct ArrowTypes<Timestamp<_format>, SerializationType::csv> {
  using ArrayType = arrow::TimestampArray;
  using BuilderType = arrow::StringBuilder;

  static auto data_type() { return arrow::timestamp(arrow::TimeUnit::MILLI); }

  static void add_to_builder(const Timestamp<_format>& _val,
                             BuilderType* _builder) {
    const auto status = _builder->Append(_val.str());
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<Timestamp<_format>,
                      SerializationType::parquet>::get_array(_arr);
  }

  static Result<Timestamp<_format>> get_value(const Ref<ArrayType>& _chunk,
                                              const int64_t _ix) {
    return ArrowTypes<Timestamp<_format>,
                      SerializationType::parquet>::get_value(_chunk, _ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <class T, SerializationType _s>
  requires internal::has_reflection_type_v<T>
struct ArrowTypes<T, _s> {
  using ArrayType =
      typename ArrowTypes<typename T::ReflectionType, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<typename T::ReflectionType, _s>::BuilderType;

  static auto data_type() {
    return ArrowTypes<typename T::ReflectionType, _s>::data_type();
  }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    ArrowTypes<typename T::ReflectionType, _s>::add_to_builder(
        _val.reflection(), _builder);
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<typename T::ReflectionType, _s>::get_array(_arr);
  }

  static Result<T> get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<typename T::ReflectionType>,
                      _s>::get_value(_chunk, _ix)
        .and_then([](const auto& _v) -> Result<T> {
          try {
            return T(_v);
          } catch (const std::exception& e) {
            return error(e.what());
          }
        });
  }

  static auto make_builder() {
    return ArrowTypes<typename T::ReflectionType, _s>::make_builder();
  }
};

template <class T, SerializationType _s>
struct ArrowTypes<std::optional<T>, _s> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<std::remove_cvref_t<T>, _s>::BuilderType;

  static auto data_type() { return ArrowTypes<T, _s>::data_type(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    if (_val) {
      ArrowTypes<T, _s>::add_to_builder(*_val, _builder);
    } else {
      const auto status = _builder->AppendNull();
      if (!status.ok()) {
        throw std::runtime_error(status.message());
      }
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<T, _s>::get_array(_arr);
  }

  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>, _s>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return std::make_optional<T>(_v); });
  }

  static auto make_builder() { return ArrowTypes<T, _s>::make_builder(); }
};

template <class T, SerializationType _s>
struct ArrowTypes<std::shared_ptr<T>, _s> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<std::remove_cvref_t<T>, _s>::BuilderType;

  static auto data_type() { return ArrowTypes<T, _s>::data_type(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    if (_val) {
      ArrowTypes<T, _s>::add_to_builder(*_val, _builder);
    } else {
      const auto status = _builder->AppendNull();
      if (!status.ok()) {
        throw std::runtime_error(status.message());
      }
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<T, _s>::get_array(_arr);
  }

  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>, _s>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return std::make_shared<T>(_v); });
  }

  static auto make_builder() { return ArrowTypes<T, _s>::make_builder(); }
};

template <class T, SerializationType _s>
struct ArrowTypes<std::unique_ptr<T>, _s> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<std::remove_cvref_t<T>, _s>::BuilderType;

  static auto data_type() { return ArrowTypes<T, _s>::data_type(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    if (_val) {
      ArrowTypes<T, _s>::add_to_builder(*_val, _builder);
    } else {
      const auto status = _builder->AppendNull();
      if (!status.ok()) {
        throw std::runtime_error(status.message());
      }
    }
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<T, _s>::get_array(_arr);
  }

  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>, _s>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return std::make_unique<T>(_v); });
  }

  static auto make_builder() { return ArrowTypes<T, _s>::make_builder(); }
};

template <class T, SerializationType _s>
struct ArrowTypes<Box<T>, _s> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<std::remove_cvref_t<T>, _s>::BuilderType;

  static auto data_type() { return ArrowTypes<T, _s>::data_type(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    ArrowTypes<T, _s>::add_to_builder(*_val, _builder);
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<T, _s>::get_array(_arr);
  }

  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>, _s>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return Box<T>::make(_v); });
  }

  static auto make_builder() { return ArrowTypes<T, _s>::make_builder(); }
};

template <class T, SerializationType _s>
struct ArrowTypes<Ref<T>, _s> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<std::remove_cvref_t<T>, _s>::BuilderType;

  static auto data_type() { return ArrowTypes<T, _s>::data_type(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    ArrowTypes<T, _s>::add_to_builder(*_val, _builder);
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<T, _s>::get_array(_arr);
  }

  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>, _s>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return Ref<T>::make(_v); });
  }

  static auto make_builder() { return ArrowTypes<T, _s>::make_builder(); }
};

template <internal::StringLiteral _name, class T, SerializationType _s>
struct ArrowTypes<Rename<_name, T>, _s> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<std::remove_cvref_t<T>, _s>::BuilderType;

  static auto data_type() { return ArrowTypes<T, _s>::data_type(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    ArrowTypes<T, _s>::add_to_builder(_val.value(), _builder);
  }

  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<T, _s>::get_array(_arr);
  }

  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>, _s>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return Rename<_name, T>(_v); });
  }

  static auto make_builder() { return ArrowTypes<T, _s>::make_builder(); }
};

template <class T, SerializationType _s, class SourceArrayType>
Result<Ref<typename ArrowTypes<T, _s>::ArrayType>>
transform_numerical_array_impl(
    const std::shared_ptr<SourceArrayType>& _arr) noexcept {
  if (!_arr) {
    return error(
        "transform_numerical_array_impl: std::shared_ptr not set. This is a "
        "bug, please report.");
  }

  auto builder = ArrowTypes<T, _s>::make_builder();

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

  using TargetArrayType = typename ArrowTypes<T, _s>::ArrayType;

  std::shared_ptr<arrow::Array> res;
  const auto status = builder.Finish(&res);
  return Ref<TargetArrayType>::make(
      std::static_pointer_cast<TargetArrayType>(res));
}

template <class T, SerializationType _s>
Result<Ref<typename ArrowTypes<T, _s>::ArrayType>> transform_numerical_array(
    const std::shared_ptr<arrow::Array>& _arr) noexcept {
  if (!_arr) {
    return error(
        "Could not transform the numerical array. std::shared_ptr not set.");
  }

  using ArrayType = typename ArrowTypes<T, _s>::ArrayType;

  if (_arr->type()->Equals(ArrowTypes<T, _s>::data_type())) {
    return Ref<ArrayType>::make(std::static_pointer_cast<ArrayType>(_arr));

  } else if (_arr->type()->Equals(ArrowTypes<uint8_t, _s>::data_type())) {
    return transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<typename ArrowTypes<uint8_t, _s>::ArrayType>(
            _arr));

  } else if (_arr->type()->Equals(ArrowTypes<uint16_t, _s>::data_type())) {
    return transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<typename ArrowTypes<uint16_t, _s>::ArrayType>(
            _arr));

  } else if (_arr->type()->Equals(ArrowTypes<uint32_t, _s>::data_type())) {
    return transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<typename ArrowTypes<uint32_t, _s>::ArrayType>(
            _arr));

  } else if (_arr->type()->Equals(ArrowTypes<uint64_t, _s>::data_type())) {
    return transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<typename ArrowTypes<uint64_t, _s>::ArrayType>(
            _arr));

  } else if (_arr->type()->Equals(ArrowTypes<int8_t, _s>::data_type())) {
    return transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<typename ArrowTypes<int8_t, _s>::ArrayType>(
            _arr));

  } else if (_arr->type()->Equals(ArrowTypes<int16_t, _s>::data_type())) {
    return transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<typename ArrowTypes<int16_t, _s>::ArrayType>(
            _arr));

  } else if (_arr->type()->Equals(ArrowTypes<int32_t, _s>::data_type())) {
    return transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<typename ArrowTypes<int32_t, _s>::ArrayType>(
            _arr));

  } else if (_arr->type()->Equals(ArrowTypes<int64_t, _s>::data_type())) {
    return transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<typename ArrowTypes<int64_t, _s>::ArrayType>(
            _arr));

  } else if (_arr->type()->Equals(ArrowTypes<float, _s>::data_type())) {
    return transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<typename ArrowTypes<float, _s>::ArrayType>(
            _arr));

  } else if (_arr->type()->Equals(ArrowTypes<double, _s>::data_type())) {
    return transform_numerical_array_impl<T, _s>(
        std::static_pointer_cast<typename ArrowTypes<double, _s>::ArrayType>(
            _arr));

  } else {
    return error("Expected numerical array, got " + _arr->type()->ToString() +
                 ".");
  }
}

}  // namespace rfl::parsing::tabular

#endif
