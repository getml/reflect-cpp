#ifndef RFL_PARSING_PARSER_TUPLE_HPP_
#define RFL_PARSING_PARSER_TUPLE_HPP_

#include <tuple>

#include "../internal/no_optionals_v.hpp"
#include "Parser_base.hpp"
#include "TupleParser.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class... Ts, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::tuple<Ts...>>
struct Parser<R, W, std::tuple<Ts...>, ProcessorsType>
    : public TupleParser<
          R, W, /*_ignore_empty_containers=*/false,
          /*_all_required=*/internal::no_optionals_v<ProcessorsType>,
          ProcessorsType, std::tuple<Ts...>> {};

}  // namespace parsing
}  // namespace rfl

#endif
