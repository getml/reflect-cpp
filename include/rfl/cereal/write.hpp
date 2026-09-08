#ifndef RFL_CEREAL_WRITE_HPP_
#define RFL_CEREAL_WRITE_HPP_

#include <ostream>
#include <sstream>

#include "../Processors.hpp"
#include "../Result.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"
#include "Writer.hpp"

namespace rfl::cereal {

/// Writes an object directly to a Cereal output archive.
/// This allows integration with existing Cereal-based code.
/// @tparam T The type of object to serialize
/// @tparam Ps Optional processors to apply during serialization
/// @param _obj The object to serialize
/// @param _archive The Cereal output archive to write to
template <class T, class... Ps>
void write(const T& _obj, Writer::CerealArchive& _archive) {
  using ParentType = parsing::Parent<Writer>;
  auto w = Writer(&_archive);
  Parser<T, Processors<Ps...>>::write(w, _obj, typename ParentType::Root{});
}

/// Serializes an object to bytes using Cereal portable binary format.
/// @tparam Ps Optional processors to apply during serialization
/// @param _obj The object to serialize
/// @return A vector of bytes containing the serialized data
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  std::stringstream ss;
  {
    ::cereal::PortableBinaryOutputArchive archive(ss);
    write<std::remove_cvref_t<decltype(_obj)>, Ps...>(_obj, archive);
  }
  auto str = ss.str();
  return std::vector<char>(str.begin(), str.end());
}

/// Writes an object to an output stream using Cereal portable binary format.
/// @tparam Ps Optional processors to apply during serialization
/// @param _obj The object to serialize
/// @param _stream The output stream to write to
/// @return The output stream (for chaining)
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  ::cereal::PortableBinaryOutputArchive archive(_stream);
  write<std::remove_cvref_t<decltype(_obj)>, Ps...>(_obj, archive);
  return _stream;
}

}  // namespace rfl::cereal

#endif
