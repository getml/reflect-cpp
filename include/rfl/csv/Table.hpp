#ifndef RFL_CSV_TABLE_HPP_
#define RFL_CSV_TABLE_HPP_

#include "../parsing/tabular/ArrowBridge.hpp"
#include "Settings.hpp"

namespace rfl::csv::detail {
RFL_API Result<std::shared_ptr<arrow::Table>> read_table(const char*, size_t,
                                                         const Settings&);
RFL_API Ref<arrow::Buffer> write_table(const std::shared_ptr<arrow::Table>&,
                                       const Settings&);
}  // namespace rfl::csv::detail
#endif
