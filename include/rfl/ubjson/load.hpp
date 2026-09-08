#ifndef RFL_UBJSON_LOAD_HPP_
#define RFL_UBJSON_LOAD_HPP_

#include "../Processors.hpp"
#include "../Result.hpp"
#include "../io/load_bytes.hpp"
#include "read.hpp"

namespace rfl::ubjson {

/// Loads an object from a UBJSON file.
/// Reads a binary file from disk and parses its UBJSON content into a C++ object using compile-time reflection.
/// UBJSON (Universal Binary JSON) is a compact binary format that mirrors JSON's data model.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _fname The filename/path of the UBJSON file to load
/// @return Result containing either the parsed object of type T or an error message
template <class T, class... Ps>
Result<T> load(const std::string& _fname) {
  const auto read_bytes = [](const auto& _bytes) {
    return read<T, Ps...>(_bytes);
  };
  return rfl::io::load_bytes(_fname).and_then(read_bytes);
}

}  // namespace rfl::ubjson

#endif
