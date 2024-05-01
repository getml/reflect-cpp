#ifndef RFL_XML_SAVE_HPP_
#define RFL_XML_SAVE_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "../Result.hpp"
#include "../internal/StringLiteral.hpp"
#include "../io/save_string.hpp"
#include "write.hpp"

namespace rfl {
namespace xml {

template <internal::StringLiteral _root = internal::StringLiteral(""),
          class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj) {
  const auto write_func = [](const auto& _obj, auto& _stream) -> auto& {
    return write<_root, Ps...>(_obj, _stream);
  };
  return rfl::io::save_string(_fname, _obj, write_func);
}

}  // namespace xml
}  // namespace rfl

#endif
