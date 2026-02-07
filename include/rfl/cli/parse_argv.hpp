#ifndef RFL_CLI_PARSE_ARGV_HPP_
#define RFL_CLI_PARSE_ARGV_HPP_

#include <map>
#include <span>
#include <string>
#include <string_view>

#include "../Result.hpp"

namespace rfl::cli {

/// Parses command-line arguments into a flat key-value map.
/// Expects format: --key=value or --flag (boolean).
inline rfl::Result<std::map<std::string, std::string>> parse_argv(
    int argc, char* argv[]) {
  std::map<std::string, std::string> result;
  if (argc <= 1) {
    return result;
  }
  for (const std::string_view arg_raw : std::span(argv + 1, argc - 1)) {
    if (arg_raw.size() < 2 || arg_raw[0] != '-' || arg_raw[1] != '-') {
      return error(
          "Expected argument starting with '--', got: " + std::string(arg_raw));
    }
    const auto arg = arg_raw.substr(2);
    const auto eq = arg.find('=');
    auto key = std::string(eq == std::string_view::npos ? arg : arg.substr(0, eq));
    auto val = std::string(eq == std::string_view::npos ? "" : arg.substr(eq + 1));
    if (key.empty()) {
      return error("Empty key in argument: --" + std::string(arg));
    }
    if (!result.emplace(std::move(key), std::move(val)).second) {
      return error("Duplicate argument: --" + std::string(key));
    }
  }
  return result;
}

}  // namespace rfl::cli

#endif
