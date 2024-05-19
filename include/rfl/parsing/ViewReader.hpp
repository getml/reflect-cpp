#ifndef RFL_PARSING_VIEWREADER_HPP_
#define RFL_PARSING_VIEWREADER_HPP_

#include <array>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Result.hpp"
#include "../internal/is_array.hpp"

namespace rfl::parsing {

template <class R, class W, class ViewType, class ProcessorsType>
class ViewReader {
 private:
  using InputVarType = typename R::InputVarType;
  static constexpr size_t size_ = ViewType::size();

 public:
  ViewReader(const R* _r, ViewType* _view, std::array<bool, size_>* _found,
             std::array<bool, size_>* _set, std::vector<Error>* _errors)
      : r_(_r), view_(_view), found_(_found), set_(_set), errors_(_errors) {}

  ~ViewReader() = default;

  template <size_t _i = 0>
  void read(const std::string_view& _name, const InputVarType& _var) const {
    if constexpr (_i < size_) {
      using FieldType = std::tuple_element_t<_i, typename ViewType::Fields>;
      using OriginalType = std::remove_cvref_t<typename FieldType::Type>;
      using CurrentType =
          std::remove_cvref_t<std::remove_pointer_t<OriginalType>>;
      constexpr auto current_name = FieldType::name();
      if (!std::get<_i>(*found_) && _name == current_name) {
        auto res = Parser<R, W, CurrentType, ProcessorsType>::read(*r_, _var);
        if (res) {
          if constexpr (std::is_pointer_v<OriginalType>) {
            move_to(rfl::get<_i>(*view_), &(*res));
          } else {
            rfl::get<_i>(*view_) = *res;
          }
          std::get<_i>(*set_) = true;
        } else {
          errors_->push_back(Error("Failed to parse field '" +
                                   std::string(current_name) +
                                   "': " + res.error()->what()));
        }
        std::get<_i>(*found_) = true;
        return;
      }
      read<_i + 1>(_name, _var);
    }
  }

  /// Because of the way we have allocated the fields, we need to manually
  /// trigger the destructors.
  template <size_t _i = 0>
  void call_destructors_where_necessary() const {
    if constexpr (_i < size_) {
      using FieldType = std::tuple_element_t<_i, typename ViewType::Fields>;
      using OriginalType = std::remove_cvref_t<typename FieldType::Type>;
      using ValueType =
          std::remove_cvref_t<std::remove_pointer_t<typename FieldType::Type>>;
      if constexpr (!std::is_array_v<ValueType> &&
                    std::is_pointer_v<OriginalType> &&
                    std::is_destructible_v<ValueType>) {
        if (std::get<_i>(*set_)) {
          rfl::get<_i>(*view_)->~ValueType();
        }
      } else if constexpr (std::is_array_v<ValueType>) {
        if (std::get<_i>(*set_)) {
          auto ptr = rfl::get<_i>(*view_);
          call_destructor_on_array(sizeof(*ptr) / sizeof(**ptr), *ptr);
        }
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
      static_assert(std::is_array_v<Target>,
                    "Expected target to be a c-array.");
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
  /// The underlying reader.
  const R* r_;

  /// The underlying view.
  ViewType* view_;

  /// Indicates that a certain field has been found.
  std::array<bool, size_>* found_;

  /// Indicates that a certain field has been successfully set - necessary,
  /// because we have to trigger the destructors manually.
  std::array<bool, size_>* set_;

  /// Collects any errors we may have come across.
  std::vector<Error>* errors_;
};

}  // namespace rfl::parsing

#endif
