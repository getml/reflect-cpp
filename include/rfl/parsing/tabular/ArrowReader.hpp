#ifndef RFL_PARSING_TABULAR_ARROWREADER_HPP_
#define RFL_PARSING_TABULAR_ARROWREADER_HPP_

#include <arrow/api.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include "../../Result.hpp"
#include "../../Tuple.hpp"
#include "../../apply.hpp"
#include "../../get.hpp"
#include "../../named_tuple_t.hpp"
#include "../../to_view.hpp"
#include "make_chunked_array_ranges.hpp"

namespace rfl::parsing::tabular {

template <class VecType>
class ArrowReader {
 public:
  using ValueType = typename std::remove_cvref_t<typename VecType::value_type>;

  static Result<ArrowReader> make(const std::shared_ptr<arrow::Table>& _table) {
    try {
      return ArrowReader(_table);
    } catch (const std::exception& e) {
      return error("Could not create ArrowReader: " + e.what());
    }
  }

  ~ArrowReader() = default;

  static Result<VecType> read(const ArrowReader& _r) { return _r.read(); }

 private:
  ArrowReader(const std::shared_ptr<arrow::Table>& _table)
      : table_(Ref<arrow::Table>::make(_table).value()) {}

  Result<VecType> read() const {
    const auto chunked_array_ranges = make_chunked_array_ranges(table_);
    if (!chunked_array_ranges) {
      return error(chunked_array_ranges.error().what());
    }
    auto chunked_array_iterators =
        apply([](const auto&... _rs) { return make_tuple(_rs.begin()...); },
              chunked_array_ranges.value());
    VecType result;
    // TODO
    return result;
  }

 private:
  Ref<arrow::Table> table_;
};

}  // namespace rfl::parsing::tabular

#endif
