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

/// Writes into an existing Boost output archive.
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

/// Returns serialized bytes using a binary archive.
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  std::ostringstream stream;
  {
    boost::archive::binary_oarchive ar(stream);
    write<boost::archive::binary_iarchive,
          boost::archive::binary_oarchive, Ps...>(ar, _obj);
  }
  const auto str = stream.str();
  return std::vector<char>(str.begin(), str.end());
}

/// Writes into an ostream using a binary archive.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  boost::archive::binary_oarchive ar(_stream);
  write<boost::archive::binary_iarchive,
        boost::archive::binary_oarchive, Ps...>(ar, _obj);
  return _stream;
}

}  // namespace rfl::boost_serialization

#endif
