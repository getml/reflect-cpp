#ifndef RFL_CSV_READ_HPP_
#define RFL_CSV_READ_HPP_

#include <arrow/csv/reader.h>
#include <arrow/io/api.h>

#include <istream>
#include <memory>
#include <string>

#include "../Processors.hpp"
#include "../Result.hpp"
#include "../concepts.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "../parsing/tabular/ArrowReader.hpp"

namespace rfl::csv {

/// Parses an object from CSV using reflection.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(const char* _str,
                                              const size_t _size) {
  arrow::io::IOContext io_context = arrow::io::default_io_context();

  const auto buffer = std::make_shared<arrow::Buffer>(
      internal::ptr_cast<const uint8_t*>(_str), _size);

  std::shared_ptr<arrow::io::InputStream> input =
      std::make_shared<arrow::io::BufferReader>(buffer);

  auto read_options = arrow::csv::ReadOptions::Defaults();
  auto parse_options = arrow::csv::ParseOptions::Defaults();
  auto convert_options = arrow::csv::ConvertOptions::Defaults();

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

  using ArrowReader = parsing::tabular::ArrowReader<T, Ps...>;

  return ArrowReader::make(table).and_then(
      [](const auto& _r) { return _r.read(); });
}

/// Parses an object from CSV using reflection.
template <class T, class... Ps>
auto read(const std::string& _str) {
  return read<T, Ps...>(_str.c_str(), _str.size());
}

/// Parses an object from a stream.
template <class T, class... Ps>
auto read(std::istream& _stream) {
  std::istreambuf_iterator<char> begin(_stream), end;
  auto bytes = std::vector<char>(begin, end);
  return read<T, Ps...>(bytes.data(), bytes.size());
}

}  // namespace rfl::csv

#endif
