#ifndef RFL_JSON_LOAD_HPP_
#define RFL_JSON_LOAD_HPP_

#include "../Result.hpp"
#include "../io/load_string.hpp"
#include "read.hpp"

namespace rfl {
namespace json {

template <class T, class... Ps>
Result<T> load(const std::string& _fname) {
  const auto read_string = [](const auto& _str) {
    return read<T, Ps...>(_str);
  };
  return rfl::io::load_string(_fname).and_then(read_string);
}

}  // namespace json
}  // namespace rfl

#endif
