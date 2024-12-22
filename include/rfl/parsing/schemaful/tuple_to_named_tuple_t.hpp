#ifndef RFL_PARSING_SCHEMAFUL_TUPLETONAMEDTUPLE_T_HPP_
#define RFL_PARSING_SCHEMAFUL_TUPLETONAMEDTUPLE_T_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../../NamedTuple.hpp"
#include "../../Tuple.hpp"
#include "tuple_to_named_tuple.hpp"

namespace rfl::parsing::schemaful {

template <class IntegerSequence, class... Ts>
struct ToNamedTuple;

template <int... _is, class... Ts>
struct ToNamedTuple<std::integer_sequence<int, _is...>, Ts...> {
  using Type = NamedTuple<Field<to_field_name<_is>(), Ts>...>;
};

template <class TupleType>
struct TupleToNamedTuple;

template <class... Ts>
struct TupleToNamedTuple<Tuple<Ts...>> {
  using Type =
      typename ToNamedTuple<std::make_integer_sequence<int, sizeof...(Ts)>,
                            Ts...>::Type;
};

template <class... Ts>
struct TupleToNamedTuple<std::tuple<Ts...>> {
  using Type =
      typename ToNamedTuple<std::make_integer_sequence<int, sizeof...(Ts)>,
                            Ts...>::Type;
};

/// Given
/// std::tuple<T1, T2, T3, ...> or rfl::Tuple<T1, T2, T3, ...>
/// return
/// NamedTuple<Field<"f000", T1>, Field<"f001", T2>, Field<"f002", T3>, ...>
template <class T>
using tuple_to_named_tuple_t = typename TupleToNamedTuple<T>::Type;

}  // namespace rfl::parsing::schemaful

#endif
