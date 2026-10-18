#ifndef RFL_PARSING_TABULAR_ARROWTYPES_HPP_
#define RFL_PARSING_TABULAR_ARROWTYPES_HPP_

#include <memory>
#include <optional>
#include <string>

#include "../../Box.hpp"
#include "../../NamedTuple.hpp"
#include "../../Ref.hpp"
#include "../../Rename.hpp"
#include "../../Timestamp.hpp"
#include "../../Tuple.hpp"
#include "../../concepts.hpp"
#include "../../enums.hpp"
#include "../../internal/has_reflection_type_v.hpp"
#include "../../internal/ptr_cast.hpp"
#include "ArrowBridge.hpp"

namespace rfl::parsing::tabular {

template <class T, SerializationType _s>
struct ArrowTypes;

template <class T>
struct ArrowScalar;
template <>
struct RFL_API ArrowScalar<bool> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static std::shared_ptr<arrow::DataType> data_type();
  static void add_to_builder(const bool& _value, BuilderType* _builder);
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr);
  static Result<bool> get_value(const Ref<ArrayType>& _arr, int64_t _ix);
  static BuilderType make_builder();
};
template <SerializationType _s>
struct ArrowTypes<bool, _s> : ArrowScalar<bool> {};
template <>
struct RFL_API ArrowScalar<uint8_t> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static std::shared_ptr<arrow::DataType> data_type();
  static void add_to_builder(const uint8_t& _value, BuilderType* _builder);
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr);
  static Result<uint8_t> get_value(const Ref<ArrayType>& _arr, int64_t _ix);
  static BuilderType make_builder();
};
template <SerializationType _s>
struct ArrowTypes<uint8_t, _s> : ArrowScalar<uint8_t> {};
template <>
struct RFL_API ArrowScalar<uint16_t> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static std::shared_ptr<arrow::DataType> data_type();
  static void add_to_builder(const uint16_t& _value, BuilderType* _builder);
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr);
  static Result<uint16_t> get_value(const Ref<ArrayType>& _arr, int64_t _ix);
  static BuilderType make_builder();
};
template <SerializationType _s>
struct ArrowTypes<uint16_t, _s> : ArrowScalar<uint16_t> {};
template <>
struct RFL_API ArrowScalar<uint32_t> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static std::shared_ptr<arrow::DataType> data_type();
  static void add_to_builder(const uint32_t& _value, BuilderType* _builder);
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr);
  static Result<uint32_t> get_value(const Ref<ArrayType>& _arr, int64_t _ix);
  static BuilderType make_builder();
};
template <SerializationType _s>
struct ArrowTypes<uint32_t, _s> : ArrowScalar<uint32_t> {};
template <>
struct RFL_API ArrowScalar<uint64_t> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static std::shared_ptr<arrow::DataType> data_type();
  static void add_to_builder(const uint64_t& _value, BuilderType* _builder);
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr);
  static Result<uint64_t> get_value(const Ref<ArrayType>& _arr, int64_t _ix);
  static BuilderType make_builder();
};
template <SerializationType _s>
struct ArrowTypes<uint64_t, _s> : ArrowScalar<uint64_t> {};
template <>
struct RFL_API ArrowScalar<int8_t> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static std::shared_ptr<arrow::DataType> data_type();
  static void add_to_builder(const int8_t& _value, BuilderType* _builder);
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr);
  static Result<int8_t> get_value(const Ref<ArrayType>& _arr, int64_t _ix);
  static BuilderType make_builder();
};
template <SerializationType _s>
struct ArrowTypes<int8_t, _s> : ArrowScalar<int8_t> {};
template <>
struct RFL_API ArrowScalar<int16_t> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static std::shared_ptr<arrow::DataType> data_type();
  static void add_to_builder(const int16_t& _value, BuilderType* _builder);
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr);
  static Result<int16_t> get_value(const Ref<ArrayType>& _arr, int64_t _ix);
  static BuilderType make_builder();
};
template <SerializationType _s>
struct ArrowTypes<int16_t, _s> : ArrowScalar<int16_t> {};
template <>
struct RFL_API ArrowScalar<int32_t> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static std::shared_ptr<arrow::DataType> data_type();
  static void add_to_builder(const int32_t& _value, BuilderType* _builder);
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr);
  static Result<int32_t> get_value(const Ref<ArrayType>& _arr, int64_t _ix);
  static BuilderType make_builder();
};
template <SerializationType _s>
struct ArrowTypes<int32_t, _s> : ArrowScalar<int32_t> {};
template <>
struct RFL_API ArrowScalar<int64_t> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static std::shared_ptr<arrow::DataType> data_type();
  static void add_to_builder(const int64_t& _value, BuilderType* _builder);
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr);
  static Result<int64_t> get_value(const Ref<ArrayType>& _arr, int64_t _ix);
  static BuilderType make_builder();
};
template <SerializationType _s>
struct ArrowTypes<int64_t, _s> : ArrowScalar<int64_t> {};
template <>
struct RFL_API ArrowScalar<float> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static std::shared_ptr<arrow::DataType> data_type();
  static void add_to_builder(const float& _value, BuilderType* _builder);
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr);
  static Result<float> get_value(const Ref<ArrayType>& _arr, int64_t _ix);
  static BuilderType make_builder();
};
template <SerializationType _s>
struct ArrowTypes<float, _s> : ArrowScalar<float> {};
template <>
struct RFL_API ArrowScalar<double> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static std::shared_ptr<arrow::DataType> data_type();
  static void add_to_builder(const double& _value, BuilderType* _builder);
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr);
  static Result<double> get_value(const Ref<ArrayType>& _arr, int64_t _ix);
  static BuilderType make_builder();
};
template <SerializationType _s>
struct ArrowTypes<double, _s> : ArrowScalar<double> {};
template <>
struct RFL_API ArrowScalar<std::string> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static std::shared_ptr<arrow::DataType> data_type();
  static void add_to_builder(const std::string& _value, BuilderType* _builder);
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr);
  static Result<std::string> get_value(const Ref<ArrayType>& _arr, int64_t _ix);
  static BuilderType make_builder();
};
template <SerializationType _s>
struct ArrowTypes<std::string, _s> : ArrowScalar<std::string> {};

