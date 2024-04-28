#ifndef RFL_TOML_LOAD_HPP_
#define RFL_TOML_LOAD_HPP_

#include "../Result.hpp"
#include "../io/load_string.hpp"
#include "read.hpp"

namespace rfl::toml {

template <class T>
Result<T> load(const std::string& _fname) {
  const auto read_string = [](const auto& _str) { return read<T>(_str); };
  return rfl::io::load_string(_fname).and_then(read_string);
}

}  // namespace rfl::toml

#endif
