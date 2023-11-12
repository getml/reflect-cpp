#ifndef RFL_IO_SAVE_BYTES_HPP_
#define RFL_IO_SAVE_BYTES_HPP_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "rfl/Result.hpp"

namespace rfl {
namespace io {

Result<Nothing> save_bytes(const std::string& _fname,
                           const std::vector<unsigned char>& _bytes) {
  try {
    std::ofstream output(_fname, std::ios::out | std::ios::binary);
    output.write(reinterpret_cast<const char*>(_bytes.data()), _bytes.size());
    output.close();
  } catch (std::exception& e) {
    return Error(e.what());
  }
  return Nothing{};
}

}  // namespace io
}  // namespace rfl

#endif
