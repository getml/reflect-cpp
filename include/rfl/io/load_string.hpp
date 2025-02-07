#ifndef RFL_IO_LOAD_STRING_HPP_
#define RFL_IO_LOAD_STRING_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "../Result.hpp"

namespace rfl {
namespace io {

inline Result<std::string> load_string(const std::string& _fname) {
  std::ifstream infile(_fname);
  if (infile.is_open()) {
    auto r = std::string(std::istreambuf_iterator<char>(infile),
                         std::istreambuf_iterator<char>());
    infile.close();
    return r;
  } else {
    return error("Unable to open file '" + _fname +
                 "' or file could not be found.");
  }
}

}  // namespace io
}  // namespace rfl

#endif
