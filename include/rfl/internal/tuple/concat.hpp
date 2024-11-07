#ifndef RFL_INTERNAL_TUPLE_CONCAT_HPP_
#define RFL_INTERNAL_TUPLE_CONCAT_HPP_

#include <utility>

#include "../../Tuple.hpp"

namespace rfl::internal::tuple {

template <class... Types>
struct TupleWrapper {
  rfl::Tuple<Types...> tuple_;
};

template <class... Types>
auto wrap_tuple(const rfl::Tuple<Types...>& _tuple) {
  return TupleWrapper<Types...>{_tuple};
}

template <class... Types>
auto wrap_tuple(rfl::Tuple<Types...>&& _tuple) {
  return TupleWrapper<Types...>{std::forward<rfl::Tuple<Types...> >(_tuple)};
}

template <class... Types1, class... Types2, int... _is, int... _js>
auto concat_two(rfl::Tuple<Types1...>&& _t1, rfl::Tuple<Types2...>&& _t2,
                std::integer_sequence<int, _is...>,
                std::integer_sequence<int, _js...>) {
  return rfl::Tuple<Types1..., Types2...>(std::move(rfl::get<_is>(_t1))...,
                                          std::move(rfl::get<_js>(_t2))...);
}

template <class... Types1, class... Types2>
auto operator+(TupleWrapper<Types1...>&& _t1, TupleWrapper<Types2...>&& _t2) {
  return TupleWrapper<Types1..., Types2...>{
      .tuple_ =
          concat_two(std::move(_t1.tuple_), std::move(_t2.tuple_),
                     std::make_integer_sequence<int, sizeof...(Types1)>(),
                     std::make_integer_sequence<int, sizeof...(Types2)>())};
}

template <class Head, class... Tail>
auto concat(const Head& _head, const Tail&... _tail) {
  return (wrap_tuple(_head) + ... + wrap_tuple(_tail));
}

template <class Head, class... Tail>
auto concat(Head&& _head, Tail&&... _tail) {
  return (wrap_tuple(std::forward<Head>(_head)) + ... +
          wrap_tuple(std::forward<Tail>(_tail)))
      .tuple_;
}

}  // namespace rfl::internal::tuple

#endif
