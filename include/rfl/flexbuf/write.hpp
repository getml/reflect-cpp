#ifndef FLEXBUF_WRITE_HPP_
#define FLEXBUF_WRITE_HPP_

#include <flatbuffers/flexbuffers.h>

#include <cstddef>
#include <ostream>
#include <sstream>
#include <vector>

#include "../Ref.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace flexbuf {

template <class T>
std::vector<uint8_t> to_buffer(const T& _obj) {
  using ParentType = parsing::Parent<Writer>;
  const auto fbb = Ref<flexbuffers::Builder>::make();
  auto w = Writer(fbb);
  Parser<T>::write(w, _obj, typename ParentType::Root{});
  fbb->Finish();
  return fbb->GetBuffer();
}

/// Writes an object to flexbuf.
template <class T>
std::vector<char> write(const T& _obj) {
  const auto buffer = to_buffer(_obj);
  const auto data = reinterpret_cast<const char*>(buffer.data());
  return std::vector<char>(data, data + buffer.size());
}

/// Writes an object to an ostream.
template <class T>
std::ostream& write(const T& _obj, std::ostream& _stream) {
  const auto buffer = to_buffer(_obj);
  const auto data = reinterpret_cast<const char*>(buffer.data());
  _stream.write(data, buffer.size());
  return _stream;
}

}  // namespace flexbuf
}  // namespace rfl

#endif