template <class T, SerializationType _s>
  requires enchantum::Enum<T>
struct ArrowTypes<T, _s> : ArrowScalar<std::string> {
  static void add_to_builder(T _value, BuilderType* _builder) {
    ArrowScalar<std::string>::add_to_builder(enum_to_string(_value), _builder);
  }
  static Result<T> get_value(const Ref<ArrayType>& _arr, int64_t _ix) {
    return ArrowScalar<std::string>::get_value(_arr, _ix).and_then(
        [](const auto& _value) { return string_to_enum<T>(_value); });
  }
};

template <class T, SerializationType _s>
  requires concepts::ContiguousByteContainer<T>
struct ArrowTypes<T, _s> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static auto data_type() { return binary_data_type(); }
  static auto make_builder() { return make_binary_builder(); }
  static void add_to_builder(const T& _value, BuilderType* _builder) {
    append_binary(_builder, internal::ptr_cast<const uint8_t*>(_value.data()),
                  _value.size());
  }
  static auto get_array(const std::shared_ptr<arrow::Array>& _arr) {
    return binary_array(_arr);
  }
  static Result<T> get_value(const Ref<ArrayType>& _arr, int64_t _ix) {
    const auto value = binary_value(_arr, _ix);
    const auto begin =
        internal::ptr_cast<const typename T::value_type*>(value.data());
    return T(begin, begin + value.size());
  }
};

