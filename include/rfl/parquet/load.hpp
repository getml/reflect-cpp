#ifndef RFL_PARQUET_LOAD_HPP_
#define RFL_PARQUET_LOAD_HPP_

#include "../Result.hpp"
#include "../io/load_bytes.hpp"
#include "read.hpp"

namespace rfl::parquet {

/**
 * @brief Loads an object of type T from a file.
 *
 * This function reads the contents of the file specified by _fname,
 * then attempts to deserialize it into an object of type T using the
 * read<T, Ps...> function.
 *
 * @tparam T The type to load from the file.
 * @tparam Ps Additional template parameters for the read function.
 * @param _fname The name of the file to load.
 * @return Result<T> The result of loading and deserializing the file contents.
 */
template <class T, class... Ps>
Result<T> load(const std::string& _fname) {
  const auto read_bytes = [](const auto& _bytes) {
    return read<T, Ps...>(_bytes);
  };
  return rfl::io::load_bytes(_fname).and_then(read_bytes);
}

}  // namespace rfl::parquet

#endif
