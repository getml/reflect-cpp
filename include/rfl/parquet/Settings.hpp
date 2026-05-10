#ifndef RFL_PARQUET_SETTINGS_HPP_
#define RFL_PARQUET_SETTINGS_HPP_

#include <arrow/io/api.h>
#include <parquet/arrow/writer.h>

#include "../Settings.hpp"
#include "../internal/deprecated_with.hpp"

namespace rfl::parquet {

using Compression = arrow::Compression::type;

struct Settings {
  /// The size of the chunks of the parquet file.
  const size_t chunksize = 2000;

  /// The compression algorithm used to compress the parquet file.
  const Compression compression = Compression::SNAPPY;

  RFL_SETTINGS_OPS(Settings)

  RFL_DEPRECATED_WITH(chunksize)
  Settings with_chunksize(const size_t _chunksize) const noexcept {
    return replace(*this, make_field<"chunksize">(_chunksize));
  }

  RFL_DEPRECATED_WITH(compression)
  Settings with_compression(const Compression _compression) const noexcept {
    return replace(*this, make_field<"compression">(_compression));
  }
};

}  // namespace rfl::parquet

#endif
