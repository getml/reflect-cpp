#ifndef RFL_PARSING_PARSER_BASE_HPP_
#define RFL_PARSING_PARSER_BASE_HPP_

#include "AreReaderAndWriter.hpp"

namespace rfl::parsing {

/**
 * @brief The main parser struct, which must be specialized for each type.
 *
 * @tparam R The reader type.
 * @tparam W The writer type.
 * @tparam T The type to be parsed.
 * @tparam ProcessorsType The processors to use.
 */
template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, T>
struct Parser;

}  // namespace rfl::parsing

#endif
