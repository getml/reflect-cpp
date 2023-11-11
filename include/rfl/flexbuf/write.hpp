#ifndef FLEXBUF_WRITE_HPP_
#define FLEXBUF_WRITE_HPP_

#include <flatbuffers/flexbuffers.h>

#include <cstddef>
#include <sstream>
#include <vector>

#include "rfl/flexbuf/Parser.hpp"

namespace rfl {
namespace flexbuf {

/// Writes an object to flexbuf.
template <class T>
std::vector<unsigned char> write(const T& _obj) {
  auto w = Writer();
  const auto flexbuf_obj = Parser<T>::write(w, _obj);
  flexbuffers::Builder fbb;
  flexbuf_obj->insert(std::nullopt, &fbb);
  fbb.Finish();
  const auto vec = fbb.GetBuffer();
  const auto data = reinterpret_cast<const unsigned char*>(vec.data());
  return std::vector<unsigned char>(data, data + vec.size());
}

}  // namespace flexbuf
}  // namespace rfl

#endif
