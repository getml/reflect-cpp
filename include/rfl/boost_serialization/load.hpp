#ifndef RFL_BOOST_SERIALIZATION_LOAD_HPP_
#define RFL_BOOST_SERIALIZATION_LOAD_HPP_

#include "../Result.hpp"
#include "../io/load_bytes.hpp"
#include "read.hpp"

namespace rfl {
namespace boost_serialization {

template <class T, class... Ps>
Result<T> load(const std::string& _fname) {
  const auto read_bytes = [](const auto& _bytes) {
    return read<T, Ps...>(_bytes);
  };
  return rfl::io::load_bytes(_fname).and_then(read_bytes);
}

}  // namespace boost_serialization
}  // namespace rfl

#endif
