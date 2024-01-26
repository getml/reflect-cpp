#ifndef RFL_CBOR_SAVE_HPP_
#define RFL_CBOR_SAVE_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "../Result.hpp"
#include "../io/save_bytes.hpp"
#include "write.hpp"

namespace rfl {
namespace cbor {

template <class T>
Result<Nothing> save(const std::string& _fname, const T& _obj) {
  const auto write_func = [](const auto& _obj, auto& _stream) -> auto& {
    return write(_obj, _stream);
  };
  return rfl::io::save_bytes(_fname, _obj, write_func);
}

}  // namespace cbor
}  // namespace rfl

#endif
