#ifndef RFL_PARQUET_SETTINGS_HPP_
#define RFL_PARQUET_SETTINGS_HPP_

#include <arrow/io/api.h>
#include <parquet/arrow/writer.h>

#include "../Field.hpp"
#include "../replace.hpp"

namespace rfl::parquet {

using Compression = arrow::Compression::type;

struct Settings {
  /// The size of the chunks of the parquet file.
  size_t chunksize = 2000;

  /// The compression algorithm used to compress the parquet file.
  Compression compression = Compression::SNAPPY;

  Settings with_chunksize(const size_t _chunksize) const noexcept {
    return replace(*this, make_field<"chunksize">(_chunksize));
  }

  Settings with_compression(const Compression _compression) const noexcept {
    return replace(*this, make_field<"compression">(_compression));
  }
};

}  // namespace rfl::parquet

#endif
