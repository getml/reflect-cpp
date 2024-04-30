#ifndef RFL_BSON_LOAD_HPP_
#define RFL_BSON_LOAD_HPP_

#include "../Result.hpp"
#include "../io/load_bytes.hpp"
#include "read.hpp"

namespace rfl {
namespace bson {

template <class T, class... Ps>
Result<T> load(const std::string& _fname) {
  const auto read_bytes = [](const auto& _bytes) {
    return read<T, Ps...>(_bytes);
  };
  return rfl::io::load_bytes(_fname).and_then(read_bytes);
}

}  // namespace bson
}  // namespace rfl

#endif
