#ifndef RFL_JSON_LOAD_HPP_
#define RFL_JSON_LOAD_HPP_

#include "../Result.hpp"
#include "../io/load_string.hpp"
#include "read.hpp"

namespace rfl {
namespace json {

template <class T, class... Ps>
Result<T> load(const std::string& _fname, const yyjson_read_flag _flag = 0) {
  const auto read_string = [_flag](const auto& _str) {
    return read<T, Ps...>(_str, _flag);
  };
  return rfl::io::load_string(_fname).and_then(read_string);
}

}  // namespace json
}  // namespace rfl

#endif
