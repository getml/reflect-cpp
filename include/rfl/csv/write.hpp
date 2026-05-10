#ifndef RFL_CSV_WRITE_HPP_
#define RFL_CSV_WRITE_HPP_

#include <arrow/csv/api.h>
#include <arrow/csv/writer.h>
#include <arrow/io/api.h>

#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>

//#include "../Processors.hpp"
//#include "../Ref.hpp"
#include "../parsing/tabular/ArrowWriter.hpp"
#include "Settings.hpp"

namespace rfl::csv {

/// Converts an array of objects to CSV and returns an Arrow buffer.
/// CSV (Comma-Separated Values) is a text format for tabular data where each line represents a row.
/// Uses Apache Arrow for efficient CSV generation. Each struct in the array becomes a row,
/// and each field becomes a column.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _arr The array of objects to serialize to CSV
/// @param _settings CSV writing settings (delimiter, quoting style, batch size, etc.)
/// @return A reference-counted Arrow buffer containing the CSV text
template <class... Ps>
Ref<arrow::Buffer> to_buffer(const auto& _arr, const Settings& _settings) {
  using T = std::remove_cvref_t<decltype(_arr)>;

  const auto table =
      parsing::tabular::ArrowWriter<T, parsing::tabular::SerializationType::csv,
                                    Ps...>(_settings.batch_size)
          .to_table(_arr);

  const auto output_buffer = arrow::io::BufferOutputStream::Create();

  if (!output_buffer.ok()) {
    throw std::runtime_error(output_buffer.status().message());
  }

  auto options = arrow::csv::WriteOptions::Defaults();
  options.batch_size = _settings.batch_size;
  options.delimiter = _settings.delimiter;
  options.null_string = _settings.null_string;
  options.quoting_style = _settings.quoting ? arrow::csv::QuotingStyle::Needed
                                            : arrow::csv::QuotingStyle::None;

  const auto status =
      arrow::csv::WriteCSV(*table, options, output_buffer.ValueOrDie().get());

  if (!status.ok()) {
    throw std::runtime_error(status.message());
  }

  const auto buffer = output_buffer.ValueOrDie()->Finish();

  if (!buffer.ok()) {
    throw std::runtime_error(output_buffer.status().message());
  }

  return Ref<arrow::Buffer>::make(buffer.ValueOrDie()).value();
}

/// Writes an array of objects to CSV format.
/// Uses compile-time reflection to serialize an array of C++ objects to CSV format.
/// Each struct in the array becomes a row, and each field becomes a column with a header.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _arr The array of objects to serialize to CSV
/// @param _settings Optional CSV writing settings (delimiter, quoting style, etc.)
/// @return A string containing the CSV representation
template <class... Ps>
std::string write(const auto& _arr, const Settings& _settings = Settings{}) {
  const auto buffer = to_buffer<Ps...>(_arr, _settings);
  const auto view = std::string_view(*buffer);
  return std::string(view);
}

/// Writes a CSV representation into an ostream.
/// Uses compile-time reflection to serialize an array of C++ objects to CSV and write to a stream.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _arr The array of objects to serialize to CSV
/// @param _stream The output stream to write CSV text to
/// @param _settings Optional CSV writing settings (delimiter, quoting style, etc.)
/// @return The output stream (for chaining)
template <class... Ps>
std::ostream& write(const auto& _arr, std::ostream& _stream,
                    const Settings& _settings = Settings{}) {
  auto buffer = to_buffer<Ps...>(_arr, _settings);
  _stream << std::string_view(*buffer);
  return _stream;
}

}  // namespace rfl::csv

#endif
