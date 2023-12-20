#ifndef RFL_XML_LOAD_HPP_
#define RFL_XML_LOAD_HPP_

#include "rfl/Result.hpp"
#include "rfl/io/load_string.hpp"
#include "rfl/xml/read.hpp"

namespace rfl {
namespace xml {

template <class T>
Result<T> load(const std::string& _fname) {
  const auto read_string = [](const auto& _str) { return read<T>(_str); };
  return rfl::io::load_string(_fname).and_then(read_string);
}

}  // namespace xml
}  // namespace rfl

#endif
