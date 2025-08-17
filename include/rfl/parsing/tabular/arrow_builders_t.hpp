#ifndef RFL_PARSING_TABULAR_ARROWBUILDERST_HPP_
#define RFL_PARSING_TABULAR_ARROWBUILDERST_HPP_

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
#include "../../internal/StringLiteral.hpp"
#include "../../internal/has_reflection_type_v.hpp"
#include "../../named_tuple_t.hpp"

namespace rfl::parsing::tabular {

template <class T>
struct ArrowBuilderType;

template <>
struct ArrowBuilderType<uint8_t> {
  using Type = arrow::UInt8Builder;

  static auto data_type() { return arrow::uint8(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }
};

template <>
struct ArrowBuilderType<uint16_t> {
  using Type = arrow::UInt16Builder;

  static auto data_type() { return arrow::uint16(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }
};

template <>
struct ArrowBuilderType<uint32_t> {
  using Type = arrow::UInt32Builder;

  static auto data_type() { return arrow::uint32(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }
};

template <>
struct ArrowBuilderType<uint64_t> {
  using Type = arrow::UInt64Builder;

  static auto data_type() { return arrow::uint64(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }
};

template <>
struct ArrowBuilderType<int8_t> {
  using Type = arrow::Int8Builder;

  static auto data_type() { return arrow::int8(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }
};

template <>
struct ArrowBuilderType<int16_t> {
  using Type = arrow::Int16Builder;

  static auto data_type() { return arrow::int16(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }
};

template <>
struct ArrowBuilderType<int32_t> {
  using Type = arrow::Int32Builder;

  static auto data_type() { return arrow::int32(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }
};

template <>
struct ArrowBuilderType<int64_t> {
  using Type = arrow::Int64Builder;

  static auto data_type() { return arrow::int64(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }
};

template <>
struct ArrowBuilderType<float> {
  using Type = arrow::FloatBuilder;

  static auto data_type() { return arrow::float32(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }
};

template <>
struct ArrowBuilderType<double> {
  using Type = arrow::DoubleBuilder;

  static auto data_type() { return arrow::float64(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }
};

template <>
struct ArrowBuilderType<std::string> {
  using Type = arrow::StringBuilder;

  static auto data_type() { return arrow::utf8(); }

  static void add_to_builder(const auto& _val, Type* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }
};

template <internal::StringLiteral _format>
struct ArrowBuilderType<Timestamp<_format>> {
  using Type = arrow::TimestampBuilder;

  static auto data_type() { return arrow::timestamp(arrow::TimeUnit::SECOND); }

  static void add_to_builder(const auto _val, Type* _builder) {
    const auto status = _builder->Append(_val);
    if (!status.ok()) {
      throw std::runtime_error(status.message());
    }
  }
};

template <class T>
  requires internal::has_reflection_type_v<T>
struct ArrowBuilderType<T> {
  using Type = typename ArrowBuilderType<typename T::ReflectionType>::Type;

  static auto data_type() {
    return ArrowBuilderType<typename T::ReflectionType>::data_type();
  }

  static void add_to_builder(const auto _val, Type* _builder) {
    ArrowBuilderType<typename T::ReflectionType>::add_to_builder(
        _val.reflection(), _builder);
  }
};

template <class T>
struct ArrowBuilderType<std::optional<T>> {
  using Type = typename ArrowBuilderType<std::remove_cvref_t<T>>::Type;

  static auto data_type() { return ArrowBuilderType<T>::data_type(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    if (_val) {
      ArrowBuilderType<T>::add_to_builder(*_val, _builder);
    } else {
      const auto status = _builder->AppendNull();
      if (!status.ok()) {
        throw std::runtime_error(status.message());
      }
    }
  }
};

template <class T>
struct ArrowBuilderType<std::shared_ptr<T>> {
  using Type = typename ArrowBuilderType<std::remove_cvref_t<T>>::Type;

  static auto data_type() { return ArrowBuilderType<T>::data_type(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    if (_val) {
      ArrowBuilderType<T>::add_to_builder(*_val, _builder);
    } else {
      const auto status = _builder->AppendNull();
      if (!status.ok()) {
        throw std::runtime_error(status.message());
      }
    }
  }
};

template <class T>
struct ArrowBuilderType<std::unique_ptr<T>> {
  using Type = typename ArrowBuilderType<std::remove_cvref_t<T>>::Type;

  static auto data_type() { return ArrowBuilderType<T>::data_type(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    if (_val) {
      ArrowBuilderType<T>::add_to_builder(*_val, _builder);
    } else {
      const auto status = _builder->AppendNull();
      if (!status.ok()) {
        throw std::runtime_error(status.message());
      }
    }
  }
};

template <class T>
struct ArrowBuilderType<Box<T>> {
  using Type = typename ArrowBuilderType<std::remove_cvref_t<T>>::Type;

  static auto data_type() { return ArrowBuilderType<T>::data_type(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    ArrowBuilderType<T>::add_to_builder(*_val, _builder);
  }
};

template <class T>
struct ArrowBuilderType<Ref<T>> {
  using Type = typename ArrowBuilderType<std::remove_cvref_t<T>>::Type;

  static auto data_type() { return ArrowBuilderType<T>::data_type(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    ArrowBuilderType<T>::add_to_builder(*_val, _builder);
  }
};

template <internal::StringLiteral _name, class T>
struct ArrowBuilderType<Rename<_name, T>> {
  using Type = typename ArrowBuilderType<std::remove_cvref_t<T>>::Type;

  static auto data_type() { return ArrowBuilderType<T>::data_type(); }

  static void add_to_builder(const auto _val, Type* _builder) {
    ArrowBuilderType<T>::add_to_builder(_val.value(), _builder);
  }
};

template <class T>
using arrow_builder_t = typename ArrowBuilderType<
    std::remove_cvref_t<std::remove_pointer_t<T>>>::Type;

template <class T>
struct ArrowBuildersType;

template <class... FieldTypes>
struct ArrowBuildersType<NamedTuple<FieldTypes...>> {
  using Type = Tuple<arrow_builder_t<typename FieldTypes::Type>...>;

  static auto data_types() {
    return [&]<size_t... _is>(std::integer_sequence<size_t, _is...>) {
      return std::array<std::shared_ptr<arrow::DataType>,
                        sizeof...(FieldTypes)>(
          {ArrowBuilderType<typename FieldTypes::Type>::data_type()...});
    }(std::make_integer_sequence<size_t, sizeof...(FieldTypes)>());
  }

  static auto schema() {
    const auto fields =
        std::vector<std::shared_ptr<arrow::Field>>({arrow::field(
            typename FieldTypes::Name().str(),
            ArrowBuilderType<typename FieldTypes::Type>::data_type())...});
    return arrow::schema(fields);
  }
};

template <class T>
using arrow_builders_t =
    typename ArrowBuildersType<named_tuple_t<std::remove_cvref_t<T>>>::Type;

}  // namespace rfl::parsing::tabular

#endif
