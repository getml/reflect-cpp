#ifndef RFL_PARSING_TABULAR_CHUNKEDARRAYRANGE_HPP_
#define RFL_PARSING_TABULAR_CHUNKEDARRAYRANGE_HPP_

#include <arrow/api.h>

#include <memory>
#include <stdexcept>
#include <string>

#include "../../Ref.hpp"
#include "../../Result.hpp"
#include "ChunkedArrayIterator.hpp"
#include "array_t.hpp"

namespace rfl::parsing::tabular {

template <class T>
class ChunkedArrayRange {
 public:
  static ChunkedArrayRange make(const Ref<arrow::ChunkedArray>& _arr) {
    return ChunkedArrayRange(_arr);
  }

  ChunkedArrayRange(const Ref<arrow::ChunkedArray>& _arr) : arr_(_arr) {}

  ~ChunkedArrayRange() = default;

  auto begin() const { return ChunkedArrayIterator<T>(arr_); }

  auto end() const { return ChunkedArrayIterator<T>::End{}; }

 private:
  Ref<arrow::ChunkedArray> arr_;
};

}  // namespace rfl::parsing::tabular

#endif
