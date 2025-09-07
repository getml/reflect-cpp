#ifndef RFL_CSV_READ_HPP_
#define RFL_CSV_READ_HPP_

#include <arrow/csv/reader.h>
#include <arrow/io/api.h>

#include <istream>
#include <memory>
#include <string>
#include <string_view>

#include "../Processors.hpp"
#include "../Result.hpp"
#include "../concepts.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "../parsing/tabular/ArrowReader.hpp"
#include "Settings.hpp"

namespace rfl::csv {

/// Parses an object from CSV using reflection.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const char* _str, const size_t _size,
    const Settings& _settings = Settings{}) {
  arrow::io::IOContext io_context = arrow::io::default_io_context();

  const auto buffer = std::make_shared<arrow::Buffer>(
      internal::ptr_cast<const uint8_t*>(_str), _size);

  std::shared_ptr<arrow::io::InputStream> input =
      std::make_shared<arrow::io::BufferReader>(buffer);

  auto read_options = arrow::csv::ReadOptions::Defaults();
  auto convert_options = arrow::csv::ConvertOptions::Defaults();
  convert_options.null_values =
      std::vector<std::string>({_settings.null_string});
  convert_options.strings_can_be_null = true;

  auto parse_options = arrow::csv::ParseOptions::Defaults();
  parse_options.delimiter = _settings.delimiter;
  parse_options.quoting = _settings.quoting;
  parse_options.quote_char = _settings.quote_char;
  parse_options.double_quote = _settings.double_quote;
  parse_options.escaping = _settings.escaping;
  parse_options.escape_char = _settings.escape_char;
  parse_options.newlines_in_values = _settings.newlines_in_values;
  parse_options.ignore_empty_lines = _settings.ignore_empty_lines;

  auto maybe_reader = arrow::csv::TableReader::Make(
      io_context, input, read_options, parse_options, convert_options);

  if (!maybe_reader.ok()) {
    return error("Could not construct CSV reader: " +
                 maybe_reader.status().message());
  }

  std::shared_ptr<arrow::csv::TableReader> reader = *maybe_reader;

  auto maybe_table = reader->Read();
  if (!maybe_table.ok()) {
    return error("Could not read table: " + maybe_table.status().message());
  }

  const std::shared_ptr<arrow::Table> table = *maybe_table;

  using ArrowReader =
      parsing::tabular::ArrowReader<T, parsing::tabular::SerializationType::csv,
                                    Ps...>;

  return ArrowReader::make(table).and_then(
      [](const auto& _r) { return _r.read(); });
}

/// Parses an object from CSV using reflection.
template <class T, class... Ps>
auto read(const std::string_view _str, const Settings& _settings = Settings{}) {
  return read<T, Ps...>(_str.data(), _str.size(), _settings);
}

/// Parses an object from a stream.
template <class T, class... Ps>
auto read(std::istream& _stream, const Settings& _settings = Settings{}) {
  std::istreambuf_iterator<char> begin(_stream), end;
  auto bytes = std::vector<char>(begin, end);
  return read<T, Ps...>(bytes.data(), bytes.size(), _settings);
}

}  // namespace rfl::csv

#endif
