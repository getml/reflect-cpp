#ifndef RFL_CLI_PARSE_ARGV_HPP_
#define RFL_CLI_PARSE_ARGV_HPP_

#include <cctype>
#include <map>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "../Result.hpp"
#include "resolve_args.hpp"

namespace rfl::cli {

/// Returns true if the token looks like a CLI option (starts with '-'
/// followed by a letter), as opposed to a negative number like "-42".
inline bool looks_like_option(std::string_view _token) noexcept {
  return _token.size() >= 2
         && _token[0] == '-'
         && !std::isdigit(static_cast<unsigned char>(_token[1]))
         && _token[1] != '.';
}

/// Parses command-line arguments into categorized buckets:
/// --key=value / --flag → named
/// -x value / -x=value / -x → short_args
/// bare arguments → positional
/// -- → everything after goes to positional
inline rfl::Result<ParsedArgs> parse_argv(int argc, char* argv[]) {
  if (argc < 0 || (argc > 0 && !argv)) {
    return error("Invalid argc/argv.");
  }
  ParsedArgs result;
  if (argc <= 1) {
    return result;
  }
  const auto args = std::span(argv + 1, argc - 1);
  bool force_positional = false;
  for (size_t i = 0; i < args.size(); ++i) {
    const std::string_view arg_raw(args[i]);

    if (force_positional) {
      result.positional.emplace_back(arg_raw);
      continue;
    }

    // "--" separator: everything after is positional.
    if (arg_raw == "--") {
      force_positional = true;
      continue;
    }

    // Long argument: --key=value or --flag.
    if (arg_raw.size() >= 2 && arg_raw[0] == '-' && arg_raw[1] == '-') {
      const auto arg = arg_raw.substr(2);
      const auto eq = arg.find('=');
      auto key = std::string(
          eq == std::string_view::npos ? arg : arg.substr(0, eq));
      auto val = std::string(
          eq == std::string_view::npos ? "" : arg.substr(eq + 1));
      if (key.empty()) {
        return error("Empty key in argument: " + std::string(arg_raw));
      }
      if (!result.named.emplace(std::move(key), std::move(val)).second) {
        return error("Duplicate argument: " + std::string(arg_raw));
      }
      continue;
    }

    // Short argument: -x value, -x=value, or -x (bool).
    if (arg_raw.size() >= 2 && arg_raw[0] == '-' && arg_raw[1] != '-') {
      const auto arg = arg_raw.substr(1);
      const auto eq = arg.find('=');
      if (eq != std::string_view::npos) {
        // -x=value
        auto key = std::string(arg.substr(0, eq));
        auto val = std::string(arg.substr(eq + 1));
        result.short_order.emplace_back(key);
        if (!result.short_args.emplace(std::move(key), std::move(val)).second) {
          return error("Duplicate short argument: " + std::string(arg_raw));
        }
      }
      else {
        auto key = std::string(arg);
        // Peek at next token: if it exists and doesn't look like an option,
        // consume it as the value.
        if (i + 1 < args.size()) {
          const std::string_view next(args[i + 1]);
          if (!next.empty() && !looks_like_option(next)) {
            ++i;
            auto val = std::string(next);
            result.short_order.emplace_back(key);
            if (!result.short_args.emplace(
                    std::move(key), std::move(val)).second) {
              return error("Duplicate short argument: " + std::string(arg_raw));
            }
            continue;
          }
        }
        // No value — boolean flag.
        result.short_order.emplace_back(key);
        if (!result.short_args.emplace(std::move(key), "").second) {
          return error("Duplicate short argument: " + std::string(arg_raw));
        }
      }
      continue;
    }

    // Bare argument → positional.
    result.positional.emplace_back(arg_raw);
  }
  return result;
}

}  // namespace rfl::cli

#endif
