#ifndef FLEXBUF_READ_HPP_
#define FLEXBUF_READ_HPP_

#include <flatbuffers/flexbuffers.h>

#include <istream>
#include <vector>

#include "../Result.hpp"
#include "Parser.hpp"

namespace rfl {
namespace flexbuf {

using InputVarType = typename Reader::InputVarType;

/// Parses an object from flexbuf var.
template <class T>
auto read(const InputVarType& _obj) {
  const auto r = Reader();
  return Parser<T>::read(r, _obj);
}

/// Parses an object from flexbuf using reflection.
template <class T>
auto read(const char* _bytes, const size_t _size) {
  const InputVarType root =
      flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(_bytes), _size);
  return read<T>(root);
}

/// Parses an object from flexbuf using reflection.
template <class T>
auto read(const std::vector<char>& _bytes) {
  return read<T>(_bytes.data(), _bytes.size());
}

/// Parses an object directly from a stream.
template <class T>
auto read(std::istream& _stream) {
  std::istreambuf_iterator<char> begin(_stream), end;
  const auto bytes = std::vector<char>(begin, end);
  return read<T>(bytes.data(), bytes.size());
}

}  // namespace flexbuf
}  // namespace rfl

#endif
