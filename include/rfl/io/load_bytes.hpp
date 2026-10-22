#ifndef RFL_IO_LOAD_BYTES_HPP_
#define RFL_IO_LOAD_BYTES_HPP_

#include <string>
#include <vector>

#include "../Result.hpp"
#include "load_file.hpp"

namespace rfl {
namespace io {

inline Result<std::vector<char>> load_bytes(const std::string& _fname) {
  return internal::load_file<std::vector<char>>(_fname, std::ios::binary);
}

}  // namespace io
}  // namespace rfl

#endif
