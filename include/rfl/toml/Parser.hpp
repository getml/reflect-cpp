#ifndef RFL_TOML_PARSER_HPP_
#define RFL_TOML_PARSER_HPP_

#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl::toml {

template <class T>
using Parser = parsing::Parser<Reader, Writer, T>;

}  // namespace rfl::toml

#endif
