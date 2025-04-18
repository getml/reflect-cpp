#ifndef RFL_PARQUET_PARSER_HPP_
#define RFL_PARQUET_PARSER_HPP_

#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl::parquet {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}  // namespace rfl::parquet

#endif
