#ifndef RFL_INTERNAL_PROCESSORS_HPP_
#define RFL_INTERNAL_PROCESSORS_HPP_

#include <type_traits>

#include "internal/is_no_optionals_v.hpp"

namespace rfl {

template <class... Ps>
struct Processors;

template <>
struct Processors<> {
  static constexpr bool all_required_ = false;

  template <class T, class NamedTupleType>
  static auto process(NamedTupleType&& _named_tuple) {
    return _named_tuple;
  }
};

template <class Head, class... Tail>
struct Processors<Head, Tail...> {
  static constexpr bool all_required_ =
      std::disjunction_v<internal::is_no_optionals<Head>,
                         internal::is_no_optionals<Tail>...>;

  template <class T, class NamedTupleType>
  static auto process(NamedTupleType&& _named_tuple) {
    return Processors<Tail...>::template process<T>(
        Head::template process<T>(std::move(_named_tuple)));
  }
};

}  // namespace rfl

#endif
