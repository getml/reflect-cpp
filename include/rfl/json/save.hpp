#ifndef RFL_JSON_SAVE_HPP_
#define RFL_JSON_SAVE_HPP_

#include <yyjson.h>

#include <fstream>
#include <iostream>
#include <string>

#include "../Result.hpp"
#include "../io/save_string.hpp"
#include "write.hpp"

namespace rfl {
namespace json {

template <class T>
Result<Nothing> save(const std::string& _fname, const T& _obj,
                     const yyjson_write_flag _flag = 0) {
  const auto write_func = [_flag](const auto& _obj, auto& _stream) -> auto& {
    return write(_obj, _stream, _flag);
  };
  return rfl::io::save_string(_fname, _obj, write_func);
}

}  // namespace json
}  // namespace rfl

#endif
