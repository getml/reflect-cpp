#ifndef RFL_CSV_CSV_HPP_
#define RFL_CSV_CSV_HPP_

#include "../Result.hpp"
#include "../io/load_string.hpp"
#include "Settings.hpp"
#include "read.hpp"

namespace rfl::csv {

template <class T, class... Ps>
Result<T> load(const std::string& _fname,
               const Settings& _settings = Settings{}) {
  const auto read_string = [&](const auto& _str) {
    return read<T, Ps...>(_str, _settings);
  };
  return rfl::io::load_string(_fname).and_then(read_string);
}

}  // namespace rfl::csv

#endif
