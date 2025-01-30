#ifndef FLEXBUF_WRITE_HPP_
#define FLEXBUF_WRITE_HPP_

#include <flatbuffers/flexbuffers.h>

#include <cstddef>
#include <ostream>
#include <sstream>
#include <vector>

#include "../Processors.hpp"
#include "../Ref.hpp"
#include "../internal/ptr_cast.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace flexbuf {

template <class... Ps>
std::vector<uint8_t> to_buffer(const auto& _obj) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  const auto fbb = Ref<flexbuffers::Builder>::make();
  auto w = Writer(fbb);
  Parser<T, Processors<Ps...>>::write(w, _obj, typename ParentType::Root{});
  fbb->Finish();
  return fbb->GetBuffer();
}

/// Writes an object to flexbuf.
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  const auto buffer = to_buffer<Ps...>(_obj);
  const auto data = internal::ptr_cast<const char*>(buffer.data());
  return std::vector<char>(data, data + buffer.size());
}

/// Writes an object to an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  const auto buffer = to_buffer<Ps...>(_obj);
  const auto data = internal::ptr_cast<const char*>(buffer.data());
  _stream.write(data, buffer.size());
  return _stream;
}

}  // namespace flexbuf
}  // namespace rfl

#endif
