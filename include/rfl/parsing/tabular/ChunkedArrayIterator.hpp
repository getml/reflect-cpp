#ifndef RFL_PARSING_TABULAR_CHUNKEDARRAYITERATOR_HPP_
#define RFL_PARSING_TABULAR_CHUNKEDARRAYITERATOR_HPP_

#include <arrow/api.h>

#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "../../Ref.hpp"
#include "../../Result.hpp"
#include "array_t.hpp"

namespace rfl::parsing::tabular {

template <class T>
class ChunkedArrayIterator {
 public:
  using difference_type = std::ptrdiff_t;
  using value_type = Result<T>;

  using ArrayType = array_t<T>;

  struct End {
    bool operator==(const ChunkedArrayIterator<T>& _it) const noexcept {
      return _it == *this;
    }

    bool operator!=(const ChunkedArrayIterator<T>& _it) const noexcept {
      return _it != *this;
    }
  };

  ChunkedArrayIterator(const Ref<arrow::ChunkedArray>& _arr)
      : arr_(_arr), chunk_ix_(0), current_chunk_(get_chunk(arr_, 0)), ix_(0) {}

  ~ChunkedArrayIterator() = default;

  Result<T> operator*() const noexcept {
    return current_chunk_.transform(
        [&](const auto& _c) { return _c->Value(ix_); });
  }

  bool operator==(const End&) const noexcept {
    return chunk_ix_ >= arr_->num_chunks();
  }

  bool operator!=(const End& _end) const noexcept { return !(*this == _end); }

  Iterator<T>& operator++() noexcept {
    if (!current_chunk_) {
      return *this;
    }
    ++ix_;
    if (ix_ >= (*current_chunk_)->length()) {
      ++chunk_ix_;
      current_chunk_ = get_chunk(arr_, chunk_ix_);
      ix_ = 0;
    }
    return *this;
  }

  void operator++(int) noexcept { ++*this; }

 private:
  static Result<Ref<ArrayType>> get_chunk(const Ref<arrow::ChunkedArray>& _arr,
                                          const int _chunk_ix) noexcept {
    return Ref<ArrayType>::make(
        std::dynamic_pointer_cast<std::shared_ptr<ArrayType>>(
            arr_->chunk(chunk_ix_)));
  }

 private:
  Ref<arrow::ChunkedArray> arr_;

  int chunk_ix_;

  Result<Ref<ArrayType>> current_chunk_;

  int64_t ix_;
};

}  // namespace rfl::parsing::tabular

#endif
