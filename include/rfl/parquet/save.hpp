#ifndef RFL_PARQUET_SAVE_HPP_
#define RFL_PARQUET_SAVE_HPP_

#include <string>

#include "../Result.hpp"
#include "../io/save_bytes.hpp"
#include "Settings.hpp"
#include "write.hpp"

namespace rfl::parquet {

/// @brief Saves an object to a file using the specified settings.
/// @tparam Ps Variadic template parameters for the write function.
/// @param _fname The filename to save to.
/// @param _obj The object to be saved.
/// @param _settings Optional settings for saving (default constructed if not
/// provided).
/// @return Result<Nothing> indicating success or failure.
template <class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj,
                     const Settings& _settings = Settings{}) {
  /// @brief Writes the object to the output stream using the provided settings.
  /// @param _obj The object to write.
  /// @param _stream The output stream to write to.
  /// @return Reference to the output stream after writing.
  const auto write_func = [&](const auto& _obj, auto& _stream) -> auto& {
    return write<Ps...>(_obj, _stream, _settings);
  };
  return rfl::io::save_bytes(_fname, _obj, write_func);
}

}  // namespace rfl::parquet

#endif
