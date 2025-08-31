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

template <class T>
struct ArrowTypes;

template <>
struct ArrowTypes<bool> {
  using ArrayType = arrow::BooleanArray;
  using BuilderType = arrow::BooleanBuilder;

  static auto data_type() { return arrow::boolean(); }

  static void add_to_builder(const bool _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<bool> get_value(const Ref<ArrayType>& _chunk,
                                const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <>
struct ArrowTypes<uint8_t> {
  using ArrayType = arrow::UInt8Array;
  using BuilderType = arrow::UInt8Builder;

  static auto data_type() { return arrow::uint8(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<uint8_t> get_value(const Ref<ArrayType>& _chunk,
                                   const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <>
struct ArrowTypes<uint16_t> {
  using ArrayType = arrow::UInt16Array;
  using BuilderType = arrow::UInt16Builder;

  static auto data_type() { return arrow::uint16(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<uint16_t> get_value(const Ref<ArrayType>& _chunk,
                                    const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <>
struct ArrowTypes<uint32_t> {
  using ArrayType = arrow::UInt32Array;
  using BuilderType = arrow::UInt32Builder;

  static auto data_type() { return arrow::uint32(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<uint32_t> get_value(const Ref<ArrayType>& _chunk,
                                    const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <>
struct ArrowTypes<uint64_t> {
  using ArrayType = arrow::UInt64Array;
  using BuilderType = arrow::UInt64Builder;

  static auto data_type() { return arrow::uint64(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<uint64_t> get_value(const Ref<ArrayType>& _chunk,
                                    const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <>
struct ArrowTypes<int8_t> {
  using ArrayType = arrow::Int8Array;
  using BuilderType = arrow::Int8Builder;

  static auto data_type() { return arrow::int8(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<int8_t> get_value(const Ref<ArrayType>& _chunk,
                                  const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <>
struct ArrowTypes<int16_t> {
  using ArrayType = arrow::Int16Array;
  using BuilderType = arrow::Int16Builder;

  static auto data_type() { return arrow::int16(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<int16_t> get_value(const Ref<ArrayType>& _chunk,
                                   const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <>
struct ArrowTypes<int32_t> {
  using ArrayType = arrow::Int32Array;
  using BuilderType = arrow::Int32Builder;

  static auto data_type() { return arrow::int32(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<int32_t> get_value(const Ref<ArrayType>& _chunk,
                                   const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <>
struct ArrowTypes<int64_t> {
  using ArrayType = arrow::Int64Array;
  using BuilderType = arrow::Int64Builder;

  static auto data_type() { return arrow::int64(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<int64_t> get_value(const Ref<ArrayType>& _chunk,
                                   const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <>
struct ArrowTypes<float> {
  using ArrayType = arrow::FloatArray;
  using BuilderType = arrow::FloatBuilder;

  static auto data_type() { return arrow::float32(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<float> get_value(const Ref<ArrayType>& _chunk,
                                 const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <>
struct ArrowTypes<double> {
  using ArrayType = arrow::DoubleArray;
  using BuilderType = arrow::DoubleBuilder;

  static auto data_type() { return arrow::float64(); }

  static void add_to_builder(const auto _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<double> get_value(const Ref<ArrayType>& _chunk,
                                  const int64_t _ix) {
    return _chunk->Value(_ix);
  }

  static auto make_builder() { return BuilderType(); }
};

template <>
struct ArrowTypes<std::string> {
  using ArrayType = arrow::StringArray;
  using BuilderType = arrow::StringBuilder;

  static auto data_type() { return arrow::utf8(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<std::string> get_value(const Ref<ArrayType>& _chunk,
                                       const int64_t _ix) {
    return std::string(_chunk->Value(_ix));
  }

  static auto make_builder() { return BuilderType(); }
};

template <class T>
  requires enchantum::Enum<T>
struct ArrowTypes<T> {
  using ArrayType = arrow::StringArray;
  using BuilderType = arrow::StringBuilder;

  static auto data_type() { return arrow::utf8(); }

  static void add_to_builder(const T _val, BuilderType* _builder) {
    const auto status = _builder->Append(enum_to_string(_val));
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<T> get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return string_to_enum<T>(std::string(_chunk->Value(_ix)));
  }

  static auto make_builder() { return BuilderType(); }
};

template <class T>
  requires concepts::ContiguousByteContainer<T>
struct ArrowTypes<T> {
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

  static Result<T> get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    const auto begin = internal::ptr_cast<const typename T::value_type*>(
        _chunk->Value(_ix).data());
    return T(begin, begin + _chunk->Value(_ix).size());
  }

  static auto make_builder() { return BuilderType(); }
};

template <internal::StringLiteral _format>
struct ArrowTypes<Timestamp<_format>> {
  using ArrayType = arrow::TimestampArray;
  using BuilderType = arrow::TimestampBuilder;

  static auto data_type() { return arrow::timestamp(arrow::TimeUnit::SECOND); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    const auto status = _builder->Append(_val.to_time_t());
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }

  static Result<Timestamp<_format>> get_value(const Ref<ArrayType>& _chunk,
                                              const int64_t _ix) {
    return Timestamp<_format>(_chunk->Value(_ix) / 1000);
  }

  static auto make_builder() {
    return BuilderType(data_type(), arrow::default_memory_pool());
  }
};

template <class T>
  requires internal::has_reflection_type_v<T>
struct ArrowTypes<T> {
  using ArrayType = typename ArrowTypes<typename T::ReflectionType>::ArrayType;
  using BuilderType =
      typename ArrowTypes<typename T::ReflectionType>::BuilderType;

  static auto data_type() {
    return ArrowTypes<typename T::ReflectionType>::data_type();
  }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    ArrowTypes<typename T::ReflectionType>::add_to_builder(_val.reflection(),
                                                           _builder);
  }

  static Result<T> get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<typename T::ReflectionType>>::
        get_value(_chunk, _ix)
            .and_then([](const auto& _v) -> Result<T> {
              try {
                return T(_v);
              } catch (const std::exception& e) {
                return error(e.what());
              }
            });
  }

  static auto make_builder() {
    return ArrowTypes<typename T::ReflectionType>::make_builder();
  }
};

template <class T>
struct ArrowTypes<std::optional<T>> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>>::ArrayType;
  using BuilderType = typename ArrowTypes<std::remove_cvref_t<T>>::BuilderType;

  static auto data_type() { return ArrowTypes<T>::data_type(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    if (_val) {
      ArrowTypes<T>::add_to_builder(*_val, _builder);
    } else {
      const auto status = _builder->AppendNull();
      if (!status.ok()) {
        throw std::runtime_error(status.message());
      }
    }
  }

  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return std::make_optional<T>(_v); });
  }

  static auto make_builder() { return ArrowTypes<T>::make_builder(); }
};

template <class T>
struct ArrowTypes<std::shared_ptr<T>> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>>::ArrayType;
  using BuilderType = typename ArrowTypes<std::remove_cvref_t<T>>::BuilderType;

  static auto data_type() { return ArrowTypes<T>::data_type(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    if (_val) {
      ArrowTypes<T>::add_to_builder(*_val, _builder);
    } else {
      const auto status = _builder->AppendNull();
      if (!status.ok()) {
        throw std::runtime_error(status.message());
      }
    }
  }

  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return std::make_shared<T>(_v); });
  }

  static auto make_builder() { return ArrowTypes<T>::make_builder(); }
};

template <class T>
struct ArrowTypes<std::unique_ptr<T>> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>>::ArrayType;
  using BuilderType = typename ArrowTypes<std::remove_cvref_t<T>>::BuilderType;

  static auto data_type() { return ArrowTypes<T>::data_type(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    if (_val) {
      ArrowTypes<T>::add_to_builder(*_val, _builder);
    } else {
      const auto status = _builder->AppendNull();
      if (!status.ok()) {
        throw std::runtime_error(status.message());
      }
    }
  }

  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return std::make_unique<T>(_v); });
  }

  static auto make_builder() { return ArrowTypes<T>::make_builder(); }
};

template <class T>
struct ArrowTypes<Box<T>> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>>::ArrayType;
  using BuilderType = typename ArrowTypes<std::remove_cvref_t<T>>::BuilderType;

  static auto data_type() { return ArrowTypes<T>::data_type(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    ArrowTypes<T>::add_to_builder(*_val, _builder);
  }

  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return Box<T>::make(_v); });
  }

  static auto make_builder() { return ArrowTypes<T>::make_builder(); }
};

template <class T>
struct ArrowTypes<Ref<T>> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>>::ArrayType;
  using BuilderType = typename ArrowTypes<std::remove_cvref_t<T>>::BuilderType;

  static auto data_type() { return ArrowTypes<T>::data_type(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    ArrowTypes<T>::add_to_builder(*_val, _builder);
  }

  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return Ref<T>::make(_v); });
  }

  static auto make_builder() { return ArrowTypes<T>::make_builder(); }
};

template <internal::StringLiteral _name, class T>
struct ArrowTypes<Rename<_name, T>> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>>::ArrayType;
  using BuilderType = typename ArrowTypes<std::remove_cvref_t<T>>::BuilderType;

  static auto data_type() { return ArrowTypes<T>::data_type(); }

  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    ArrowTypes<T>::add_to_builder(_val.value(), _builder);
  }

  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return Rename<_name, T>(_v); });
  }

  static auto make_builder() { return ArrowTypes<T>::make_builder(); }
};

}  // namespace rfl::parsing::tabular

#endif
