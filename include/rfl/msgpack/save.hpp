#ifndef RFL_MSGPACK_SAVE_HPP_
#define RFL_MSGPACK_SAVE_HPP_

#include <string>

#include "../Result.hpp"
#include "../io/save_bytes.hpp"
#include "write.hpp"

namespace rfl {
namespace msgpack {

/// Saves an object to a MSGPACK file.
/// Serializes a C++ object to MSGPACK binary format and writes it to a file using compile-time reflection.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _fname The filename/path where the MSGPACK file will be saved
/// @param _obj The object to serialize to MSGPACK
/// @return Result containing Nothing on success or an error message on failure
template <class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj) {
  const auto write_func = [](const auto& _obj, auto& _stream) -> auto& {
    return write<Ps...>(_obj, _stream);
  };
  return rfl::io::save_bytes(_fname, _obj, write_func);
}

}  // namespace msgpack
}  // namespace rfl

#endif
