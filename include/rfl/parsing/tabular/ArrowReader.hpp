#ifndef RFL_PARSING_TABULAR_ARROWREADER_HPP_
#define RFL_PARSING_TABULAR_ARROWREADER_HPP_

#include <arrow/api.h>

#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "../../Result.hpp"
#include "../../Tuple.hpp"
#include "../../get.hpp"
#include "../../named_tuple_t.hpp"
#include "../../to_view.hpp"
#include "add_to_builder.hpp"
#include "arrow_builders_t.hpp"
#include "make_arrow_data_types.hpp"
#include "make_arrow_schema.hpp"

namespace rfl::parsing::tabular {

template <class VecType>
class ArrowReader {
 public:
  using ValueType = typename std::remove_cvref_t<typename VecType::value_type>;

  ArrowReader(const size_t _chunksize) : chunksize_(_chunksize) {}

  ~ArrowReader() = default;

  Result<VecType> from_table(
      const std::shared_ptr<arrow::Table>& _table) const {}
};

}  // namespace rfl::parsing::tabular

#endif
