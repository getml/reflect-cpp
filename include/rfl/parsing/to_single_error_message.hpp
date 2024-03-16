#ifndef RFL_PARSING_TOSINGLEERRORMESSAGE_HPP_
#define RFL_PARSING_TOSINGLEERRORMESSAGE_HPP_

#include <string>
#include <vector>

#include "../internal/strings/replace_all.hpp"

namespace rfl::parsing {

/// Combines a set of errors to a single, readable error message.
inline Error to_single_error_message(std::vector<Error> _errors) {
  if (_errors.size() == 1) {
    return std::move(_errors[0]);
  } else {
    std::string msg = "Found " + std::to_string(_errors.size()) + " errors:";
    for (size_t i = 0; i < _errors.size() && i < 10; ++i) {
      msg +=
          "\n" + std::to_string(i + 1) + ") " +
          internal::strings::replace_all(_errors.at(i).what(), "\n", "\n    ");
    }
    if (_errors.size() > 10) {
      msg +=
          "\n...\nMore than 10 errors occurred, but I am only showing the "
          "first 10.";
    }
    return Error(msg);
  }
}

}  // namespace rfl::parsing

#endif
