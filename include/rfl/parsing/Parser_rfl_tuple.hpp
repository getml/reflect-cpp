#ifndef RFL_PARSING_PARSER_RFL_TUPLE_HPP_
#define RFL_PARSING_PARSER_RFL_TUPLE_HPP_

#include "../Tuple.hpp"
#include "Parser_base.hpp"
#include "TupleParser.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class... Ts, class ProcessorsType>
requires AreReaderAndWriter<R, W, rfl::Tuple<Ts...>>
struct Parser<R, W, rfl::Tuple<Ts...>, ProcessorsType>
    : public TupleParser<R, W, /*_ignore_empty_containers=*/false,
                         /*_all_required=*/ProcessorsType::all_required_,
                         ProcessorsType, rfl::Tuple<Ts...>> {
};

}  // namespace parsing
}  // namespace rfl

#endif
