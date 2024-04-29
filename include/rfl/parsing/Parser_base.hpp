#ifndef RFL_PARSING_PARSER_BASE_HPP_
#define RFL_PARSING_PARSER_BASE_HPP_

#include "AreReaderAndWriter.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, class ProcessorsType>
requires AreReaderAndWriter<R, W, T>
struct Parser;

}  // namespace parsing
}  // namespace rfl

#endif
