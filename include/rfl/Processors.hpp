#ifndef RFL_INTERNAL_PROCESSORS_HPP_
#define RFL_INTERNAL_PROCESSORS_HPP_

#include <type_traits>
#include <utility>

#include "Tuple.hpp"

namespace rfl {

template <class... Ps>
struct Processors;

template <>
struct Processors<> {
  template <class T, class NamedTupleType>
  static auto process(NamedTupleType&& _named_tuple) {
    return std::forward<NamedTupleType>(_named_tuple);
  }
};

template <class Head, class... Tail>
struct Processors<Head, Tail...> {
  template <class T, class NamedTupleType>
  static auto process(NamedTupleType&& _named_tuple) {
    return Processors<Tail...>::template process<T>(
        Head::template process<T>(std::move(_named_tuple)));
  }
};

}  // namespace rfl

#endif
