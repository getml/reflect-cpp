#ifndef RFL_FLEXBUF_SAVE_HPP_
#define RFL_FLEXBUF_SAVE_HPP_

#include <string>

#include "../Result.hpp"
#include "../io/save_bytes.hpp"
#include "write.hpp"

namespace rfl {
namespace flexbuf {

/// Saves an object to a FlexBuffers file.
/// Serializes a C++ object to FlexBuffers binary format and writes it to a file using compile-time reflection.
/// FlexBuffers is a schema-less binary format from Google's FlatBuffers project.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _fname The filename/path where the FlexBuffers file will be saved
/// @param _obj The object to serialize to FlexBuffers
/// @return Result containing Nothing on success or an error message on failure
template <class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj) {
  const auto write_func = [](const auto& _obj, auto& _stream) -> auto& {
    return write<Ps...>(_obj, _stream);
  };
  return rfl::io::save_bytes(_fname, _obj, write_func);
}

}  // namespace flexbuf
}  // namespace rfl

#endif
