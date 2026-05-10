#ifndef RFL_AVRO_SAVE_HPP_
#define RFL_AVRO_SAVE_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "../Result.hpp"
#include "../io/save_bytes.hpp"
#include "write.hpp"

namespace rfl::avro {

/// Saves an object to a file in Avro binary format.
/// Serializes the object and writes it to the specified file path.
/// @tparam Ps Optional processors to apply during serialization
/// @param _fname The path to the file to save to
/// @param _obj The object to serialize
/// @return A Result containing Nothing on success or an error
template <class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj) {
  const auto write_func = [](const auto& _obj,
                             std::ostream& _stream) -> std::ostream& {
    return write<Ps...>(_obj, _stream);
  };
  return rfl::io::save_bytes(_fname, _obj, write_func);
}

}  // namespace rfl::avro

#endif
