#ifndef RFL_PARSING_PARSER_NAMED_TUPLE_HPP_
#define RFL_PARSING_PARSER_NAMED_TUPLE_HPP_

#include "../NamedTuple.hpp"
#include "../Processors.hpp"
#include "../internal/no_field_names_v.hpp"
#include "../internal/no_optionals_v.hpp"
#include "NamedTupleParser.hpp"
#include "Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class... FieldTypes, class ProcessorsType>
  requires AreReaderAndWriter<R, W, NamedTuple<FieldTypes...>>
struct Parser<R, W, NamedTuple<FieldTypes...>, ProcessorsType>
    : public NamedTupleParser<
          R, W, /*_ignore_empty_containers=*/false,
          /*_all_required=*/internal::no_optionals_v<ProcessorsType>,
          /*_no_field_names=*/internal::no_field_names_v<ProcessorsType>,
          ProcessorsType, FieldTypes...> {};

}  // namespace parsing
}  // namespace rfl

#endif
