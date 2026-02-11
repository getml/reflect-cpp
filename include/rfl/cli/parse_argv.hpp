#ifndef RFL_CLI_PARSE_ARGV_HPP_
#define RFL_CLI_PARSE_ARGV_HPP_

#include <map>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "../Result.hpp"
#include "resolve_args.hpp"

namespace rfl::cli {

/// Parses command-line arguments into categorized buckets:
/// --key=value / --flag → named
/// -x value / -x=value / -x → short_args
/// bare arguments → positional
/// -- → everything after goes to positional
inline rfl::Result<ParsedArgs> parse_argv(int argc, char* argv[]) {
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
        return error("Empty key in argument: --" + std::string(arg));
      }
      const auto key_copy = key;
      if (!result.named.emplace(std::move(key), std::move(val)).second) {
        return error("Duplicate argument: --" + key_copy);
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
        const auto short_key_copy = key;
        if (!result.short_args.emplace(std::move(key), std::move(val)).second) {
          return error("Duplicate short argument: -" + short_key_copy);
        }
      }
      else {
        auto key = std::string(arg);
        const auto short_key_copy = key;
        // Peek at next token: if it exists and doesn't start with '-',
        // consume it as the value.
        if (i + 1 < args.size()) {
          const std::string_view next(args[i + 1]);
          if (!next.empty() && next[0] != '-') {
            ++i;
            auto val = std::string(next);
            if (!result.short_args.emplace(
                    std::move(key), std::move(val)).second) {
              return error("Duplicate short argument: -" + short_key_copy);
            }
            continue;
          }
        }
        // No value — boolean flag.
        if (!result.short_args.emplace(std::move(key), "").second) {
          return error("Duplicate short argument: -" + short_key_copy);
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
