#ifndef RFL_CSV_SAVE_HPP_
#define RFL_CSV_SAVE_HPP_

#include <string>

#include "../Result.hpp"
#include "../io/save_string.hpp"
#include "Settings.hpp"
#include "write.hpp"

namespace rfl::csv {

/// Saves an array of objects to a CSV file.
/// Serializes an array of C++ objects to CSV text format and writes it to a file using compile-time reflection.
/// CSV (Comma-Separated Values) is a text format for tabular data, where each row represents an object.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _fname The filename/path where the CSV file will be saved
/// @param _obj The array of objects to serialize to CSV
/// @param _settings Optional CSV writing settings (delimiter, quoting style, etc.)
/// @return Result containing Nothing on success or an error message on failure
template <class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj,
                     const Settings& _settings = Settings{}) {
  const auto write_func = [&](const auto& _obj, auto& _stream) -> auto& {
    return write<Ps...>(_obj, _stream, _settings);
  };
  return rfl::io::save_string(_fname, _obj, write_func);
}

}  // namespace rfl::csv

#endif