template <internal::StringLiteral _format, SerializationType _s>
struct ArrowTypes<Timestamp<_format>, _s> {
  using ArrayType = arrow::Array;
  using BuilderType = ArrowBuilder;
  static auto data_type() { return timestamp_data_type(); }
  static auto make_builder() {
    if constexpr (_s == SerializationType::csv)
      return ArrowScalar<std::string>::make_builder();
    else
      return make_timestamp_builder();
  }
  static void add_to_builder(const Timestamp<_format>& _value,
                             BuilderType* _builder) {
    if constexpr (_s == SerializationType::csv)
      ArrowScalar<std::string>::add_to_builder(_value.str(), _builder);
    else
      append_timestamp(_builder, _value.to_time_t() * 1000);
  }
  static auto get_array(const std::shared_ptr<arrow::Array>& _arr) {
    return timestamp_array(
        _arr, [](std::string_view _value) -> Result<int64_t> {
          return Timestamp<_format>::make(std::string(_value))
              .transform([](const auto& _ts) -> int64_t {
                return _ts.to_time_t() * 1000;
              });
        });
  }
  static Result<Timestamp<_format>> get_value(const Ref<ArrayType>& _arr,
                                              int64_t _ix) {
    return Timestamp<_format>(timestamp_value(_arr, _ix) / 1000);
  }
};

template <class T, SerializationType _s>
  requires internal::has_reflection_type_v<T>
struct ArrowTypes<T, _s> {
  using ArrayType =
      typename ArrowTypes<typename T::ReflectionType, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<typename T::ReflectionType, _s>::BuilderType;

  /**
   * @brief Returns the Arrow data type for T based on its ReflectionType.
   * @return The Arrow data type.
   */
  static auto data_type() {
    return ArrowTypes<typename T::ReflectionType, _s>::data_type();
  }

  /**
   * @brief Adds a value of type T to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    ArrowTypes<typename T::ReflectionType, _s>::add_to_builder(
        _val.reflection(), _builder);
  }

  /**
   * @brief Returns the array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<typename T::ReflectionType, _s>::get_array(_arr);
  }

  /**
   * @brief Returns the value of type T from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The value of type T.
   */
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

  /**
   * @brief Creates a builder for arrays of type T.
   * @return The builder.
   */
  static auto make_builder() {
    return ArrowTypes<typename T::ReflectionType, _s>::make_builder();
  }
};

template <class T, SerializationType _s>
struct ArrowTypes<std::optional<T>, _s> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<std::remove_cvref_t<T>, _s>::BuilderType;

  /**
   * @brief Returns the Arrow data type for optional T.
   * @return The Arrow data type.
   */
  static auto data_type() { return ArrowTypes<T, _s>::data_type(); }

  /**
   * @brief Adds an optional value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    if (_val) {
      ArrowTypes<T, _s>::add_to_builder(*_val, _builder);
    } else {
      _builder->append_null();
    }
  }

  /**
   * @brief Returns the array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<T, _s>::get_array(_arr);
  }

  /**
   * @brief Returns the optional value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The optional value.
   */
  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>, _s>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return std::make_optional<T>(_v); });
  }

  /**
   * @brief Creates a builder for arrays of optional T.
   * @return The builder.
   */
  static auto make_builder() { return ArrowTypes<T, _s>::make_builder(); }
};

template <class T, SerializationType _s>
struct ArrowTypes<std::shared_ptr<T>, _s> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<std::remove_cvref_t<T>, _s>::BuilderType;

  /**
   * @brief Returns the Arrow data type for shared_ptr T.
   * @return The Arrow data type.
   */
  static auto data_type() { return ArrowTypes<T, _s>::data_type(); }

  /**
   * @brief Adds a shared_ptr value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    if (_val) {
      ArrowTypes<T, _s>::add_to_builder(*_val, _builder);
    } else {
      _builder->append_null();
    }
  }

  /**
   * @brief Returns the array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<T, _s>::get_array(_arr);
  }

  /**
   * @brief Returns the shared_ptr value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The shared_ptr value.
   */
  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>, _s>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return std::make_shared<T>(_v); });
  }

  /**
   * @brief Creates a builder for arrays of shared_ptr T.
   * @return The builder.
   */
  static auto make_builder() { return ArrowTypes<T, _s>::make_builder(); }
};

