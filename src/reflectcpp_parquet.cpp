#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>

#include "rfl/parquet/Table.hpp"

namespace rfl::parquet::detail {
Result<std::shared_ptr<arrow::Table>> read_table(const uint8_t* _bytes,
                                                 const size_t _size) {
  arrow::MemoryPool* pool = arrow::default_memory_pool();

  const auto buffer = std::make_shared<arrow::Buffer>(
      internal::ptr_cast<const uint8_t*>(_bytes), _size);

  const auto input = std::make_shared<arrow::io::BufferReader>(buffer);

  auto arrow_reader = ::parquet::arrow::OpenFile(input, pool);

  if (!arrow_reader.ok()) {
    return error(std::string("Could not generate the arrow reader: ") +
                 arrow_reader.status().message());
  }

  std::shared_ptr<arrow::Table> table;

  const auto status = arrow_reader.ValueOrDie()->ReadTable(&table);

  if (!status.ok()) {
    return error("Could not read table: " + status.message());
  }

  return table;
}
Ref<arrow::Buffer> write_table(const std::shared_ptr<arrow::Table>& table,
                               const Settings& _settings) {
  const auto props = ::parquet::WriterProperties::Builder()
                         .compression(_settings.compression)
                         ->build();

  const auto arrow_props =
      ::parquet::ArrowWriterProperties::Builder().store_schema()->build();

  const auto output_buffer = arrow::io::BufferOutputStream::Create();

  if (!output_buffer.ok()) {
    throw std::runtime_error(output_buffer.status().message());
  }

  const auto status = ::parquet::arrow::WriteTable(
      *table.get(), arrow::default_memory_pool(), output_buffer.ValueOrDie(),
      _settings.chunksize, props, arrow_props);

  if (!status.ok()) {
    throw std::runtime_error(status.message());
  }

  const auto buffer = output_buffer.ValueOrDie()->Finish();

  if (!buffer.ok()) {
    throw std::runtime_error(output_buffer.status().message());
  }

  return Ref<arrow::Buffer>::make(buffer.ValueOrDie()).value();
}
}  // namespace rfl::parquet::detail
