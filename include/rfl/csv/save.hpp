#ifndef RFL_CSV_SAVE_HPP_
#define RFL_CSV_SAVE_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "../Result.hpp"
#include "../io/save_string.hpp"
#include "Settings.hpp"
#include "write.hpp"

namespace rfl::csv {

template <class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj,
                     const Settings& _settings = Settings{}) {
  const auto write_func = [&](const auto& _obj, auto& _stream) -> auto& {
    return write<Ps...>(_obj, _stream, _settings);
  };
  return rfl::io::save_string(_fname, _obj, write_func);
}

}  // namespace rfl::csv

#endif
