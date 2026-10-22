#ifndef RFL_IO_SAVE_BYTES_HPP_
#define RFL_IO_SAVE_BYTES_HPP_

#include <fstream>
#include <string>

#include "../Result.hpp"

namespace rfl {
namespace io {

template <class T, class WriteFunction>
Result<Nothing> save_bytes(const std::string& _fname, const T& _obj,
                           const WriteFunction& _write) {
  try {
    std::ofstream output(_fname, std::ios::out | std::ios::binary);
    if (!output.is_open()) {
      return error("Unable to open file '" + _fname + "' for writing.");
    }
    _write(_obj, output);
    output.close();
    if (!output) {
      return error("Unable to write or close file '" + _fname + "'.");
    }
  } catch (const std::exception& e) {
    return error(e.what());
  }
  return Nothing{};
}

}  // namespace io
}  // namespace rfl

#endif
