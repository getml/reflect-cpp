#ifndef RFL_AVRO_PARSER_HPP_
#define RFL_AVRO_PARSER_HPP_

#include "../Generic.hpp"
#include "../Tuple.hpp"
#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl {
namespace parsing {

/// AVRO requires us to explicitly set all fields. Because
/// of that, we require all of the fields and then set them to nullptr, if
/// necessary.
template <class ProcessorsType, class... FieldTypes>
  requires AreReaderAndWriter<avro::Reader, avro::Writer,
                              NamedTuple<FieldTypes...>>
struct Parser<avro::Reader, avro::Writer, NamedTuple<FieldTypes...>,
              ProcessorsType>
    : public NamedTupleParser<
          avro::Reader, avro::Writer,
          /*_ignore_empty_containers=*/false,
          /*_all_required=*/true,
          /*_no_field_names=*/ProcessorsType::no_field_names_, ProcessorsType,
          FieldTypes...> {};

template <class ProcessorsType, class... Ts>
  requires AreReaderAndWriter<avro::Reader, avro::Writer, rfl::Tuple<Ts...>>
struct Parser<avro::Reader, avro::Writer, rfl::Tuple<Ts...>, ProcessorsType>
    : public TupleParser<avro::Reader, avro::Writer,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType,
                         rfl::Tuple<Ts...>> {};

template <class ProcessorsType, class... Ts>
  requires AreReaderAndWriter<avro::Reader, avro::Writer, std::tuple<Ts...>>
struct Parser<avro::Reader, avro::Writer, std::tuple<Ts...>, ProcessorsType>
    : public TupleParser<avro::Reader, avro::Writer,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType,
                         std::tuple<Ts...>> {};

template <class ProcessorsType>
  requires AreReaderAndWriter<avro::Reader, avro::Writer, Generic>
struct Parser<avro::Reader, avro::Writer, Generic, ProcessorsType> {
  using InputVarType = typename avro::Reader::InputVarType;

  static Result<Generic> read(const avro::Reader &,
                              const InputVarType &) noexcept {
    static_assert(false, "Generics are unsupported in Avro.");
    return Error("Unsupported");
  }

  template <class P>
  static void write(const avro::Writer &, const Generic &, const P &) noexcept {
    static_assert(false, "Generics are unsupported in Avro.");
  }

  static schema::Type to_schema(std::map<std::string, schema::Type> *) {
    static_assert(false, "Generics are unsupported in Avro.");
    return schema::Type{};
  }
};

}  // namespace parsing
}  // namespace rfl

namespace rfl {
namespace avro {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}
}  // namespace rfl

#endif
