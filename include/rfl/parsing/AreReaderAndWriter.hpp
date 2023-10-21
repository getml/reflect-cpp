#ifndef RFL_PARSING_AREREADERANDWRITER_HPP_
#define RFL_PARSING_AREREADERANDWRITER_HPP_

#include "rfl/parsing/IsReader.hpp"
#include "rfl/parsing/IsWriter.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T>
concept AreReaderAndWriter = IsReader<R, T> && IsWriter<W>;

}  // namespace parsing
}  // namespace rfl

#endif
