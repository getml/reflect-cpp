#include "native_reference.hpp"

#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>

#include <stdexcept>

namespace test_parquet_bridge {
namespace {
void check(const arrow::Status& _status) {
  if (!_status.ok()) throw std::runtime_error(_status.message());
}
template <class T>
T unwrap(arrow::Result<T> _value) {
  if (!_value.ok()) throw std::runtime_error(_value.status().message());
  return std::move(_value).ValueOrDie();
}
}  // namespace
std::vector<char> reference_bytes() {
  arrow::UInt16Builder counts;
  check(counts.Append(42));
  check(counts.Append(43));
  arrow::UInt8Builder nullable;
  check(nullable.AppendNull());
  check(nullable.Append(17));
  arrow::FloatBuilder scores;
  check(scores.Append(0.5));
  check(scores.Append(2.5));
  const auto time_type = arrow::timestamp(arrow::TimeUnit::NANO);
  arrow::TimestampBuilder times(time_type, arrow::default_memory_pool());
  check(times.Append(1704067200000000000LL));
  check(times.Append(1704067201000000000LL));
  const auto names =
      unwrap(arrow::MakeArrayFromScalar(arrow::StringScalar("native 世界"), 2));
  const auto blobs = unwrap(arrow::MakeArrayFromScalar(
      arrow::BinaryScalar(std::string("\0\x7f\xff", 3)), 2));
  auto table = arrow::Table::Make(
      arrow::schema({arrow::field("count", arrow::uint16()),
                     arrow::field("maybe", arrow::uint8()),
                     arrow::field("score", arrow::float32()),
                     arrow::field("when", time_type),
                     arrow::field("name", arrow::utf8()),
                     arrow::field("bytes", arrow::binary())}),
      {unwrap(counts.Finish()), unwrap(nullable.Finish()),
       unwrap(scores.Finish()), unwrap(times.Finish()), names, blobs});
  auto stream = unwrap(arrow::io::BufferOutputStream::Create());
  auto properties =
      parquet::ArrowWriterProperties::Builder().store_schema()->build();
  check(parquet::arrow::WriteTable(*table, arrow::default_memory_pool(), stream,
                                   1, parquet::default_writer_properties(),
                                   properties));
  auto buffer = unwrap(stream->Finish());
  return {reinterpret_cast<const char*>(buffer->data()),
          reinterpret_cast<const char*>(buffer->data()) + buffer->size()};
}
bool reference_check(const std::vector<char>& _bytes) {
  auto input =
      std::make_shared<arrow::io::BufferReader>(std::make_shared<arrow::Buffer>(
          reinterpret_cast<const uint8_t*>(_bytes.data()), _bytes.size()));
  auto reader =
      unwrap(parquet::arrow::OpenFile(input, arrow::default_memory_pool()));
  std::shared_ptr<arrow::Table> table;
  check(reader->ReadTable(&table));
  table = unwrap(table->CombineChunks());
  if (table->num_rows() != 2) return false;
  const auto count = std::static_pointer_cast<arrow::Int32Array>(
      table->GetColumnByName("count")->chunk(0));
  const auto maybe = std::static_pointer_cast<arrow::Int64Array>(
      table->GetColumnByName("maybe")->chunk(0));
  const auto score = std::static_pointer_cast<arrow::DoubleArray>(
      table->GetColumnByName("score")->chunk(0));
  const auto when = std::static_pointer_cast<arrow::TimestampArray>(
      table->GetColumnByName("when")->chunk(0));
  const auto name = std::static_pointer_cast<arrow::StringArray>(
      table->GetColumnByName("name")->chunk(0));
  const auto bytes = std::static_pointer_cast<arrow::BinaryArray>(
      table->GetColumnByName("bytes")->chunk(0));
  return count->Value(1) == 43 && maybe->IsNull(0) && maybe->Value(1) == 17 &&
         score->Value(1) == 2.5 && when->Value(1) == 1704067201000LL &&
         name->Value(0) == "native 世界" &&
         bytes->Value(1) == std::string_view("\0\x7f\xff", 3);
}
}  // namespace test_parquet_bridge
