#ifndef RFL_IO_LOAD_BYTES_HPP_
#define RFL_IO_LOAD_BYTES_HPP_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "rfl/Result.hpp"

namespace rfl {
namespace io {

Result<std::vector<unsigned char>> load_bytes(const std::string& _fname) {
  std::ifstream input(_fname, std::ios::binary);
  if (input.is_open()) {
    std::istreambuf_iterator<char> begin(input), end;
    const auto bytes = std::vector<unsigned char>(begin, end);
    input.close();
    return bytes;
  } else {
    return rfl::Error("File '" + _fname + "' not found!");
  }
}

}  // namespace io
}  // namespace rfl

#endif
