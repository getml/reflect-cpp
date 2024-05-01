#ifndef RFL_YAML_SAVE_HPP_
#define RFL_YAML_SAVE_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "../Processors.hpp"
#include "../Result.hpp"
#include "../io/save_string.hpp"
#include "write.hpp"

namespace rfl {
namespace yaml {

template <class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj) {
  const auto write_func = [](const auto& _obj, auto& _stream) -> auto& {
    return write<Ps...>(_obj, _stream);
  };
  return rfl::io::save_string(_fname, _obj, write_func);
}

}  // namespace yaml
}  // namespace rfl

#endif
