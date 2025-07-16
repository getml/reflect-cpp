#ifndef RFL_CBOR_PARSER_HPP_
#define RFL_CBOR_PARSER_HPP_

#include "../Tuple.hpp"
#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl::cbor {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}  // namespace rfl::cbor

#endif
