#ifndef RFL_PARQUET_WRITE_HPP_
#define RFL_PARQUET_WRITE_HPP_

#include <arrow/io/api.h>
#include <parquet/arrow/writer.h>

#include <cstdint>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "../Processors.hpp"
#include "../parsing/tabular/ArrowWriter.hpp"

namespace rfl::parquet {

/// Returns parquet bytes.
template <class... Ps>
std::vector<char> write(const auto& _arr) {
  /// TODO: Support processors
  using T = std::remove_cvref_t<decltype(_arr)>;
  const auto table =
      parsing::tabular::ArrowWriter<T>(/*chunksize=*/2000).to_table(_arr);

  const auto props = ::parquet::WriterProperties::Builder()
                         .compression(arrow::Compression::SNAPPY)
                         ->build();

  const auto arrow_props =
      ::parquet::ArrowWriterProperties::Builder().store_schema()->build();

  const auto output_buffer = arrow::io::BufferOutputStream::Create();

  if (!output_buffer.ok()) {
    throw std::runtime_error(output_buffer.status().message());
  }

  const auto status = ::parquet::arrow::WriteTable(
      *table.get(), arrow::default_memory_pool(), output_buffer.ValueOrDie(),
      /*chunk_size=*/2000, props, arrow_props);

  if (!status.ok()) {
    throw std::runtime_error(status.message());
  }

  const auto buffer = output_buffer.ValueOrDie()->Finish();

  if (!buffer.ok()) {
    throw std::runtime_error(output_buffer.status().message());
  }

  const auto view = std::string_view(*buffer.ValueOrDie());

  return std::vector<char>(view.begin(), view.end());
}

/// Writes a PARQUET into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) noexcept {
  auto buffer = write<Ps...>(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace rfl::parquet

#endif
