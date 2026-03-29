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

/// Parses an object from a Cereal InputArchive.
template <class T, class... Ps>
auto read(Reader::CerealArchive& _archive) {
  const auto r = Reader();
  auto var = InputVarType{&_archive};
  return Parser<T, Processors<Ps...>>::read(r, var);
}

/// Parses an object from Cereal binary format using reflection.
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

/// Parses an object from Cereal binary format using reflection.
template <class T, class... Ps>
auto read(const concepts::ContiguousByteContainer auto& _bytes) {
  return read<T, Ps...>(_bytes.data(), _bytes.size());
}

/// Parses an object from a stream using Cereal binary format.
template <class T, class... Ps>
auto read(std::istream& _stream) {
  try {
    ::cereal::BinaryInputArchive archive(_stream);
    return read<T, Ps...>(archive);
  } catch (std::exception& e) {
    return Result<internal::wrap_in_rfl_array_t<T>>(
        error(std::string("Cereal read error: ") + e.what()));
  }
}

}  // namespace rfl::cereal

#endif
