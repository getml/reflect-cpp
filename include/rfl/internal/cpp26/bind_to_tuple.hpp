#ifndef RFL_INTERNAL_CPP26_BIND_TO_TUPLE_HPP_
#define RFL_INTERNAL_CPP26_BIND_TO_TUPLE_HPP_

#include <array>
#include <cassert>
#include <meta>
#include <ranges>

#include "../../Tuple.hpp"

namespace rfl::internal::cpp26 {

template <class T>
auto bind_to_tuple(T& _t) {
  auto& [... xs] = _t;
  return rfl::make_tuple(&xs...);
}

}  // namespace rfl::internal::cpp26

#endif
