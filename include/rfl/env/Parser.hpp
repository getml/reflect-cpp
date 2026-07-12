#ifndef RFL_ENV_PARSER_HPP_
#define RFL_ENV_PARSER_HPP_

#include "../parsing/AreReaderAndWriter.hpp"
#include "../parsing/Parser_base.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl::parsing {

template <class ProcessorsType, class... FieldTypes>
  requires AreReaderAndWriter<env::Reader, env::Writer,
                              NamedTuple<FieldTypes...>>
struct Parser<env::Reader, env::Writer, NamedTuple<FieldTypes...>,
              ProcessorsType>
    : public NamedTupleParser<
          env::Reader, env::Writer,
          /*_ignore_empty_containers=*/true,
          /*_all_required=*/internal::no_optionals_v<ProcessorsType>,
          /*_no_field_names_=*/false, ProcessorsType, FieldTypes...> {};

}  // namespace rfl::parsing

namespace rfl::env {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}  // namespace rfl::env

#endif
