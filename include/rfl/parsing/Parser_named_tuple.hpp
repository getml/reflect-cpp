#ifndef RFL_PARSING_PARSER_NAMED_TUPLE_HPP_
#define RFL_PARSING_PARSER_NAMED_TUPLE_HPP_

#include "NamedTupleParser.hpp"
#include "Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class... FieldTypes>
requires AreReaderAndWriter<R, W, NamedTuple<FieldTypes...>>
struct Parser<R, W, NamedTuple<FieldTypes...>>
    : public NamedTupleParser<R, W, /*_ignore_empty_containers=*/false,
                              /*_all_required=*/false, FieldTypes...> {
};

}  // namespace parsing
}  // namespace rfl

#endif
