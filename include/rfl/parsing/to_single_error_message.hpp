#ifndef RFL_PARSING_TOSINGLEERRORMESSAGE_HPP_
#define RFL_PARSING_TOSINGLEERRORMESSAGE_HPP_

#include <optional>
#include <string>
#include <vector>

#include "../internal/strings/replace_all.hpp"

namespace rfl::parsing {

/// Combines a set of errors to a single, readable error message.
inline Error to_single_error_message(
    std::vector<Error> _errors,
    std::optional<std::string> _msg_prefix = std::nullopt,
    size_t _err_limit = 10) {
  if (_errors.size() == 1) {
    return std::move(_errors[0]);
  } else {
    std::string msg =
        _msg_prefix ? *_msg_prefix
                    : "Found " + std::to_string(_errors.size()) + " errors:";
    for (size_t i = 0; i < _errors.size() && i < _err_limit; ++i) {
      msg +=
          "\n" + std::to_string(i + 1) + ") " +
          internal::strings::replace_all(_errors.at(i).what(), "\n", "\n    ");
    }
    if (_errors.size() > _err_limit) {
      msg += "\n...\nMore than " + std::to_string(_err_limit) +
             " errors occurred, but I am only showing the "
             "first " +
             std::to_string(_err_limit) + ".";
    }
    return Error(msg);
  }
}

}  // namespace rfl::parsing

#endif
