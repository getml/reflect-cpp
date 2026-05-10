#ifndef RFL_PARQUET_WRITE_HPP_
#define RFL_PARQUET_WRITE_HPP_

#include <arrow/io/api.h>
#include <parquet/arrow/writer.h>

#include <ostream>
#include <stdexcept>
#include <string_view>

#include "../parsing/tabular/ArrowWriter.hpp"
#include "Settings.hpp"

namespace rfl::parquet {

/// @brief Returns parquet bytes as an Arrow buffer.
/// @tparam Ps The processors used for serialization (e.g., transformations
/// applied to the data).
/// @param _arr The input array to serialize.
/// @param _settings Serialization settings.
/// @return Ref<arrow::Buffer> containing the parquet bytes.
template <class... Ps>
Ref<arrow::Buffer> to_buffer(const auto& _arr, const Settings& _settings) {
  using T = std::remove_cvref_t<decltype(_arr)>;

  const auto table =
      parsing::tabular::ArrowWriter<
          T, parsing::tabular::SerializationType::parquet, Ps...>(
          _settings.chunksize)
          .to_table(_arr);

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

/// @brief Returns parquet bytes as a vector of chars.
/// @tparam Ps The processors used for serialization (e.g., transformations
/// applied to the data).
/// @param _arr The input array to serialize.
/// @param _settings Serialization settings (optional).
/// @return std::vector<char> containing the parquet bytes.
template <class... Ps>
std::vector<char> write(const auto& _arr,
                        const Settings& _settings = Settings{}) {
  const auto buffer = to_buffer<Ps...>(_arr, _settings);
  const auto view = std::string_view(*buffer);
  return std::vector<char>(view.begin(), view.end());
}

/// @brief Writes a PARQUET representation of the array into an ostream.
/// @tparam Ps The processors used for serialization (e.g., transformations
/// applied to the data).
/// @param _arr The input array to serialize.
/// @param _stream The output stream to write to.
/// @param _settings Serialization settings (optional).
/// @return Reference to the output stream.
template <class... Ps>
std::ostream& write(const auto& _arr, std::ostream& _stream,
                    const Settings& _settings = Settings{}) {
  auto buffer = to_buffer<Ps...>(_arr, _settings);
  _stream << std::string_view(*buffer);
  return _stream;
}

}  // namespace rfl::parquet

#endif
