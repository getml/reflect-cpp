#ifndef RFL_ENV_PARSER_HPP_
#define RFL_ENV_PARSER_HPP_

#include "../NamedTuple.hpp"
#include "../internal/no_optionals_v.hpp"
#include "../parsing/AreReaderAndWriter.hpp"
#include "../parsing/NamedTupleParser.hpp"
#include "../parsing/Parser_base.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl::parsing {

/// Specializes the Parser template for NamedTuple types within the env
/// namespace. This specialization inherits from NamedTupleParser to handle
/// serialization and deserialization of NamedTuple structures into/from
/// environment variables.
///
/// Key configuration:
/// - _ignore_empty_containers: true (empty containers are ignored during
/// parsing)
/// - _all_required: determined by no_optionals_v<ProcessorsType> (fields are
/// required if the processors do not contain the NoOptional preprocessor)
/// - _no_field_names: false (field names are preserved in the output)
///
/// The AreReaderAndWriter concept ensures that env::Reader and env::Writer can
/// handle NamedTuple<FieldTypes...> types.
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
