#ifndef RFL_CLI_PARSER_HPP_
#define RFL_CLI_PARSER_HPP_

#include "../generic/Writer.hpp"
#include "../parsing/Parser.hpp"
#include "Reader.hpp"

namespace rfl::cli {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, generic::Writer, T, ProcessorsType>;

}  // namespace rfl::cli

#endif
