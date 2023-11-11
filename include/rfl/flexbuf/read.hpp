#ifndef FLEXBUF_READ_HPP_
#define FLEXBUF_READ_HPP_

#include <flatbuffers/flexbuffers.h>

#include <vector>

#include "rfl/Result.hpp"
#include "rfl/flexbuf/Parser.hpp"

namespace rfl {
namespace flexbuf {

using InputVarType = typename Reader::InputVarType;

/// Parses an object from flexbuf using reflection.
template <class T>
Result<T> read(const unsigned char* _bytes, const size_t _size) {
  const InputVarType root =
      flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(_bytes), _size);
  const auto r = Reader();
  return Parser<T>::read(r, root);
}

/// Parses an object from flexbuf using reflection.
template <class T>
Result<T> read(const std::vector<unsigned char>& _bytes) {
  return read<T>(_bytes.data(), _bytes.size());
}

}  // namespace flexbuf
}  // namespace rfl

#endif
