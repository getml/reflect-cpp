#ifndef RFL_CSV_READ_HPP_
#define RFL_CSV_READ_HPP_

#include <istream>
#include <string>
#include <string_view>

#include "../Result.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "../parsing/tabular/ArrowReader.hpp"
#include "Settings.hpp"
#include "Table.hpp"

namespace rfl::csv {

/// Parses an object from CSV bytes using reflection (with raw pointer and
/// size). CSV (Comma-Separated Values) is a text format for tabular data where
/// each line represents a row. Uses Apache Arrow for efficient CSV parsing. The
/// input must be an array of structs, where each struct becomes a row and each
/// field becomes a column.
/// @tparam T The type to parse into (must be an array of structs)
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _str Pointer to CSV text data
/// @param _size The size of the CSV data in bytes
/// @param _settings Optional CSV parsing settings (delimiter, quoting, etc.)
/// @return Result containing either an array of parsed objects or an error
/// message
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const char* _str, const size_t _size,
    const Settings& _settings = Settings{}) {
  const auto table = detail::read_table(_str, _size, _settings);
  if (!table) return error(table.error().what());

  using ArrowReader =
      parsing::tabular::ArrowReader<T, parsing::tabular::SerializationType::csv,
                                    Ps...>;

  return ArrowReader::make(*table).and_then(
      [](const auto& _r) { return _r.read(); });
}

/// Parses an object from CSV using reflection (string_view version).
/// @tparam T The type to parse into (must be an array of structs)
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _str The CSV string to parse
/// @param _settings Optional CSV parsing settings (delimiter, quoting, etc.)
/// @return Result containing either an array of parsed objects or an error
/// message
template <class T, class... Ps>
auto read(const std::string_view _str, const Settings& _settings = Settings{}) {
  return read<T, Ps...>(_str.data(), _str.size(), _settings);
}

/// Parses an object from a stream containing CSV data.
/// Reads CSV text from the stream and constructs an array of C++ objects.
/// @tparam T The type to parse into (must be an array of structs)
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _stream The input stream containing CSV text
/// @param _settings Optional CSV parsing settings (delimiter, quoting, etc.)
/// @return Result containing either an array of parsed objects or an error
/// message
template <class T, class... Ps>
auto read(std::istream& _stream, const Settings& _settings = Settings{}) {
  std::istreambuf_iterator<char> begin(_stream), end;
  auto bytes = std::vector<char>(begin, end);
  return read<T, Ps...>(bytes.data(), bytes.size(), _settings);
}

}  // namespace rfl::csv

#endif
