#ifndef RFL_MSGPACK_PARSER_HPP_
#define RFL_MSGPACK_PARSER_HPP_

#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl {
namespace parsing {

/// msgpack-c requires us to explicitly set the number of fields in advance.
/// Because of that, we require all of the fields and then set them to nullptr,
/// if necessary.
template <class ProcessorsType, class... FieldTypes>
requires AreReaderAndWriter<msgpack::Reader, msgpack::Writer,
                            NamedTuple<FieldTypes...>>
struct Parser<msgpack::Reader, msgpack::Writer, NamedTuple<FieldTypes...>,
              ProcessorsType>
    : public NamedTupleParser<msgpack::Reader, msgpack::Writer,
                              /*_ignore_empty_containers=*/false,
                              /*_all_required=*/true, ProcessorsType,
                              FieldTypes...> {
};

template <class ProcessorsType, class... Ts>
requires AreReaderAndWriter<msgpack::Reader, msgpack::Writer, std::tuple<Ts...>>
struct Parser<msgpack::Reader, msgpack::Writer, std::tuple<Ts...>,
              ProcessorsType>
    : public TupleParser<msgpack::Reader, msgpack::Writer,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType, Ts...> {
};

}  // namespace parsing
}  // namespace rfl

namespace rfl {
namespace msgpack {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}
}  // namespace rfl

#endif
