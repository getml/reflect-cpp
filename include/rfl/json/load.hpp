#ifndef RFL_JSON_LOAD_HPP_
#define RFL_JSON_LOAD_HPP_

#include "../Result.hpp"
#include "../io/load_string.hpp"
#include "read.hpp"

namespace rfl {
namespace json {

/// Loads an object from a JSON file.
/// Reads a file from disk and parses its JSON content into a C++ object using compile-time reflection.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _fname The filename/path of the JSON file to load
/// @param _flag Optional yyjson flags for parsing (default: 0)
/// @return Result containing either the parsed object of type T or an error message
template <class T, class... Ps>
Result<T> load(const std::string& _fname, const yyjson_read_flag _flag = 0) {
  const auto read_string = [_flag](const auto& _str) {
    return read<T, Ps...>(_str, _flag);
  };
  return rfl::io::load_string(_fname).and_then(read_string);
}

}  // namespace json
}  // namespace rfl

#endif
