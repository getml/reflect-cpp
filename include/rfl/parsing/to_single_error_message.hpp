#ifndef RFL_PARSING_TOSINGLEERRORMESSAGE_HPP_
#define RFL_PARSING_TOSINGLEERRORMESSAGE_HPP_

#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "../internal/strings/strings.hpp"

namespace rfl::parsing {

/// Combines a set of errors to a single, readable error message.
inline std::string to_single_error_message(
    std::vector<Error> _errors,
    std::optional<std::string> _msg_prefix = std::nullopt,
    size_t _err_limit = 10) {
  if (_errors.size() == 1) {
    return std::move(_errors[0].what());
  } else {
    std::stringstream stream;
    stream << (_msg_prefix
                   ? *_msg_prefix
                   : "Found " + std::to_string(_errors.size()) + " errors:");
    for (size_t i = 0; i < _errors.size() && i < _err_limit; ++i) {
      stream << "\n"
             << i + 1 << ") "
             << internal::strings::replace_all(_errors.at(i).what(), "\n",
                                               "\n    ");
    }
    if (_errors.size() > _err_limit) {
      stream << "\n...\nMore than " << _err_limit
             << " errors occurred, but I am only showing the "
                "first "
             << _err_limit << ".";
    }
    return stream.str();
  }
}

}  // namespace rfl::parsing

#endif
