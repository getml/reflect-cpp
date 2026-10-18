#include "NativeArrowTypes.hpp"
#include "rfl/parsing/tabular/ArrowTypes.hpp"

namespace rfl::parsing::tabular {
namespace {
void check_status(const arrow::Status& _status) {
  if (!_status.ok()) throw std::runtime_error(_status.message());
}
}  // namespace
ArrowBuilder::ArrowBuilder(std::shared_ptr<arrow::ArrayBuilder> _impl)
    : impl_(std::move(_impl)) {}
void ArrowBuilder::append_null() {
  check_status(tabular::append_null(impl_.get()));
}
std::shared_ptr<arrow::Array> ArrowBuilder::finish() {
  std::shared_ptr<arrow::Array> result;
  check_status(impl_->Finish(&result));
  return result;
}
arrow::ArrayBuilder* ArrowBuilder::get() const { return impl_.get(); }

std::shared_ptr<arrow::DataType> ArrowScalar<bool>::data_type() {
  return NativeArrowTypes<bool, SerializationType::parquet>::data_type();
}
ArrowBuilder ArrowScalar<bool>::make_builder() {
  using Native = NativeArrowTypes<bool, SerializationType::parquet>;
  return ArrowBuilder(std::make_shared<typename Native::BuilderType>());
}
void ArrowScalar<bool>::add_to_builder(const bool& _value,
                                       BuilderType* _builder) {
  using Native = NativeArrowTypes<bool, SerializationType::parquet>;
  Native::add_to_builder(
      _value, static_cast<typename Native::BuilderType*>(_builder->get()));
}
Result<Ref<arrow::Array>> ArrowScalar<bool>::get_array(
    const std::shared_ptr<arrow::Array>& _arr) {
  return NativeArrowTypes<bool, SerializationType::parquet>::get_array(_arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
Result<bool> ArrowScalar<bool>::get_value(const Ref<ArrayType>& _arr,
                                          int64_t _ix) {
  using Native = NativeArrowTypes<bool, SerializationType::parquet>;
  return Native::get_value(
      Ref<typename Native::ArrayType>::make(
          std::static_pointer_cast<typename Native::ArrayType>(_arr.ptr()))
          .value(),
      _ix);
}
std::shared_ptr<arrow::DataType> ArrowScalar<uint8_t>::data_type() {
  return NativeArrowTypes<uint8_t, SerializationType::parquet>::data_type();
}
ArrowBuilder ArrowScalar<uint8_t>::make_builder() {
  using Native = NativeArrowTypes<uint8_t, SerializationType::parquet>;
  return ArrowBuilder(std::make_shared<typename Native::BuilderType>());
}
void ArrowScalar<uint8_t>::add_to_builder(const uint8_t& _value,
                                          BuilderType* _builder) {
  using Native = NativeArrowTypes<uint8_t, SerializationType::parquet>;
  Native::add_to_builder(
      _value, static_cast<typename Native::BuilderType*>(_builder->get()));
}
Result<Ref<arrow::Array>> ArrowScalar<uint8_t>::get_array(
    const std::shared_ptr<arrow::Array>& _arr) {
  return NativeArrowTypes<uint8_t, SerializationType::parquet>::get_array(_arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
Result<uint8_t> ArrowScalar<uint8_t>::get_value(const Ref<ArrayType>& _arr,
                                                int64_t _ix) {
  using Native = NativeArrowTypes<uint8_t, SerializationType::parquet>;
  return Native::get_value(
      Ref<typename Native::ArrayType>::make(
          std::static_pointer_cast<typename Native::ArrayType>(_arr.ptr()))
          .value(),
      _ix);
}
std::shared_ptr<arrow::DataType> ArrowScalar<uint16_t>::data_type() {
  return NativeArrowTypes<uint16_t, SerializationType::parquet>::data_type();
}
ArrowBuilder ArrowScalar<uint16_t>::make_builder() {
  using Native = NativeArrowTypes<uint16_t, SerializationType::parquet>;
  return ArrowBuilder(std::make_shared<typename Native::BuilderType>());
}
void ArrowScalar<uint16_t>::add_to_builder(const uint16_t& _value,
                                           BuilderType* _builder) {
  using Native = NativeArrowTypes<uint16_t, SerializationType::parquet>;
  Native::add_to_builder(
      _value, static_cast<typename Native::BuilderType*>(_builder->get()));
}
Result<Ref<arrow::Array>> ArrowScalar<uint16_t>::get_array(
    const std::shared_ptr<arrow::Array>& _arr) {
  return NativeArrowTypes<uint16_t, SerializationType::parquet>::get_array(_arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
Result<uint16_t> ArrowScalar<uint16_t>::get_value(const Ref<ArrayType>& _arr,
                                                  int64_t _ix) {
  using Native = NativeArrowTypes<uint16_t, SerializationType::parquet>;
  return Native::get_value(
      Ref<typename Native::ArrayType>::make(
          std::static_pointer_cast<typename Native::ArrayType>(_arr.ptr()))
          .value(),
      _ix);
}
std::shared_ptr<arrow::DataType> ArrowScalar<uint32_t>::data_type() {
  return NativeArrowTypes<uint32_t, SerializationType::parquet>::data_type();
}
ArrowBuilder ArrowScalar<uint32_t>::make_builder() {
  using Native = NativeArrowTypes<uint32_t, SerializationType::parquet>;
  return ArrowBuilder(std::make_shared<typename Native::BuilderType>());
}
void ArrowScalar<uint32_t>::add_to_builder(const uint32_t& _value,
                                           BuilderType* _builder) {
  using Native = NativeArrowTypes<uint32_t, SerializationType::parquet>;
  Native::add_to_builder(
      _value, static_cast<typename Native::BuilderType*>(_builder->get()));
}
Result<Ref<arrow::Array>> ArrowScalar<uint32_t>::get_array(
    const std::shared_ptr<arrow::Array>& _arr) {
  return NativeArrowTypes<uint32_t, SerializationType::parquet>::get_array(_arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
Result<uint32_t> ArrowScalar<uint32_t>::get_value(const Ref<ArrayType>& _arr,
                                                  int64_t _ix) {
  using Native = NativeArrowTypes<uint32_t, SerializationType::parquet>;
  return Native::get_value(
      Ref<typename Native::ArrayType>::make(
          std::static_pointer_cast<typename Native::ArrayType>(_arr.ptr()))
          .value(),
      _ix);
}
std::shared_ptr<arrow::DataType> ArrowScalar<uint64_t>::data_type() {
  return NativeArrowTypes<uint64_t, SerializationType::parquet>::data_type();
}
ArrowBuilder ArrowScalar<uint64_t>::make_builder() {
  using Native = NativeArrowTypes<uint64_t, SerializationType::parquet>;
  return ArrowBuilder(std::make_shared<typename Native::BuilderType>());
}
void ArrowScalar<uint64_t>::add_to_builder(const uint64_t& _value,
                                           BuilderType* _builder) {
  using Native = NativeArrowTypes<uint64_t, SerializationType::parquet>;
  Native::add_to_builder(
      _value, static_cast<typename Native::BuilderType*>(_builder->get()));
}
Result<Ref<arrow::Array>> ArrowScalar<uint64_t>::get_array(
    const std::shared_ptr<arrow::Array>& _arr) {
  return NativeArrowTypes<uint64_t, SerializationType::parquet>::get_array(_arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
Result<uint64_t> ArrowScalar<uint64_t>::get_value(const Ref<ArrayType>& _arr,
                                                  int64_t _ix) {
  using Native = NativeArrowTypes<uint64_t, SerializationType::parquet>;
  return Native::get_value(
      Ref<typename Native::ArrayType>::make(
          std::static_pointer_cast<typename Native::ArrayType>(_arr.ptr()))
          .value(),
      _ix);
}
std::shared_ptr<arrow::DataType> ArrowScalar<int8_t>::data_type() {
  return NativeArrowTypes<int8_t, SerializationType::parquet>::data_type();
}
ArrowBuilder ArrowScalar<int8_t>::make_builder() {
  using Native = NativeArrowTypes<int8_t, SerializationType::parquet>;
  return ArrowBuilder(std::make_shared<typename Native::BuilderType>());
}
void ArrowScalar<int8_t>::add_to_builder(const int8_t& _value,
                                         BuilderType* _builder) {
  using Native = NativeArrowTypes<int8_t, SerializationType::parquet>;
  Native::add_to_builder(
      _value, static_cast<typename Native::BuilderType*>(_builder->get()));
}
Result<Ref<arrow::Array>> ArrowScalar<int8_t>::get_array(
    const std::shared_ptr<arrow::Array>& _arr) {
  return NativeArrowTypes<int8_t, SerializationType::parquet>::get_array(_arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
Result<int8_t> ArrowScalar<int8_t>::get_value(const Ref<ArrayType>& _arr,
                                              int64_t _ix) {
  using Native = NativeArrowTypes<int8_t, SerializationType::parquet>;
  return Native::get_value(
      Ref<typename Native::ArrayType>::make(
          std::static_pointer_cast<typename Native::ArrayType>(_arr.ptr()))
          .value(),
      _ix);
}
std::shared_ptr<arrow::DataType> ArrowScalar<int16_t>::data_type() {
  return NativeArrowTypes<int16_t, SerializationType::parquet>::data_type();
}
ArrowBuilder ArrowScalar<int16_t>::make_builder() {
  using Native = NativeArrowTypes<int16_t, SerializationType::parquet>;
  return ArrowBuilder(std::make_shared<typename Native::BuilderType>());
}
void ArrowScalar<int16_t>::add_to_builder(const int16_t& _value,
                                          BuilderType* _builder) {
  using Native = NativeArrowTypes<int16_t, SerializationType::parquet>;
  Native::add_to_builder(
      _value, static_cast<typename Native::BuilderType*>(_builder->get()));
}
Result<Ref<arrow::Array>> ArrowScalar<int16_t>::get_array(
    const std::shared_ptr<arrow::Array>& _arr) {
  return NativeArrowTypes<int16_t, SerializationType::parquet>::get_array(_arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
Result<int16_t> ArrowScalar<int16_t>::get_value(const Ref<ArrayType>& _arr,
                                                int64_t _ix) {
  using Native = NativeArrowTypes<int16_t, SerializationType::parquet>;
  return Native::get_value(
      Ref<typename Native::ArrayType>::make(
          std::static_pointer_cast<typename Native::ArrayType>(_arr.ptr()))
          .value(),
      _ix);
}
std::shared_ptr<arrow::DataType> ArrowScalar<int32_t>::data_type() {
  return NativeArrowTypes<int32_t, SerializationType::parquet>::data_type();
}
ArrowBuilder ArrowScalar<int32_t>::make_builder() {
  using Native = NativeArrowTypes<int32_t, SerializationType::parquet>;
  return ArrowBuilder(std::make_shared<typename Native::BuilderType>());
}
void ArrowScalar<int32_t>::add_to_builder(const int32_t& _value,
                                          BuilderType* _builder) {
  using Native = NativeArrowTypes<int32_t, SerializationType::parquet>;
  Native::add_to_builder(
      _value, static_cast<typename Native::BuilderType*>(_builder->get()));
}
Result<Ref<arrow::Array>> ArrowScalar<int32_t>::get_array(
    const std::shared_ptr<arrow::Array>& _arr) {
  return NativeArrowTypes<int32_t, SerializationType::parquet>::get_array(_arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
Result<int32_t> ArrowScalar<int32_t>::get_value(const Ref<ArrayType>& _arr,
                                                int64_t _ix) {
  using Native = NativeArrowTypes<int32_t, SerializationType::parquet>;
  return Native::get_value(
      Ref<typename Native::ArrayType>::make(
          std::static_pointer_cast<typename Native::ArrayType>(_arr.ptr()))
          .value(),
      _ix);
}
std::shared_ptr<arrow::DataType> ArrowScalar<int64_t>::data_type() {
  return NativeArrowTypes<int64_t, SerializationType::parquet>::data_type();
}
ArrowBuilder ArrowScalar<int64_t>::make_builder() {
  using Native = NativeArrowTypes<int64_t, SerializationType::parquet>;
  return ArrowBuilder(std::make_shared<typename Native::BuilderType>());
}
void ArrowScalar<int64_t>::add_to_builder(const int64_t& _value,
                                          BuilderType* _builder) {
  using Native = NativeArrowTypes<int64_t, SerializationType::parquet>;
  Native::add_to_builder(
      _value, static_cast<typename Native::BuilderType*>(_builder->get()));
}
Result<Ref<arrow::Array>> ArrowScalar<int64_t>::get_array(
    const std::shared_ptr<arrow::Array>& _arr) {
  return NativeArrowTypes<int64_t, SerializationType::parquet>::get_array(_arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
Result<int64_t> ArrowScalar<int64_t>::get_value(const Ref<ArrayType>& _arr,
                                                int64_t _ix) {
  using Native = NativeArrowTypes<int64_t, SerializationType::parquet>;
  return Native::get_value(
      Ref<typename Native::ArrayType>::make(
          std::static_pointer_cast<typename Native::ArrayType>(_arr.ptr()))
          .value(),
      _ix);
}
std::shared_ptr<arrow::DataType> ArrowScalar<float>::data_type() {
  return NativeArrowTypes<float, SerializationType::parquet>::data_type();
}
ArrowBuilder ArrowScalar<float>::make_builder() {
  using Native = NativeArrowTypes<float, SerializationType::parquet>;
  return ArrowBuilder(std::make_shared<typename Native::BuilderType>());
}
void ArrowScalar<float>::add_to_builder(const float& _value,
                                        BuilderType* _builder) {
  using Native = NativeArrowTypes<float, SerializationType::parquet>;
  Native::add_to_builder(
      _value, static_cast<typename Native::BuilderType*>(_builder->get()));
}
Result<Ref<arrow::Array>> ArrowScalar<float>::get_array(
    const std::shared_ptr<arrow::Array>& _arr) {
  return NativeArrowTypes<float, SerializationType::parquet>::get_array(_arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
Result<float> ArrowScalar<float>::get_value(const Ref<ArrayType>& _arr,
                                            int64_t _ix) {
  using Native = NativeArrowTypes<float, SerializationType::parquet>;
  return Native::get_value(
      Ref<typename Native::ArrayType>::make(
          std::static_pointer_cast<typename Native::ArrayType>(_arr.ptr()))
          .value(),
      _ix);
}
std::shared_ptr<arrow::DataType> ArrowScalar<double>::data_type() {
  return NativeArrowTypes<double, SerializationType::parquet>::data_type();
}
ArrowBuilder ArrowScalar<double>::make_builder() {
  using Native = NativeArrowTypes<double, SerializationType::parquet>;
  return ArrowBuilder(std::make_shared<typename Native::BuilderType>());
}
void ArrowScalar<double>::add_to_builder(const double& _value,
                                         BuilderType* _builder) {
  using Native = NativeArrowTypes<double, SerializationType::parquet>;
  Native::add_to_builder(
      _value, static_cast<typename Native::BuilderType*>(_builder->get()));
}
Result<Ref<arrow::Array>> ArrowScalar<double>::get_array(
    const std::shared_ptr<arrow::Array>& _arr) {
  return NativeArrowTypes<double, SerializationType::parquet>::get_array(_arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
Result<double> ArrowScalar<double>::get_value(const Ref<ArrayType>& _arr,
                                              int64_t _ix) {
  using Native = NativeArrowTypes<double, SerializationType::parquet>;
  return Native::get_value(
      Ref<typename Native::ArrayType>::make(
          std::static_pointer_cast<typename Native::ArrayType>(_arr.ptr()))
          .value(),
      _ix);
}
std::shared_ptr<arrow::DataType> ArrowScalar<std::string>::data_type() {
  return NativeArrowTypes<std::string, SerializationType::parquet>::data_type();
}
ArrowBuilder ArrowScalar<std::string>::make_builder() {
  using Native = NativeArrowTypes<std::string, SerializationType::parquet>;
  return ArrowBuilder(std::make_shared<typename Native::BuilderType>());
}
void ArrowScalar<std::string>::add_to_builder(const std::string& _value,
                                              BuilderType* _builder) {
  using Native = NativeArrowTypes<std::string, SerializationType::parquet>;
  Native::add_to_builder(
      _value, static_cast<typename Native::BuilderType*>(_builder->get()));
}
Result<Ref<arrow::Array>> ArrowScalar<std::string>::get_array(
    const std::shared_ptr<arrow::Array>& _arr) {
  return NativeArrowTypes<std::string, SerializationType::parquet>::get_array(
             _arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
Result<std::string> ArrowScalar<std::string>::get_value(
    const Ref<ArrayType>& _arr, int64_t _ix) {
  using Native = NativeArrowTypes<std::string, SerializationType::parquet>;
  return Native::get_value(
      Ref<typename Native::ArrayType>::make(
          std::static_pointer_cast<typename Native::ArrayType>(_arr.ptr()))
          .value(),
      _ix);
}
std::shared_ptr<arrow::DataType> binary_data_type() { return arrow::binary(); }
ArrowBuilder make_binary_builder() {
  return ArrowBuilder(std::make_shared<arrow::BinaryBuilder>());
}
void append_binary(ArrowBuilder* _builder, const uint8_t* _bytes,
                   size_t _size) {
  check_status(static_cast<arrow::BinaryBuilder*>(_builder->get())
                   ->Append(_bytes, _size));
}
Result<Ref<arrow::Array>> binary_array(
    const std::shared_ptr<arrow::Array>& _arr) {
  return NativeArrowTypes<std::vector<char>,
                          SerializationType::parquet>::get_array(_arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
std::string_view binary_value(const Ref<arrow::Array>& _arr, int64_t _ix) {
  return static_cast<arrow::BinaryArray*>(_arr.get())->Value(_ix);
}
std::shared_ptr<arrow::DataType> timestamp_data_type() {
  return arrow::timestamp(arrow::TimeUnit::MILLI);
}
ArrowBuilder make_timestamp_builder() {
  return ArrowBuilder(std::make_shared<arrow::TimestampBuilder>(
      timestamp_data_type(), arrow::default_memory_pool()));
}
void append_timestamp(ArrowBuilder* _builder, int64_t _value) {
  check_status(
      static_cast<arrow::TimestampBuilder*>(_builder->get())->Append(_value));
}
Result<Ref<arrow::Array>> timestamp_array(
    const std::shared_ptr<arrow::Array>& _arr,
    Result<int64_t> (*_parse)(std::string_view)) {
  // The format-dependent conversion is supplied by the importing template.
  // Arrow types and builders stay in this translation unit.
  if (_arr->type()->Equals(arrow::utf8())) {
    const auto strings = std::static_pointer_cast<arrow::StringArray>(_arr);
    auto builder = make_timestamp_builder();
    for (int64_t i = 0; i < strings->length(); ++i) {
      if (strings->IsNull(i))
        builder.append_null();
      else {
        const auto value = _parse(strings->Value(i));
        if (!value) return error(value.error().what());
        append_timestamp(&builder, *value);
      }
    }
    return Ref<arrow::Array>::make(builder.finish());
  }
  // Non-string timestamps do not depend on the user's format.
  return NativeArrowTypes<Timestamp<"%Y-%m-%d">,
                          SerializationType::parquet>::get_array(_arr)
      .and_then([](const auto& _value) {
        return Ref<arrow::Array>::make(
            std::static_pointer_cast<arrow::Array>(_value.ptr()));
      });
}
int64_t timestamp_value(const Ref<arrow::Array>& _arr, int64_t _ix) {
  return static_cast<arrow::TimestampArray*>(_arr.get())->Value(_ix);
}
bool array_is_null(const Ref<arrow::Array>& _arr, int64_t _ix) {
  return _arr->IsNull(_ix);
}
int64_t array_length(const Ref<arrow::Array>& _arr) { return _arr->length(); }
int num_chunks(const Ref<arrow::ChunkedArray>& _arr) {
  return _arr->num_chunks();
}
std::shared_ptr<arrow::Array> array_chunk(const Ref<arrow::ChunkedArray>& _arr,
                                          int _ix) {
  return _arr->chunk(_ix);
}
Result<Ref<arrow::ChunkedArray>> table_column(const Ref<arrow::Table>& _table,
                                              const std::string& _name) {
  const auto column = _table->GetColumnByName(_name);
  if (!column) return error("Column named '" + _name + "' not found.");
  return Ref<arrow::ChunkedArray>::make(column);
}
std::shared_ptr<arrow::Field> make_arrow_field(
    const std::string& _name, const std::shared_ptr<arrow::DataType>& _type) {
  return arrow::field(_name, _type);
}
std::shared_ptr<arrow::Schema> make_schema(
    const std::vector<std::shared_ptr<arrow::Field>>& _fields) {
  return arrow::schema(_fields);
}
std::shared_ptr<arrow::Table> make_table(
    const std::shared_ptr<arrow::Schema>& _schema,
    const std::vector<std::shared_ptr<arrow::ChunkedArray>>& _arrays) {
  return arrow::Table::Make(_schema, _arrays);
}
std::shared_ptr<arrow::ChunkedArray> make_chunked_array(
    const std::vector<std::shared_ptr<arrow::Array>>& _arrays,
    const std::shared_ptr<arrow::DataType>& _type) {
  return std::make_shared<arrow::ChunkedArray>(_arrays, _type);
}
std::string_view buffer_view(const Ref<arrow::Buffer>& _buffer) {
  return std::string_view(*_buffer);
}
}  // namespace rfl::parsing::tabular
