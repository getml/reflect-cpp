#ifndef RFL_PARSING_CALL_DESTRUCTORS_WHERE_NECESSARY_HPP_
#define RFL_PARSING_CALL_DESTRUCTORS_WHERE_NECESSARY_HPP_

#include <array>
#include <type_traits>

#include "../get.hpp"

namespace rfl::parsing {

/// Because of the way we have allocated the fields, we need to manually
/// trigger the destructors.

template <class T>
void call_destructor_on_array(const size_t _size, T* _ptr) {
  for (size_t i = 0; i < _size; ++i) {
    if constexpr (std::is_array_v<T>) {
      call_destructor_on_array(sizeof(*_ptr) / sizeof(**_ptr), *(_ptr + i));
    } else if constexpr (std::is_destructible_v<std::remove_cv_t<T>>) {
      (_ptr + i)->~T();
    }
  }
}

template <class ViewType, unsigned long _size, int _i>
void call_destructor_on_one_if_necessary(const std::array<bool, _size>& _set,
                                         ViewType* _view) {
  using FieldType = tuple_element_t<_i, typename ViewType::Fields>;
  using OriginalType = std::remove_cv_t<typename FieldType::Type>;
  using ValueType =
      std::remove_cvref_t<std::remove_pointer_t<typename FieldType::Type>>;
  if constexpr (!std::is_array_v<ValueType> &&
                std::is_pointer_v<OriginalType> &&
                std::is_destructible_v<ValueType>) {
    if (std::get<_i>(_set)) {
      rfl::get<_i>(*_view)->~ValueType();
    }
  } else if constexpr (std::is_array_v<ValueType>) {
    if (std::get<_i>(_set)) {
      auto ptr = rfl::get<_i>(*_view);
      call_destructor_on_array(sizeof(*ptr) / sizeof(**ptr), *ptr);
    }
  }
}

template <class ViewType, auto _size>
void call_destructors_where_necessary(const std::array<bool, _size>& _set,
                                      ViewType* _view) {
  [&]<int... is>(std::integer_sequence<int, is...>) {
    (call_destructor_on_one_if_necessary<ViewType, _size, is>(_set, _view),
     ...);
  }(std::make_integer_sequence<int, _size>());
}

}  // namespace rfl::parsing

#endif
