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

/// Writes an object to a Cereal OutputArchive.
template <class T, class... Ps>
void write(const T& _obj, Writer::CerealArchive& _archive) {
  using ParentType = parsing::Parent<Writer>;
  auto w = Writer(&_archive);
  Parser<T, Processors<Ps...>>::write(w, _obj, typename ParentType::Root{});
}

/// Returns Cereal binary bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  std::stringstream ss;
  {
    ::cereal::PortableBinaryOutputArchive archive(ss);
    write<std::remove_cvref_t<decltype(_obj)>, decltype(archive), Ps...>(
        _obj, archive);
  }
  auto str = ss.str();
  return std::vector<char>(str.begin(), str.end());
}

/// Writes Cereal binary format into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  ::cereal::BinaryOutputArchive archive(_stream);
  write<std::remove_cvref_t<decltype(_obj)>, decltype(archive), Ps...>(_obj,
                                                                       archive);
  return _stream;
}

}  // namespace rfl::cereal

#endif
