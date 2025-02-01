#ifndef RFL_IO_LOAD_BYTES_HPP_
#define RFL_IO_LOAD_BYTES_HPP_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../Result.hpp"

namespace rfl {
namespace io {

inline Result<std::vector<char>> load_bytes(const std::string& _fname) {
  std::ifstream input(_fname, std::ios::binary);
  if (input.is_open()) {
    std::istreambuf_iterator<char> begin(input), end;
    const auto bytes = std::vector<char>(begin, end);
    input.close();
    return bytes;
  } else {
    return error("File '" + _fname + "' not found!");
  }
}

}  // namespace io
}  // namespace rfl

#endif
