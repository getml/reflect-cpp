#ifndef RFL_YAS_PARSER_HPP_
#define RFL_YAS_PARSER_HPP_

#include "../Generic.hpp"
#include "../NamedTuple.hpp"
#include "../Tuple.hpp"
#include "../always_false.hpp"
#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl {
namespace parsing {

template <class IArchive, class OArchive, class ProcessorsType,
          class... FieldTypes>
  requires AreReaderAndWriter<yas::Reader<IArchive>, yas::Writer<OArchive>,
                              NamedTuple<FieldTypes...>>
struct Parser<yas::Reader<IArchive>, yas::Writer<OArchive>,
              NamedTuple<FieldTypes...>, ProcessorsType>
    : public NamedTupleParser<yas::Reader<IArchive>, yas::Writer<OArchive>,
                              /*_ignore_empty_containers=*/false,
                              /*_all_required=*/true,
                              /*_no_field_names=*/true, ProcessorsType,
                              FieldTypes...> {};

template <class IArchive, class OArchive, class ProcessorsType, class... Ts>
  requires AreReaderAndWriter<yas::Reader<IArchive>, yas::Writer<OArchive>,
                              rfl::Tuple<Ts...>>
struct Parser<yas::Reader<IArchive>, yas::Writer<OArchive>, rfl::Tuple<Ts...>,
              ProcessorsType>
    : public TupleParser<yas::Reader<IArchive>, yas::Writer<OArchive>,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType,
                         rfl::Tuple<Ts...>> {};

template <class IArchive, class OArchive, class ProcessorsType, class... Ts>
  requires AreReaderAndWriter<yas::Reader<IArchive>, yas::Writer<OArchive>,
                              std::tuple<Ts...>>
struct Parser<yas::Reader<IArchive>, yas::Writer<OArchive>, std::tuple<Ts...>,
              ProcessorsType>
    : public TupleParser<yas::Reader<IArchive>, yas::Writer<OArchive>,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType,
                         std::tuple<Ts...>> {};

template <class IArchive, class OArchive, class ProcessorsType>
  requires AreReaderAndWriter<yas::Reader<IArchive>, yas::Writer<OArchive>,
                              Generic>
struct Parser<yas::Reader<IArchive>, yas::Writer<OArchive>, Generic,
              ProcessorsType> {
  template <class T>
  static Result<Generic> read(const yas::Reader<IArchive>&, const T&) noexcept {
    static_assert(always_false_v<T>, "Generics are unsupported in yas.");
    return error("Unsupported");
  }

  template <class P>
  static void write(const yas::Writer<OArchive>&, const Generic&,
                    const P&) noexcept {
    static_assert(always_false_v<P>, "Generics are unsupported in yas.");
  }

  template <class T>
  static schema::Type to_schema(T*) {
    static_assert(always_false_v<T>, "Generics are unsupported in yas.");
    return schema::Type{};
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
