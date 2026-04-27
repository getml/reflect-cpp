#ifndef RFL_YAS_PARSER_HPP_
#define RFL_YAS_PARSER_HPP_

#include "../Generic.hpp"
#include "../NamedTuple.hpp"
#include "../Tuple.hpp"
#include "../always_false.hpp"
#include "../internal/no_field_names_v.hpp"
#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl {
namespace parsing {

/// Yas requires us to explicitly set the number of fields in advance.
/// Because of that, we require all of the fields and then set them to nullptr,
/// if necessary.
template <class ProcessorsType, class... FieldTypes>
  requires AreReaderAndWriter<yas::Reader, yas::Writer,
                              NamedTuple<FieldTypes...>>
struct Parser<yas::Reader, yas::Writer, NamedTuple<FieldTypes...>,
              ProcessorsType>
    : public NamedTupleParser<
          yas::Reader, yas::Writer,
          /*_ignore_empty_containers=*/false,
          /*_all_required=*/true,
          /*_no_field_names=*/internal::no_field_names_v<ProcessorsType>,
          ProcessorsType, FieldTypes...> {};

template <class ProcessorsType, class... Ts>
  requires AreReaderAndWriter<yas::Reader, yas::Writer, rfl::Tuple<Ts...>>
struct Parser<yas::Reader, yas::Writer, rfl::Tuple<Ts...>, ProcessorsType>
    : public TupleParser<yas::Reader, yas::Writer,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType,
                         rfl::Tuple<Ts...>> {};

template <class ProcessorsType, class... Ts>
  requires AreReaderAndWriter<yas::Reader, yas::Writer, std::tuple<Ts...>>
struct Parser<yas::Reader, yas::Writer, std::tuple<Ts...>, ProcessorsType>
    : public TupleParser<yas::Reader, yas::Writer,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType,
                         std::tuple<Ts...>> {};

}  // namespace parsing
}  // namespace rfl

namespace rfl {
namespace yas {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}
}  // namespace rfl

#endif
