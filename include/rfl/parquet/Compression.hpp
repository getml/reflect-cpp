#ifndef RFL_PARQUET_COMPRESSION_HPP_
#define RFL_PARQUET_COMPRESSION_HPP_

#include <arrow/io/api.h>

namespace rfl::parquet {

using Compression = arrow::Compression::type;

}  // namespace rfl::parquet

#endif
