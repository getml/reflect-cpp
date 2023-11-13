#ifndef FLEXBUF_PARSER_HPP_
#define FLEXBUF_PARSER_HPP_

#include "rfl/flexbuf/Reader.hpp"
#include "rfl/flexbuf/Writer.hpp"
#include "rfl/parsing/Parser.hpp"

namespace rfl {
namespace flexbuf {

template <class T>
using Parser = rfl::parsing::Parser<Reader, Writer, T>;

}
}  // namespace rfl

#endif
