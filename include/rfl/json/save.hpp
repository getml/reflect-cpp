#ifndef RFL_JSON_SAVE_HPP_
#define RFL_JSON_SAVE_HPP_

#if __has_include(<yyjson.h>)
#include <yyjson.h>
#else
#include "../thirdparty/yyjson.h"
#endif

#include <fstream>
#include <iostream>
#include <string>

#include "../Result.hpp"
#include "../io/save_string.hpp"
#include "write.hpp"

namespace rfl {
namespace json {

template <class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj,
                     const yyjson_write_flag _flag = 0) {
  const auto write_func = [_flag](const auto& _obj, auto& _stream) -> auto& {
    return write<Ps...>(_obj, _stream, _flag);
  };
  return rfl::io::save_string(_fname, _obj, write_func);
}

}  // namespace json
}  // namespace rfl

#endif
