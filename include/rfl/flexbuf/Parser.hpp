#ifndef FLEXBUF_PARSER_HPP_
#define FLEXBUF_PARSER_HPP_

#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl {
namespace flexbuf {

template <class T>
using Parser = rfl::parsing::Parser<Reader, Writer, T>;

}
}  // namespace rfl

#endif
