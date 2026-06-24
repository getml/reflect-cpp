module;

#include "rfl/cli.hpp"

export module rfl.cli;
export import rfl;
export import rfl.generic;

export namespace rfl::cli {
using ::rfl::cli::array_delimiter;
using ::rfl::cli::CliArrayType;
using ::rfl::cli::CliObjectType;
using ::rfl::cli::CliVarType;
using ::rfl::cli::looks_like_option;
using ::rfl::cli::parse_argv;
using ::rfl::cli::parse_value;
using ::rfl::cli::ParsedArgs;
using ::rfl::cli::Parser;
using ::rfl::cli::path_separator;
using ::rfl::cli::read;
using ::rfl::cli::Reader;
using ::rfl::cli::resolve_args;
}  // namespace rfl::cli
