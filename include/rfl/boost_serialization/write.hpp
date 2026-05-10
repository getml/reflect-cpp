#ifndef RFL_BOOST_SERIALIZATION_WRITE_HPP_
#define RFL_BOOST_SERIALIZATION_WRITE_HPP_

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "../Processors.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"
#include "Writer.hpp"

namespace rfl::boost_serialization {

/// Writes an object directly into an existing Boost output archive.
/// This allows you to use custom archive types and integrate with existing Boost.Serialization code.
/// @tparam IArchive The corresponding Boost input archive type (for type info)
/// @tparam OArchive The Boost output archive type
/// @tparam Ps Optional processors to apply during serialization
/// @param _ar The Boost output archive to write to
/// @param _obj The object to serialize
template <class IArchive = boost::archive::binary_iarchive, class OArchive,
          class... Ps>
void write(OArchive& _ar, const auto& _obj) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using W = Writer<OArchive>;
  using ParentType = parsing::Parent<W>;
  auto w = W(&_ar);
  Parser<IArchive, OArchive, T, Processors<Ps...>>::write(
      w, _obj, typename ParentType::Root{});
}

/// Serializes an object to bytes using Boost binary archive format.
/// @tparam Ps Optional processors to apply during serialization
/// @param _obj The object to serialize
/// @return A vector of bytes containing the serialized data
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  std::ostringstream stream;
  {
    boost::archive::binary_oarchive ar(
        stream, boost::archive::no_header | boost::archive::no_tracking);
    write<boost::archive::binary_iarchive, boost::archive::binary_oarchive,
          Ps...>(ar, _obj);
  }
  const auto str = stream.str();
  return std::vector<char>(str.begin(), str.end());
}

/// Writes an object to an output stream using Boost binary archive format.
/// @tparam Ps Optional processors to apply during serialization
/// @param _obj The object to serialize
/// @param _stream The output stream to write to
/// @return The output stream (for chaining)
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  boost::archive::binary_oarchive ar(
      _stream, boost::archive::no_header | boost::archive::no_tracking);
  write<boost::archive::binary_iarchive, boost::archive::binary_oarchive,
        Ps...>(ar, _obj);
  return _stream;
}

}  // namespace rfl::boost_serialization

#endif
