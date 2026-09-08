#ifndef RFL_PARSING_AREREADERANDWRITER_HPP_
#define RFL_PARSING_AREREADERANDWRITER_HPP_

#include "IsReader.hpp"
#include "IsWriter.hpp"

namespace rfl {
namespace parsing {

/**
 * @brief Checks if R and W are a reader and a writer for type T.
 *
 * @tparam R The reader type.
 * @tparam W The writer type.
 * @tparam T The type to be read and written.
 */
template <class R, class W, class T>
concept AreReaderAndWriter = IsReader<R, T> && IsWriter<W, T>;

}  // namespace parsing
}  // namespace rfl

#endif
