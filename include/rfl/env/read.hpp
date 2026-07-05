#ifndef RFL_ENV_READ_HPP_
#define RFL_ENV_READ_HPP_

#include "../Processors.hpp"
#include "../SnakeCaseToKebabCase.hpp"
#include "Parser.hpp"
#include "Reader.hpp"
#include "parse_argv.hpp"
#include "resolve_args.hpp"

namespace rfl::cli {

/// Parses command-line arguments into a struct using reflection.
/// Field names are automatically converted from snake_case to kebab-case for
/// ENV arguments. For example, a struct field named `host_name` will match the
/// CLI argument `--host-name`. Supports nested objects (e.g., --database.host),
/// arrays (e.g., --ports=8080,8081), positional arguments, and short flags.
/// @tparam T The struct type to parse into
/// @tparam Ps Optional processors to apply during parsing
/// @param argc Number of command-line arguments
/// @param argv Array of command-line argument strings
/// @return A Result containing the parsed struct or an error
template <class T, class... Ps>
rfl::Result<T> read(int argc, char* argv[]) {
  using ProcessorsType = Processors<SnakeCaseToKebabCase, Ps...>;
  return parse_argv(argc, argv)
      .and_then(resolve_args<T, ProcessorsType>)
      .and_then([](auto _args) -> rfl::Result<T> {
        const auto r = Reader();
        const auto var = CliVarType{&_args, "", std::nullopt};
        return Parser<T, ProcessorsType>::read(r, var);
      });
}

}  // namespace rfl::cli

#endif
