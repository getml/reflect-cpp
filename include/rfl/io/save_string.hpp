#ifndef RFL_IO_SAVE_STRING_HPP_
#define RFL_IO_SAVE_STRING_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "../Result.hpp"

namespace rfl {
namespace io {

template <class T, class WriteFunction>
Result<Nothing> save_string(const std::string& _fname, const T& _obj,
                            const WriteFunction& _write) {
  try {
    std::ofstream outfile;
    outfile.open(_fname);
    _write(_obj, outfile);
    outfile.close();
  } catch (std::exception& e) {
    return error(e.what());
  }
  return Nothing{};
}

}  // namespace io
}  // namespace rfl

#endif
