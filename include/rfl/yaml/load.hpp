#ifndef RFL_YAML_LOAD_HPP_
#define RFL_YAML_LOAD_HPP_

#include "../Result.hpp"
#include "../io/load_string.hpp"
#include "read.hpp"

namespace rfl {
namespace yaml {

/// Loads an object from a YAML file.
/// Reads a file from disk and parses its YAML content into a C++ object using compile-time reflection.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _fname The filename/path of the YAML file to load
/// @return Result containing either the parsed object of type T or an error message
template <class T, class... Ps>
Result<T> load(const std::string& _fname) {
  const auto read_string = [](const auto& _str) {
    return read<T, Ps...>(_str);
  };
  return rfl::io::load_string(_fname).and_then(read_string);
}

}  // namespace yaml
}  // namespace rfl

#endif
