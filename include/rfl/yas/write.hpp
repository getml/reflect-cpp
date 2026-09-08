#ifndef RFL_YAS_WRITE_HPP_
#define RFL_YAS_WRITE_HPP_

#include <string>
#include <vector>
#include <yas/binary_iarchive.hpp>
#include <yas/binary_oarchive.hpp>
#include <yas/mem_streams.hpp>
#include <yas/std_types.hpp>

#include "../Processors.hpp"
#include "../Result.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"
#include "Writer.hpp"

namespace rfl::yas {

using OArchive = Writer::OArchive;

/// Writes an object directly to an existing YAS binary output archive.
/// YAS (Yet Another Serialization) is a fast binary serialization library.
/// This function allows you to write to an already-opened YAS archive, useful when working
/// with YAS archives directly or writing multiple objects to the same archive.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _archive The YAS binary output archive to write to
/// @param _obj The object to serialize to the archive
template <class... Ps>
void write_to_archive(OArchive& _archive, const auto& _obj) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  auto w = Writer(&_archive);
  Parser<T, Processors<Ps...>>::write(w, _obj, typename ParentType::Root{});
}

/// Returns YAS binary bytes representation of the object.
/// YAS (Yet Another Serialization) is a high-performance binary serialization library
/// designed for speed and efficiency. Uses compile-time reflection to serialize to YAS format.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to YAS binary format
/// @return A vector of chars containing the YAS binary representation
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  ::yas::mem_ostream os;
  ::yas::binary_oarchive<::yas::mem_ostream> ar(os);
  write_to_archive<Ps...>(ar, _obj);
  auto buf = os.get_intrusive_buffer();
  return std::vector<char>(buf.data, buf.data + buf.size);
}

/// Writes a YAS binary representation into an ostream.
/// Uses compile-time reflection to serialize a C++ object to YAS binary format and write to a stream.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to YAS binary format
/// @param _stream The output stream to write YAS binary data to
/// @return The output stream (for chaining)
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  ::yas::mem_ostream os;
  ::yas::binary_oarchive<::yas::mem_ostream> ar(os);
  write_to_archive<Ps...>(ar, _obj);
  auto buf = os.get_intrusive_buffer();
  _stream.write(buf.data, buf.size);
  return _stream;
}

}  // namespace rfl::yas

#endif
