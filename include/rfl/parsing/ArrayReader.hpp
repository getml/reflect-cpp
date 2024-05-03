#ifndef RFL_PARSING_ARRAYREADER_HPP_
#define RFL_PARSING_ARRAYREADER_HPP_

#include <array>
#include <optional>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Result.hpp"
#include "../internal/is_array.hpp"

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

  template <size_t _i = 0>
  std::optional<Error> read(const InputVarType& _var) const {
    if constexpr (_i < size_) {
      if (i_ == _i) {
        auto res =
            Parser<R, W, std::remove_cv_t<T>, ProcessorsType>::read(*r_, _var);
        if (res) {
          if constexpr (internal::is_array_v<T>) {
            move_to(std::get<_i>(*array_), &(*res));
          } else {
            std::get<_i>(*array_) = *res;
          }
        } else {
          return Error("Failed to parse element " + std::string(_i) + ": " +
                       res.error()->what());
        }
        ++i_;
        return std::nullopt;
      }
      read<_i + 1>(_var);
    }
  }

  /// Because of the way we have allocated the fields, we need to manually
  /// trigger the destructors.
  template <size_t _i = 0>
  void call_destructors_where_necessary() const {
    if constexpr (_i < size_) {
      if (_i < i_) {
        if constexpr (!std::is_array_v<ValueType> &&
                      std::is_pointer_v<OriginalType> &&
                      std::is_destructible_v<ValueType>) {
          std::get<_i>(*array_).~T();
        } else if constexpr (std::is_array_v<ValueType>) {
          auto ptr = std::get<_i>(*array_);
          call_destructor_on_array(sizeof(*ptr) / sizeof(**ptr), *ptr);
        }
      } else {
        return;
      }
      call_destructors_where_necessary<_i + 1>();
    }
  }

 private:
  template <class T>
  void call_destructor_on_array(const size_t _size, T* _ptr) const {
    for (size_t i = 0; i < _size; ++i) {
      if constexpr (std::is_array_v<T>) {
        call_destructor_on_array(sizeof(*_ptr) / sizeof(**_ptr), *(_ptr + i));
      } else if constexpr (std::is_destructible_v<T>) {
        (_ptr + i)->~T();
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
