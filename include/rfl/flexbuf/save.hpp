#ifndef RFL_FLEXBUF_SAVE_HPP_
#define RFL_FLEXBUF_SAVE_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "rfl/Result.hpp"
#include "rfl/flexbuf/write.hpp"
#include "rfl/io/save_bytes.hpp"

namespace rfl {
namespace flexbuf {

template <class T>
Result<Nothing> save(const std::string& _fname, const T& _obj) {
  return rfl::io::save_bytes(_fname, write(_obj));
}

}  // namespace flexbuf
}  // namespace rfl

#endif
