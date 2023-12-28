#ifndef RFL_PARSING_AREREADERANDWRITER_HPP_
#define RFL_PARSING_AREREADERANDWRITER_HPP_

#include "IsReader.hpp"
#include "IsWriter.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T>
concept AreReaderAndWriter = IsReader<R, T> && IsWriter<W, T>;

}  // namespace parsing
}  // namespace rfl

#endif
