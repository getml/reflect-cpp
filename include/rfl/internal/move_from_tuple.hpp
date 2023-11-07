#ifndef RFL_MOVE_FROM_TUPLE_HPP_
#define RFL_MOVE_FROM_TUPLE_HPP_

#include <functional>
#include <type_traits>

#include "rfl/internal/is_field.hpp"
#include "rfl/internal/is_named_tuple.hpp"
#include "rfl/internal/nt_to_ptr_named_tuple.hpp"
#include "rfl/internal/ptr_tuple_t.hpp"
#include "rfl/named_tuple_t.hpp"

namespace rfl {
namespace internal {

template <class PtrTupleType, class FlattenedPtrTupleType, class... Args>
auto unflatten_pointers(FlattenedPtrTupleType& _n, Args... _args) {
  constexpr auto i = sizeof...(Args);

  constexpr auto size = std::tuple_size_v<std::decay_t<PtrTupleType>>;

  if constexpr (i == size) {
    return std::make_tuple(_args...);
  } else {
    using Field = std::decay_t<std::tuple_element_t<i, PtrTupleType>>;
    using T = std::decay_t<std::remove_pointer_t<typename Field::Type>>;

    if constexpr (is_flatten_field<Field>::value) {
      using SubPtrTupleType = std::decay_t<ptr_tuple_t<T>>;

      return unflatten_pointers<PtrTupleType>(
          _n, _args..., unflatten_pointers<SubPtrTupleType>(_n));

    } else {
      return unflatten_pointers<PtrTupleType>(
          _n, _args..., _n.template get_field<Field::name_>());
    }
  }
}

template <class T, int _j, class Pointers, class... Args>
auto move_from_flattened_pointers(Pointers& _ptrs, Args&&... _args) {
  constexpr auto i = sizeof...(Args);
  if constexpr (i == std::tuple_size_v<std::decay_t<Pointers>>) {
    return T{std::move(_args)...};
  } else {
    if constexpr (!is_flatten_field_v<FieldType>) {
      return move_from_flattened_pointers<T, _j + 1>(
          _ptrs, std::move(_args)..., std::move(*std::get<_j>(_ptrs)));

    } else {
      // TODO
      /*using PtrTupleType = std::decay_t<unflattened_ptr_tuple_t<T>>;

      using U = std::decay_t<
          std::remove_pointer_t<std::tuple_element_t<i, PtrTupleType>>>;

      return move_from_pointers<T>(_ptrs, std::move(_args)...,
                                   move_from_pointers<U>(std::get<i>(_ptrs)));*/
    }
  }
}

/// Creates a struct of type T from a named tuple by moving the underlying
/// fields.
template <class T, class TupleType>
T move_from_tuple(TupleType&& _n) {
  using RequiredType = std::decay_t<tuple_t<T>>;

  if constexpr (is_named_tuple_v<std::decay_t<T>>) {
    return std::move(_n);

  } else {
    auto flattened_ptr_tuple = tuple_to_ptr_tuple(_n);

    using PtrTupleType = std::decay_t<unflattened_ptr_tuple_t<T>>;

    auto pointers = make_pointers<PtrTupleType>(flattened_ptr_tuple);

    return move_from_flattened_pointers<T, 0>(pointers);
  }
}

}  // namespace internal
}  // namespace rfl

#endif
