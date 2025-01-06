#ifndef RFL_CBOR_PARSER_HPP_
#define RFL_CBOR_PARSER_HPP_

#include "../Tuple.hpp"
#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl::parsing {

/// CBOR requires us to explicitly set the number of fields in advance.
/// Because of that, we require all of the fields and then set them to nullptr,
/// if necessary.
template <class ProcessorsType, class... FieldTypes>
  requires AreReaderAndWriter<cbor::Reader, cbor::Writer,
                              NamedTuple<FieldTypes...>>
struct Parser<cbor::Reader, cbor::Writer, NamedTuple<FieldTypes...>,
              ProcessorsType>
    : public NamedTupleParser<
          cbor::Reader, cbor::Writer,
          /*_ignore_empty_containers=*/false,
          /*_all_required=*/true,
          /*_no_field_names=*/ProcessorsType::no_field_names_, ProcessorsType,
          FieldTypes...> {};

template <class ProcessorsType, class... Ts>
  requires AreReaderAndWriter<cbor::Reader, cbor::Writer, rfl::Tuple<Ts...>>
struct Parser<cbor::Reader, cbor::Writer, rfl::Tuple<Ts...>, ProcessorsType>
    : public TupleParser<cbor::Reader, cbor::Writer,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType,
                         rfl::Tuple<Ts...>> {};

template <class ProcessorsType, class... Ts>
  requires AreReaderAndWriter<cbor::Reader, cbor::Writer, std::tuple<Ts...>>
struct Parser<cbor::Reader, cbor::Writer, std::tuple<Ts...>, ProcessorsType>
    : public TupleParser<cbor::Reader, cbor::Writer,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType,
                         std::tuple<Ts...>> {};

}  // namespace rfl::parsing

namespace rfl::cbor {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}  // namespace rfl::cbor

#endif
