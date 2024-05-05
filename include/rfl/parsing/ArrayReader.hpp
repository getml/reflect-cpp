#ifndef RFL_PARSING_ARRAYREADER_HPP_
#define RFL_PARSING_ARRAYREADER_HPP_

#include <array>
#include <optional>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Result.hpp"
#include "../internal/is_array.hpp"
#include "Parser_base.hpp"

namespace rfl::parsing {

template <class R, class W, class ProcessorsType, class T, size_t _size>
class ArrayReader {
 private:
  using InputVarType = typename R::InputVarType;
  static constexpr size_t size_ = _size;

 public:
  ArrayReader(const R* _r, std::array<T, _size>* _array)
      : array_(_array), i_(0), r_(_r) {}

  ~ArrayReader() = default;

  std::optional<Error> check_size() const {
    if (i_ != size_) {
      return Error("Expected " + std::to_string(size_) + " elements, got " +
                   std::to_string(i_) + ".");
    }
    return std::nullopt;
  }

  std::optional<Error> read(const InputVarType& _var) const {
    auto res =
        Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(*r_, _var);
    if (res) {
      move_to(&((*array_)[i_]), &(*res));
    } else {
      return Error("Failed to parse element " + std::to_string(i_) + ": " +
                   res.error()->what());
    }
    ++i_;
    return std::nullopt;
  }

  /// Because of the way we have allocated the fields, we need to manually
  /// trigger the destructors.
  void call_destructors_where_necessary() const {
    for (size_t i = 0; i < std::min(i_, size_); ++i) {
      if constexpr (!std::is_array_v<T> && std::is_pointer_v<T> &&
                    std::is_destructible_v<T>) {
        (*array_)[i].~T();
      } else if constexpr (std::is_array_v<T>) {
        auto ptr = (*array_)[i];
        call_destructor_on_array(sizeof(*ptr) / sizeof(**ptr), *ptr);
      }
    }
  }

 private:
  template <class U>
  void call_destructor_on_array(const size_t _size2, U* _ptr) const {
    for (size_t i = 0; i < _size2; ++i) {
      if constexpr (std::is_array_v<U>) {
        call_destructor_on_array(sizeof(*_ptr) / sizeof(**_ptr), *(_ptr + i));
      } else if constexpr (std::is_destructible_v<U>) {
        (_ptr + i)->~U();
      }
    }
  }

  template <class Target, class Source>
  void move_to(Target* _t, Source* _s) const {
    if constexpr (std::is_const_v<Target>) {
      return move_to(const_cast<std::remove_const_t<Target>*>(_t), _s);
    } else if constexpr (!internal::is_array_v<Source> &&
                         !std::is_array_v<Target>) {
      ::new (_t) Target(std::move(*_s));
    } else if constexpr (internal::is_array_v<Source>) {
      for (size_t i = 0; i < _s->arr_.size(); ++i) {
        move_to(&((*_t)[i]), &(_s->arr_[i]));
      }
    } else {
      for (size_t i = 0; i < _s->size(); ++i) {
        move_to(&((*_t)[i]), &((*_s)[i]));
      }
    }
  }

 private:
  /// The underlying array.
  std::array<T, size_>* array_;

  /// Indicates the current field
  mutable size_t i_;

  /// The underlying reader.
  const R* r_;
};

}  // namespace rfl::parsing

#endif
