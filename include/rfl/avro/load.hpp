#ifndef RFL_AVRO_LOAD_HPP_
#define RFL_AVRO_LOAD_HPP_

//#include "../Processors.hpp"
#include "../Result.hpp"
#include "../io/load_bytes.hpp"
#include "read.hpp"

namespace rfl {
namespace avro {

/// Loads and parses an Avro object from a file.
/// Reads the file as binary data and deserializes it using Avro.
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

}  // namespace avro
}  // namespace rfl

#endif
