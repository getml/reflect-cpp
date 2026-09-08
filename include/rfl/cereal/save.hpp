#ifndef RFL_CEREAL_SAVE_HPP_
#define RFL_CEREAL_SAVE_HPP_

#include <string>

#include "../Result.hpp"
#include "../io/save_bytes.hpp"
#include "write.hpp"

namespace rfl {
namespace cereal {

/// Saves an object to a file using Cereal portable binary format.
/// Serializes the object and writes it to the specified file path.
/// @tparam Ps Optional processors to apply during serialization
/// @param _fname The path to the file to save to
/// @param _obj The object to serialize
/// @return A Result containing Nothing on success or an error
template <class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj) {
  const auto write_func = [](const auto& _obj, auto& _stream) -> auto& {
    return write<Ps...>(_obj, _stream);
  };
  return rfl::io::save_bytes(_fname, _obj, write_func);
}

}  // namespace cereal
}  // namespace rfl

#endif
