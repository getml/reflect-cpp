#ifndef RFL_INTERNAL_CPP26_BIND_TO_TUPLE_HPP_
#define RFL_INTERNAL_CPP26_BIND_TO_TUPLE_HPP_

#ifdef REFLECTCPP_USE_CPP26_REFLECTION

#include <array>
#include <cassert>
#include <meta>
#include <ranges>
#include <tuple>

#include "../../Tuple.hpp"
#include "get_members.hpp"

namespace rfl::internal::cpp26 {

template <class T>
auto bind_to_tuple(T& _t) {
  using Type = std::remove_cvref_t<T>;
  constexpr auto members = std::define_static_array(get_members<Type>());
  constexpr auto [... indices] = std::make_index_sequence<members.size()>();
  return rfl::make_tuple(&_t.[:members[indices]:]...);
}

}  // namespace rfl::internal::cpp26

#endif
#endif
