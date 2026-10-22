#ifndef RFL_IO_LOAD_STRING_HPP_
#define RFL_IO_LOAD_STRING_HPP_

#include <string>

#include "../Result.hpp"
#include "load_file.hpp"

namespace rfl {
namespace io {

inline Result<std::string> load_string(const std::string& _fname) {
  return internal::load_file<std::string>(_fname, std::ios::in);
}

}  // namespace io
}  // namespace rfl

#endif
