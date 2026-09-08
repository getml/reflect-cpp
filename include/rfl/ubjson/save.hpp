#ifndef RFL_UBJSON_SAVE_HPP_
#define RFL_UBJSON_SAVE_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "../Result.hpp"
#include "../io/save_bytes.hpp"
#include "write.hpp"

namespace rfl::ubjson {

/// Saves an object to a UBJSON file.
/// Serializes a C++ object to UBJSON binary format and writes it to a file using compile-time reflection.
/// UBJSON (Universal Binary JSON) is a compact binary format that mirrors JSON's data model.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _fname The filename/path where the UBJSON file will be saved
/// @param _obj The object to serialize to UBJSON
/// @return Result containing Nothing on success or an error message on failure
template <class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj) {
  const auto write_func = [](const auto& _obj, auto& _stream) -> auto& {
    return write<Ps...>(_obj, _stream);
  };
  return rfl::io::save_bytes(_fname, _obj, write_func);
}

}  // namespace rfl::ubjson

#endif
