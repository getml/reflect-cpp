#ifndef RFL_ENV_PARSER_HPP_
#define RFL_ENV_PARSER_HPP_

#include "../parsing/Parser.hpp"
#include "../parsing/Parser_base.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl::env {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}  // namespace rfl::env

#endif
