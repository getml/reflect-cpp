#ifndef RFL_PARSING_TABULAR_MAKECHUNKEDARRAYITERATORS_HPP_
#define RFL_PARSING_TABULAR_MAKECHUNKEDARRAYITERATORS_HPP_

#include <arrow/api.h>

#include <array>
#include <cinttypes>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "../../NamedTuple.hpp"
#include "../../Ref.hpp"
#include "../../Result.hpp"
#include "../../Tuple.hpp"
#include "ArrowTypes.hpp"
#include "ChunkedArrayIterator.hpp"

namespace rfl::parsing::tabular {

template <class NamedTupleType>
struct MakeChunkedArrayIterators;

template <class... FieldTypes>
struct MakeChunkedArrayIterators<NamedTuple<FieldTypes...>> {
  using TupleType = Tuple<ChunkedArrayIterator<typename FieldTypes::Type>...>;

  Result<TupleType> operator()(const Ref<arrow::Table>& _table) const {
    const auto get_column =
        [&](const std::string& _colname) -> Result<Ref<arrow::ChunkedArray>> {
      const auto col = _table->GetColumnByName(_colname);
      if (!col) {
        return error("Column named '" + _colname + "' not found.");
      }
      return Ref<arrow::ChunkedArray>::make(col);
    };

    try {
      return TupleType(
          get_column(typename FieldTypes::Name().str())
              .transform(ChunkedArrayIterator<typename FieldTypes::Type>::make)
              .value()...);
    } catch (const std::exception& e) {
      return error(e.what());
    }
  }
};

template <class NamedTupleType>
const auto make_chunked_array_iterators =
    MakeChunkedArrayIterators<NamedTupleType>{};

}  // namespace rfl::parsing::tabular

#endif
