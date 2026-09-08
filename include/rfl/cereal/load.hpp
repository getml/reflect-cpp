#ifndef RFL_CEREAL_LOAD_HPP_
#define RFL_CEREAL_LOAD_HPP_

#include "../Result.hpp"
#include "../io/load_bytes.hpp"
#include "read.hpp"

namespace rfl {
namespace cereal {

/// Loads and parses an object from a file using Cereal serialization.
/// Reads the file as binary data and deserializes it using Cereal's portable binary archive.
/// @tparam T The type to deserialize into
/// @tparam Ps Optional processors to apply during deserialization
/// @param _fname The path to the file to load
/// @return A Result containing the deserialized object or an error
template <class T, class... Ps>
Result<T> load(const std::string& _fname) {
  const auto read_bytes = [](const auto& _bytes) {
    return read<T, Ps...>(_bytes);
  };
  return rfl::io::load_bytes(_fname).and_then(read_bytes);
}

}  // namespace cereal
}  // namespace rfl

#endif