template <class T, SerializationType _s>
struct ArrowTypes<std::unique_ptr<T>, _s> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<std::remove_cvref_t<T>, _s>::BuilderType;

  /**
   * @brief Returns the Arrow data type for unique_ptr T.
   * @return The Arrow data type.
   */
  static auto data_type() { return ArrowTypes<T, _s>::data_type(); }

  /**
   * @brief Adds a unique_ptr value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    if (_val) {
      ArrowTypes<T, _s>::add_to_builder(*_val, _builder);
    } else {
      _builder->append_null();
    }
  }

  /**
   * @brief Returns the array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<T, _s>::get_array(_arr);
  }

  /**
   * @brief Returns the unique_ptr value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The unique_ptr value.
   */
  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>, _s>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return std::make_unique<T>(_v); });
  }

  /**
   * @brief Creates a builder for arrays of unique_ptr T.
   * @return The builder.
   */
  static auto make_builder() { return ArrowTypes<T, _s>::make_builder(); }
};

template <class T, SerializationType _s>
struct ArrowTypes<Box<T>, _s> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<std::remove_cvref_t<T>, _s>::BuilderType;

  /**
   * @brief Returns the Arrow data type for Box T.
   * @return The Arrow data type.
   */
  static auto data_type() { return ArrowTypes<T, _s>::data_type(); }

  /**
   * @brief Adds a Box value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    ArrowTypes<T, _s>::add_to_builder(*_val, _builder);
  }

  /**
   * @brief Returns the array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<T, _s>::get_array(_arr);
  }

  /**
   * @brief Returns the Box value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The Box value.
   */
  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>, _s>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return Box<T>::make(_v); });
  }

  /**
   * @brief Creates a builder for arrays of Box T.
   * @return The builder.
   */
  static auto make_builder() { return ArrowTypes<T, _s>::make_builder(); }
};

template <class T, SerializationType _s>
struct ArrowTypes<Ref<T>, _s> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<std::remove_cvref_t<T>, _s>::BuilderType;

  /**
   * @brief Returns the Arrow data type for Ref T.
   * @return The Arrow data type.
   */
  static auto data_type() { return ArrowTypes<T, _s>::data_type(); }

  /**
   * @brief Adds a Ref value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    ArrowTypes<T, _s>::add_to_builder(*_val, _builder);
  }

  /**
   * @brief Returns the array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<T, _s>::get_array(_arr);
  }

  /**
   * @brief Returns the Ref value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The Ref value.
   */
  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>, _s>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return Ref<T>::make(_v); });
  }

  /**
   * @brief Creates a builder for arrays of Ref T.
   * @return The builder.
   */
  static auto make_builder() { return ArrowTypes<T, _s>::make_builder(); }
};

template <internal::StringLiteral _name, class T, SerializationType _s>
struct ArrowTypes<Rename<_name, T>, _s> {
  using ArrayType = typename ArrowTypes<std::remove_cvref_t<T>, _s>::ArrayType;
  using BuilderType =
      typename ArrowTypes<std::remove_cvref_t<T>, _s>::BuilderType;

  /**
   * @brief Returns the Arrow data type for Rename T.
   * @return The Arrow data type.
   */
  static auto data_type() { return ArrowTypes<T, _s>::data_type(); }

  /**
   * @brief Adds a Rename value to the builder.
   * @param _val The value to add.
   * @param _builder The builder to add to.
   */
  static void add_to_builder(const auto& _val, BuilderType* _builder) {
    ArrowTypes<T, _s>::add_to_builder(_val.value(), _builder);
  }

  /**
   * @brief Returns the array from the shared pointer.
   * @param _arr The shared pointer to the arrow array.
   * @return The array or an error.
   */
  static Result<Ref<ArrayType>> get_array(
      const std::shared_ptr<arrow::Array>& _arr) {
    return ArrowTypes<T, _s>::get_array(_arr);
  }

  /**
   * @brief Returns the Rename value from the array at the given index.
   * @param _chunk The array to get the value from.
   * @param _ix The index of the value.
   * @return The Rename value.
   */
  static auto get_value(const Ref<ArrayType>& _chunk, const int64_t _ix) {
    return ArrowTypes<std::remove_cvref_t<T>, _s>::get_value(_chunk, _ix)
        .transform([](const auto& _v) { return Rename<_name, T>(_v); });
  }

  /**
   * @brief Creates a builder for arrays of Rename T.
   * @return The builder.
   */
  static auto make_builder() { return ArrowTypes<T, _s>::make_builder(); }
};

}  // namespace rfl::parsing::tabular
#endif
