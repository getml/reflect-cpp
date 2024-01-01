#ifndef RFL_YAML_LOAD_HPP_
#define RFL_YAML_LOAD_HPP_

#include "../Result.hpp"
#include "../io/load_string.hpp"
#include "read.hpp"

namespace rfl {
namespace yaml {

template <class T>
Result<T> load(const std::string& _fname) {
  const auto read_string = [](const auto& _str) { return read<T>(_str); };
  return rfl::io::load_string(_fname).and_then(read_string);
}

}  // namespace yaml
}  // namespace rfl

#endif
