#ifndef RFL_PARQUET_TABLE_HPP_
#define RFL_PARQUET_TABLE_HPP_

#include "../parsing/tabular/ArrowBridge.hpp"
#include "Settings.hpp"

namespace rfl::parquet::detail {
RFL_API Result<std::shared_ptr<arrow::Table>> read_table(const uint8_t*,
                                                         size_t);
RFL_API Ref<arrow::Buffer> write_table(const std::shared_ptr<arrow::Table>&,
                                       const Settings&);
}  // namespace rfl::parquet::detail
#endif
