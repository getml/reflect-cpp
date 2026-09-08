#ifndef RFL_CSV_CSV_HPP_
#define RFL_CSV_CSV_HPP_

#include "../Result.hpp"
#include "../io/load_string.hpp"
#include "Settings.hpp"
#include "read.hpp"

namespace rfl::csv {

/// Loads an array of objects from a CSV file.
/// Reads a text file from disk and parses its CSV content into an array of C++ objects using compile-time reflection.
/// CSV (Comma-Separated Values) is a text format for tabular data, where each row represents an object.
/// @tparam T The type to parse into (must be an array of structs)
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _fname The filename/path of the CSV file to load
/// @param _settings Optional CSV parsing settings (delimiter, quoting, etc.)
/// @return Result containing either an array of parsed objects or an error message
template <class T, class... Ps>
Result<T> load(const std::string& _fname,
               const Settings& _settings = Settings{}) {
  const auto read_string = [&](const auto& _str) {
    return read<T, Ps...>(_str, _settings);
  };
  return rfl::io::load_string(_fname).and_then(read_string);
}

}  // namespace rfl::csv

#endif
