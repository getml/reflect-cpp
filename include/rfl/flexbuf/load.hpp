#ifndef RFL_FLEXBUF_LOAD_HPP_
#define RFL_FLEXBUF_LOAD_HPP_

#include "rfl/Result.hpp"
#include "rfl/flexbuf/read.hpp"
#include "rfl/io/load_bytes.hpp"

namespace rfl {
namespace flexbuf {

template <class T>
Result<T> load(const std::string& _fname) {
  const auto read_bytes = [](const auto& _bytes) { return read<T>(_bytes); };
  return rfl::io::load_bytes(_fname).and_then(read_bytes);
}

}  // namespace flexbuf
}  // namespace rfl

#endif
