#ifndef RFL_CBOR_LOAD_HPP_
#define RFL_CBOR_LOAD_HPP_

//#include "../Processors.hpp"
#include "../Result.hpp"
#include "../io/load_bytes.hpp"
#include "read.hpp"

namespace rfl::cbor {

/// Loads an object from a CBOR file.
/// Reads a binary file from disk and parses its CBOR content into a C++ object using compile-time reflection.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _fname The filename/path of the CBOR file to load
/// @return Result containing either the parsed object of type T or an error message
template <class T, class... Ps>
Result<T> load(const std::string& _fname) {
  const auto read_bytes = [](const auto& _bytes) {
    return read<T, Ps...>(_bytes);
  };
  return rfl::io::load_bytes(_fname).and_then(read_bytes);
}

}  // namespace rfl::cbor

#endif
