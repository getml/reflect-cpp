#ifndef RFL_PARSING_SCHEMAFUL_TUPLETONAMEDTUPLE_HPP_
#define RFL_PARSING_SCHEMAFUL_TUPLETONAMEDTUPLE_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../../Field.hpp"
#include "../../Tuple.hpp"
#include "../../internal/StringLiteral.hpp"
#include "../../make_named_tuple.hpp"

namespace rfl::parsing::schemaful {

template <int _i>
inline consteval auto to_field_name() {
  return internal::StringLiteral<5>('f',
                                    static_cast<char>('0' + ((_i / 100) % 10)),
                                    static_cast<char>('0' + ((_i / 10) % 10)),
                                    static_cast<char>('0' + (_i % 10)));
}

template <int _i>
inline auto to_field(const auto& _t) {
  using T = std::remove_cvref_t<decltype(_t)>;
  return rfl::Field<to_field_name<_i>(), const T*>(&_t);
}

/// Schemaful formats often don't have an explicit tuple representation.
/// This is the required workaround.
template <class... Ts>
auto tuple_to_named_tuple(const Tuple<Ts...>& _tup) {
  static_assert(sizeof...(Ts) <= 1000,
                "The tuple cannot contain more than 1000 elements.");
  return [&]<int... _is>(std::integer_sequence<int, _is...>) {
    return rfl::make_named_tuple(to_field<_is>(rfl::get<_is>(_tup))...);
  }(std::make_integer_sequence<int, sizeof...(Ts)>());
}

/// Schemaful formats often don't have an explicit tuple representation.
/// This is the required workaround.
template <class... Ts>
auto tuple_to_named_tuple(const std::tuple<Ts...>& _tup) {
  static_assert(sizeof...(Ts) <= 1000,
                "The tuple cannot contain more than 1000 elements.");
  return [&]<int... _is>(std::integer_sequence<int, _is...>) {
    return rfl::make_named_tuple(to_field<_is>(std::get<_is>(_tup))...);
  }(std::make_integer_sequence<int, sizeof...(Ts)>());
}

}  // namespace rfl::parsing::schemaful

#endif
