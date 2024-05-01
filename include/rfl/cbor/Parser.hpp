#ifndef RFL_CBOR_PARSER_HPP_
#define RFL_CBOR_PARSER_HPP_

#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl {
namespace parsing {

/// CBOR requires us to explicitly set the number of fields in advance. Because
/// of that, we require all of the fields and then set them to nullptr, if
/// necessary.
template <class ProcessorsType, class... FieldTypes>
requires AreReaderAndWriter<cbor::Reader, cbor::Writer,
                            NamedTuple<FieldTypes...>>
struct Parser<cbor::Reader, cbor::Writer, NamedTuple<FieldTypes...>,
              ProcessorsType>
    : public NamedTupleParser<cbor::Reader, cbor::Writer,
                              /*_ignore_empty_containers=*/false,
                              /*_all_required=*/true, ProcessorsType,
                              FieldTypes...> {
};

template <class ProcessorsType, class... Ts>
requires AreReaderAndWriter<cbor::Reader, cbor::Writer, std::tuple<Ts...>>
struct Parser<cbor::Reader, cbor::Writer, std::tuple<Ts...>, ProcessorsType>
    : public TupleParser<cbor::Reader, cbor::Writer,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType, Ts...> {
};

}  // namespace parsing
}  // namespace rfl

namespace rfl {
namespace cbor {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}
}  // namespace rfl

#endif
