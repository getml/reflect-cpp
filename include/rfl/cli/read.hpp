#ifndef RFL_CLI_READ_HPP_
#define RFL_CLI_READ_HPP_

#include "../Processors.hpp"
#include "../SnakeCaseToKebabCase.hpp"
#include "Parser.hpp"
#include "Reader.hpp"
#include "parse_argv.hpp"

namespace rfl::cli {

/// Parses command-line arguments into a struct using reflection.
/// Field names are automatically converted from snake_case to kebab-case.
/// Example: struct field `host_name` matches CLI argument `--host-name`.
template <class T, class... Ps>
rfl::Result<T> read(int argc, char* argv[]) {
  return parse_argv(argc, argv).and_then(
      [](auto _args) -> rfl::Result<T> {
        const auto r = Reader();
        const auto var = CliVarType{&_args, "", std::nullopt};
        return Parser<T, Processors<SnakeCaseToKebabCase, Ps...>>::read(r, var);
      });
}

}  // namespace rfl::cli

#endif
