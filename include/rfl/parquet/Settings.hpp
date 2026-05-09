#ifndef RFL_PARQUET_SETTINGS_HPP_
#define RFL_PARQUET_SETTINGS_HPP_

#include <arrow/io/api.h>
#include <parquet/arrow/writer.h>

#include "../Settings.hpp"

namespace rfl::parquet {

using Compression = arrow::Compression::type;

struct Settings {
  /// The size of the chunks of the parquet file.
  const size_t chunksize = 2000;

  /// The compression algorithm used to compress the parquet file.
  const Compression compression = Compression::SNAPPY;

  RFL_SETTINGS_OPS(Settings)
};

}  // namespace rfl::parquet

#endif
