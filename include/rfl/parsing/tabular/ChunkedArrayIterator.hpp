#ifndef RFL_PARSING_TABULAR_CHUNKEDARRAYITERATOR_HPP_
#define RFL_PARSING_TABULAR_CHUNKEDARRAYITERATOR_HPP_

#include <arrow/api.h>

#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "../../Ref.hpp"
#include "../../Result.hpp"
#include "../../internal/ptr_cast.hpp"
#include "../is_required.hpp"
#include "array_t.hpp"

namespace rfl::parsing::tabular {

template <class T>
class ChunkedArrayIterator {
 public:
  using difference_type = std::ptrdiff_t;
  using value_type = Result<T>;

  using ArrayType = array_t<T>;

  static ChunkedArrayIterator make(const Ref<arrow::ChunkedArray>& _arr) {
    return ChunkedArrayIterator(_arr);
  }

  ChunkedArrayIterator(const Ref<arrow::ChunkedArray>& _arr)
      : arr_(_arr), chunk_ix_(0), current_chunk_(get_chunk(arr_, 0)), ix_(0) {}

  ~ChunkedArrayIterator() = default;

  Result<T> operator*() const noexcept {
    const bool is_null =
        current_chunk_
            .transform([&](const auto& _c) { return _c->IsNull(ix_); })
            .value_or(false);

    if (is_null) {
      if constexpr (is_required<T, false>()) {
        return error("Value cannot be null.");
      } else {
        return T();
      }
    }

    if constexpr (std::is_same_v<ArrayType, arrow::BinaryArray>) {
      return current_chunk_.transform([&](const auto& _c) {
        const auto begin = internal::ptr_cast<const typename T::value_type*>(
            _c->Value(ix_).data());
        return T(begin, begin + _c->Value(ix_).size());
      });

    } else if constexpr (std::is_same_v<ArrayType, arrow::StringArray>) {
      return current_chunk_.transform(
          [&](const auto& _c) { return T(std::string(_c->Value(ix_))); });

    } else if constexpr (std::is_same_v<ArrayType, arrow::TimestampArray>) {
      return current_chunk_.transform(
          [&](const auto& _c) { return T(_c->Value(ix_) / 1000); });

    } else {
      return current_chunk_.transform(
          [&](const auto& _c) { return T(_c->Value(ix_)); });
    }
  }

  bool end() const noexcept {
    return !current_chunk_ || (chunk_ix_ >= arr_->num_chunks());
  }

  ChunkedArrayIterator<T>& operator++() noexcept {
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
    if (_chunk_ix < _arr->num_chunks()) {
      return Ref<ArrayType>::make(
          std::static_pointer_cast<ArrayType>(_arr->chunk(_chunk_ix)));
    } else {
      return error("chunk_ix out of bounds.");
    }
  }

 private:
  Ref<arrow::ChunkedArray> arr_;

  int chunk_ix_;

  Result<Ref<ArrayType>> current_chunk_;

  int64_t ix_;
};

}  // namespace rfl::parsing::tabular

#endif
