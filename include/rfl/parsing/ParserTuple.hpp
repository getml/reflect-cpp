#ifndef RFL_PARSING_PARSER_TUPLE_HPP_
#define RFL_PARSING_PARSER_TUPLE_HPP_

#include <tuple>

#include "../internal/no_optionals_v.hpp"
#include "Parser_base.hpp"
#include "TupleParser.hpp"

namespace rfl::parsing {

template <class T>
struct is_tuple : std::false_type {};

template <class... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type {};

template <class T>
constexpr bool is_tuple_v = is_tuple<std::remove_cvref_t<T>>::value;

/**
 * @brief Parser specialization for std::tuple.
 */
template <class R, class W, class T, class ProcessorsType>
struct ParserTuple
    : public TupleParser<
          R, W, /*_ignore_empty_containers=*/false,
          /*_all_required=*/internal::no_optionals_v<ProcessorsType>,
          ProcessorsType, T> {};

}  // namespace rfl::parsing

#endif
