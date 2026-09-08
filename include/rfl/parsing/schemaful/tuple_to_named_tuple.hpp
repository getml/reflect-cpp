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

/**
 * @brief Generates a field name for a tuple element.
 *
 * @tparam _i The index of the element.
 * @return The generated field name.
 */
template <int _i>
inline consteval auto to_field_name() {
  return internal::StringLiteral<5>('f',
                                    static_cast<char>('0' + ((_i / 100) % 10)),
                                    static_cast<char>('0' + ((_i / 10) % 10)),
                                    static_cast<char>('0' + (_i % 10)));
}

/**
 * @brief Converts a tuple element to a field.
 *
 * @tparam _i The index of the element.
 * @param _t The element to convert.
 * @return The generated field.
 */
template <int _i>
inline auto to_field(const auto& _t) {
  using T = std::remove_cvref_t<decltype(_t)>;
  return rfl::Field<to_field_name<_i>(), const T*>(&_t);
}

/**
 * @brief Converts a Tuple to a NamedTuple.
 *
 * @tparam Ts The types in the tuple.
 * @param _tup The tuple to convert.
 * @return The generated NamedTuple.
 */
template <class... Ts>
auto tuple_to_named_tuple(const Tuple<Ts...>& _tup) {
  static_assert(sizeof...(Ts) <= 1000,
                "The tuple cannot contain more than 1000 elements.");
  return [&]<int... _is>(std::integer_sequence<int, _is...>) {
    return rfl::make_named_tuple(to_field<_is>(rfl::get<_is>(_tup))...);
  }(std::make_integer_sequence<int, sizeof...(Ts)>());
}

/**
 * @brief Converts a std::tuple to a NamedTuple.
 *
 * @tparam Ts The types in the tuple.
 * @param _tup The tuple to convert.
 * @return The generated NamedTuple.
 */
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
