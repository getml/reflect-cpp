#ifndef RFL_CEREAL_READ_HPP_
#define RFL_CEREAL_READ_HPP_

#include <cereal/archives/portable_binary.hpp>
#include <cereal/cereal.hpp>
#include <istream>
#include <sstream>

#include "../Processors.hpp"
#include "../concepts.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::cereal {

using InputVarType = Reader::InputVarType;

/// Parses an object from a Cereal input archive.
/// This allows direct integration with existing Cereal-based code.
/// @tparam T The type to deserialize into
/// @tparam Ps Optional processors to apply during deserialization
/// @param _archive The Cereal input archive to read from
/// @return A Result containing the deserialized object or an error
template <class T, class... Ps>
auto read(Reader::CerealArchive& _archive) {
  const auto r = Reader();
  auto var = InputVarType{&_archive};
  return Parser<T, Processors<Ps...>>::read(r, var);
}

/// Parses an object from raw bytes using Cereal portable binary format.
/// Creates a memory stream and deserializes using Cereal.
/// @tparam T The type to deserialize into
/// @tparam Ps Optional processors to apply during deserialization
/// @param _bytes Pointer to the binary data
/// @param _size Size of the binary data in bytes
/// @return A Result containing the deserialized object or an error
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const concepts::ByteLike auto* _bytes, const size_t _size) {
  try {
    std::stringstream ss(
        std::string(reinterpret_cast<const char*>(_bytes), _size));
    ::cereal::PortableBinaryInputArchive archive(ss);
    return read<T, Ps...>(archive);
  } catch (std::exception& e) {
    return error(std::string("Cereal read error: ") + e.what());
  }
}

/// Parses an object from a byte container using Cereal portable binary format.
/// @tparam T The type to deserialize into
/// @tparam Ps Optional processors to apply during deserialization
/// @param _bytes A contiguous container containing the Cereal binary data
/// @return A Result containing the deserialized object or an error
template <class T, class... Ps>
auto read(const concepts::ContiguousByteContainer auto& _bytes) {
  return read<T, Ps...>(_bytes.data(), _bytes.size());
}

/// Parses an object from an input stream using Cereal portable binary format.
/// @tparam T The type to deserialize into
/// @tparam Ps Optional processors to apply during deserialization
/// @param _stream The input stream to read from
/// @return A Result containing the deserialized object or an error
template <class T, class... Ps>
auto read(std::istream& _stream) {
  try {
    ::cereal::PortableBinaryInputArchive archive(_stream);
    return read<T, Ps...>(archive);
  } catch (std::exception& e) {
    return Result<internal::wrap_in_rfl_array_t<T>>(
        error(std::string("Cereal read error: ") + e.what()));
  }
}

}  // namespace rfl::cereal

#endif
