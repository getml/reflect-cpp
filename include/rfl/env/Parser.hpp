#ifndef RFL_ENV_PARSER_HPP_
#define RFL_ENV_PARSER_HPP_

#include "../NamedTuple.hpp"
#include "../generic/Writer.hpp"
#include "../internal/no_field_names_v.hpp"
#include "../internal/no_optionals_v.hpp"
#include "../parsing/NamedTupleParser.hpp"
#include "../parsing/Parser.hpp"
#include "../parsing/Parser_base.hpp"
#include "Reader.hpp"

namespace rfl::env {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}  // namespace rfl::env

#endif
