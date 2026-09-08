#ifndef RFL_PARSING_CALL_DESTRUCTORS_ON_TUPLE_WHERE_NECESSARY_HPP_
#define RFL_PARSING_CALL_DESTRUCTORS_ON_TUPLE_WHERE_NECESSARY_HPP_

#include <type_traits>

#include "../Tuple.hpp"
#include "call_destructors_where_necessary.hpp"

namespace rfl::parsing {

/**
 * @brief Calls the destructor on a single element of a tuple, if necessary.
 *
 * @tparam TupleType The type of the tuple.
 * @tparam _size The size of the tuple.
 * @tparam _i The index of the element.
 * @param _num_set The number of elements that have been set.
 * @param _tup The tuple to call the destructor on.
 */
template <class TupleType, unsigned long _size, int _i>
void call_destructor_on_one_tuple_element_if_necessary(const size_t _num_set,
                                                       TupleType* _tup) {
  using FieldType = tuple_element_t<_i, TupleType>;
  using ValueType = std::remove_cv_t<std::remove_pointer_t<FieldType>>;
  if constexpr (!std::is_array_v<ValueType> &&
                std::is_destructible_v<ValueType>) {
    if (_i < _num_set) {
      rfl::get<_i>(*_tup).~ValueType();
    }
  } else if constexpr (std::is_array_v<ValueType>) {
    if (_i < _num_set) {
      auto ptr = rfl::get<_i>(*_tup);
      call_destructor_on_array(sizeof(*ptr) / sizeof(**ptr), *ptr);
    }
  }
}

/**
 * @brief Calls the destructors on the elements of a tuple, if necessary.
 *
 * @tparam TupleType The type of the tuple.
 * @param _num_set The number of elements that have been set.
 * @param _tup The tuple to call the destructors on.
 */
template <class TupleType>
void call_destructors_on_tuple_where_necessary(const size_t _num_set,
                                               TupleType* _tup) {
  [&]<int... is>(std::integer_sequence<int, is...>) {
    (call_destructor_on_one_tuple_element_if_necessary<
         TupleType, rfl::tuple_size_v<TupleType>, is>(_num_set, _tup),
     ...);
  }
  (std::make_integer_sequence<int, rfl::tuple_size_v<TupleType>>());
}

}  // namespace rfl::parsing

#endif
