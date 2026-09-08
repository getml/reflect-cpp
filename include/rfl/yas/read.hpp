#ifndef RFL_YAS_READ_HPP_
#define RFL_YAS_READ_HPP_

#include <istream>
#include <string>
#include <yas/binary_iarchive.hpp>
#include <yas/binary_oarchive.hpp>
#include <yas/mem_streams.hpp>
#include <yas/std_types.hpp>

#include "../Processors.hpp"
#include "../concepts.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::yas {

using IArchive = Reader::IArchive;

/// Reads an object directly from an existing YAS binary input archive.
/// YAS (Yet Another Serialization) is a fast binary serialization library.
/// This function allows you to read from an already-opened YAS archive, useful when working
/// with YAS archives directly or reading multiple objects from the same archive.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _ar The YAS binary input archive to read from
/// @return The parsed object of type T
template <class T, class... Ps>
auto read_from_archive(IArchive& _ar) {
  auto r = Reader();
  auto var = Reader::InputVarType{&_ar};
  return Parser<T, Processors<Ps...>>::read(r, var);
}

/// Parses an object from YAS binary bytes using reflection (with raw pointer and size).
/// YAS (Yet Another Serialization) is a high-performance binary serialization library
/// designed for speed and efficiency. It provides compact binary encoding.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _bytes Pointer to byte-like data containing YAS binary format
/// @param _size The size of the byte array
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const concepts::ByteLike auto* _bytes, const size_t _size) {
  try {
    ::yas::mem_istream is(_bytes, _size);
    ::yas::binary_iarchive<::yas::mem_istream> ar(is);
    return read_from_archive<T, Ps...>(ar);
  } catch (std::exception& e) {
    return error(std::string("yas read error: ") + e.what());
  }
}

/// Parses an object from YAS binary format using reflection (contiguous container version).
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _bytes A contiguous byte container (e.g., std::vector<char>, std::string) containing YAS binary data
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
auto read(const concepts::ContiguousByteContainer auto& _bytes) {
  return read<T, Ps...>(_bytes.data(), _bytes.size());
}

}  // namespace rfl::yas

#endif
